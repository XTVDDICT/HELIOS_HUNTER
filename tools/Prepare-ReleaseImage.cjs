const fs = require('node:fs');
const crypto = require('node:crypto');
const assert = require('node:assert/strict');

const digest = (bytes) => crypto.createHash('sha256').update(bytes).digest();
const localPath = /[A-Z]:[\\/]+Users[\\/]+[^\x00\r\n]+/gi;

// Espressif's unsigned ESP32 image format: 24-byte header, segments, XOR
// checksum, optional SHA-256 footer. Signed or populated images are rejected.
// https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/firmware-image-format.html
function readImage(bytes, offset) {
  assert.equal(bytes[offset], 0xe9, 'Missing ESP32 image header');
  const count = bytes[offset + 1];
  assert(count > 0 && count <= 16, 'Invalid segment count');
  assert.equal(bytes.readUInt16LE(offset + 12), 0, 'Not an ESP32 image');
  assert(bytes[offset + 23] <= 1, 'Invalid hash-appended flag');
  const segments = [];
  let cursor = offset + 24;
  let checksum = 0xef;
  for (let i = 0; i < count; i++) {
    assert(cursor + 8 <= bytes.length, 'Truncated segment header');
    const address = bytes.readUInt32LE(cursor);
    const length = bytes.readUInt32LE(cursor + 4);
    cursor += 8;
    assert(length > 0 && cursor + length <= bytes.length, 'Truncated segment');
    segments.push({ address, start: cursor, end: cursor + length });
    for (let j = cursor; j < cursor + length; j++) checksum ^= bytes[j];
    cursor += length;
  }
  const checksumOffset = offset + Math.ceil((cursor - offset + 1) / 16) * 16 - 1;
  assert(checksumOffset < bytes.length, 'Missing checksum');
  assert.equal(bytes[checksumOffset], checksum, 'Invalid original checksum');
  const hashOffset = bytes[offset + 23] ? checksumOffset + 1 : null;
  const end = hashOffset === null ? checksumOffset + 1 : hashOffset + 32;
  assert(end <= bytes.length, 'Truncated hash footer');
  if (hashOffset !== null) {
    assert(digest(bytes.subarray(offset, hashOffset)).equals(
      bytes.subarray(hashOffset, end)), 'Invalid original SHA-256 footer');
  }
  return { offset, segments, checksumOffset, hashOffset, end };
}

function readPartitions(bytes) {
  const result = [];
  for (let cursor = 0x8000; cursor < 0x9000; cursor += 32) {
    const magic = bytes.readUInt16LE(cursor);
    if (magic === 0xffff || magic === 0xebeb) break;
    assert.equal(magic, 0x50aa, 'Invalid partition table');
    const type = bytes[cursor + 2];
    const subtype = bytes[cursor + 3];
    const offset = bytes.readUInt32LE(cursor + 4);
    const size = bytes.readUInt32LE(cursor + 8);
    assert(size > 0 && offset + size <= bytes.length, 'Invalid partition range');
    result.push({ type, subtype, offset, size });
  }
  assert(result.length > 0, 'Missing partitions');
  return result;
}

function isBlank(bytes, start, end) {
  return bytes.subarray(start, end).every((byte) => byte === 0xff);
}

function prepare(original, requireBuild = true) {
  assert.equal(original.length, 4 * 1024 * 1024, 'Expected a 4 MB merged image');
  const partitions = readPartitions(original);
  const apps = partitions.filter((p) => p.type === 0);
  assert.equal(apps.length, 1, 'Expected one application partition');
  // Arduino Huge APP uses a single app0 (OTA_0) partition despite no OTA UI.
  assert([0, 0x10].includes(apps[0].subtype), 'Unexpected application subtype');
  const nvs = partitions.filter((p) => p.type === 1 && p.subtype === 2);
  assert(nvs.length > 0, 'Missing NVS partition');
  for (const p of nvs) {
    assert(isBlank(original, p.offset, p.offset + p.size),
      'NVS contains data: do not publish a device flash dump');
  }
  for (const p of partitions.filter((p) => p.type === 1 && p.subtype !== 0)) {
    assert(isBlank(original, p.offset, p.offset + p.size),
      'User-data partition contains data: manual privacy review required');
  }
  const images = [readImage(original, 0x1000), readImage(original, apps[0].offset)];
  assert(images[0].end <= 0x8000, 'Bootloader overlaps partition table');
  assert(images[1].end <= apps[0].offset + apps[0].size, 'Application exceeds partition');
  assert(isBlank(original, images[0].end, 0x8000), 'Bootloader has extra/signature data');
  assert(isBlank(original, images[1].end, apps[0].offset + apps[0].size),
    'Application has extra/signature data');
  if (requireBuild) {
    assert(original.includes(Buffer.from('R12_SHA_REFERENCE_NATIVE_20260916')),
      'Not the R12 release build');
    assert(original.includes(Buffer.from('HELIOS_HUNTER/1.0.7')), 'Wrong firmware version');
  }
  const output = Buffer.from(original);
  const allowed = new Uint8Array(output.length);
  let patchedPaths = 0;
  for (const image of images) {
    for (const segment of image.segments) {
      const text = original.subarray(segment.start, segment.end).toString('latin1');
      for (const match of text.matchAll(localPath)) {
        assert(segment.address >= 0x3f400000 && segment.address < 0x3f800000,
          'Local path outside read-only string segment: manual review required');
        const prefixLength = Math.max(match[0].lastIndexOf('/'), match[0].lastIndexOf('\\')) + 1;
        assert(prefixLength >= 6, 'Invalid path prefix');
        const replacement = Buffer.from('build/' + '_'.repeat(prefixLength - 6), 'ascii');
        const start = segment.start + match.index;
        replacement.copy(output, start);
        allowed.fill(1, start, start + replacement.length);
        patchedPaths++;
      }
    }
    let checksum = 0xef;
    for (const segment of image.segments) {
      for (let j = segment.start; j < segment.end; j++) checksum ^= output[j];
    }
    output[image.checksumOffset] = checksum;
    allowed[image.checksumOffset] = 1;
    if (image.hashOffset !== null) {
      digest(output.subarray(image.offset, image.hashOffset)).copy(output, image.hashOffset);
      allowed.fill(1, image.hashOffset, image.end);
    }
    readImage(output, image.offset);
  }
  assert(!localPath.test(output.toString('latin1')), 'Personal debug path remains');
  for (let i = 0; i < output.length; i++) {
    assert(output[i] === original[i] || allowed[i], 'Unexpected change outside diagnostic strings/footer');
  }
  return { output, patchedPaths, images };
}

function tests() {
  function image(segments) {
    const length = 24 + segments.reduce((sum, s) => sum + 8 + s.data.length, 0);
    const hashOffset = Math.ceil((length + 1) / 16) * 16;
    const result = Buffer.alloc(hashOffset + 32);
    result[0] = 0xe9;
    result[1] = segments.length;
    result[23] = 1;
    let cursor = 24;
    let checksum = 0xef;
    for (const s of segments) {
      result.writeUInt32LE(s.address, cursor);
      result.writeUInt32LE(s.data.length, cursor + 4);
      s.data.copy(result, cursor + 8);
      for (const byte of s.data) checksum ^= byte;
      cursor += 8 + s.data.length;
    }
    result[hashOffset - 1] = checksum;
    digest(result.subarray(0, hashOffset)).copy(result, hashOffset);
    return result;
  }
  const input = Buffer.alloc(4 * 1024 * 1024, 0xff);
  const boot = image([{ address: 0x3fff0000, data: Buffer.from('boot') }]);
  const app = image([
    { address: 0x3f400020, data: Buffer.from('C:/Users/private/project/diag.h\0') },
    { address: 0x400d0020, data: Buffer.from([1, 2, 3, 4]) }
  ]);
  boot.copy(input, 0x1000);
  app.copy(input, 0x10000);
  for (const [index, type, subtype, offset, size] of [
    [0, 1, 2, 0x9000, 0x5000], [1, 0, 0, 0x10000, 0x300000]
  ]) {
    const cursor = 0x8000 + index * 32;
    input.writeUInt16LE(0x50aa, cursor);
    input[cursor + 2] = type;
    input[cursor + 3] = subtype;
    input.writeUInt32LE(offset, cursor + 4);
    input.writeUInt32LE(size, cursor + 8);
  }
  const result = prepare(input, false);
  assert.equal(result.patchedPaths, 1);
  assert(result.output.includes(Buffer.from('diag.h\0')));
  assert(!result.output.includes(Buffer.from('private')));
  assert.equal(prepare(result.output, false).patchedPaths, 0);
  const app0 = Buffer.from(input);
  app0[0x8023] = 0x10;
  assert.equal(prepare(app0, false).patchedPaths, 1);
  const badChecksum = Buffer.from(input);
  badChecksum[0x10000 + app.length - 33] ^= 1;
  assert.throws(() => prepare(badChecksum, false), /checksum/);
  const badHash = Buffer.from(input);
  badHash[0x10000 + app.length - 1] ^= 1;
  assert.throws(() => prepare(badHash, false), /SHA-256 footer/);
  const populated = Buffer.from(input);
  populated[0x9000] = 0;
  assert.throws(() => prepare(populated, false), /NVS contains data/);
  const signed = Buffer.from(input);
  signed[0x10000 + app.length + 32] = 0xe7;
  assert.throws(() => prepare(signed, false), /extra\/signature/);
  assert.throws(() => prepare(input), /Not the R12/);
  console.log('PASS: path removal, unchanged code, image checksums/hashes, idempotence, populated-NVS and signature rejection.');
}

if (require.main === module) {
  if (process.argv[2] === '--test') {
    tests();
  } else {
    const [source, destination] = process.argv.slice(2);
    assert(source && destination, 'Usage: node Prepare-ReleaseImage.cjs input-merged.bin output-merged.bin');
    assert(source !== destination, 'Do not overwrite the original image');
    const original = fs.readFileSync(source);
    const result = prepare(original);
    fs.writeFileSync(destination, result.output, { flag: 'wx' });
    console.log(JSON.stringify({
      patchedDebugPaths: result.patchedPaths,
      originalSha256: digest(original).toString('hex'),
      releaseSha256: digest(result.output).toString('hex'),
      bytes: result.output.length,
      blankNvs: true,
      instructionsUnchanged: true,
      imageIntegrityValid: true
    }));
  }
}

module.exports = { prepare, readImage };
