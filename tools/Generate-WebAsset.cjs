const fs = require('node:fs');
const path = require('node:path');
const zlib = require('node:zlib');
const assert = require('node:assert/strict');

function renderHeader(compressed, sourceBytes) {
  const rows = [];
  for (let offset = 0; offset < compressed.length; offset += 16) {
    const values = [...compressed.subarray(offset, offset + 16)]
      .map((value) => `0x${value.toString(16).padStart(2, '0')}`);
    rows.push(`    ${values.join(', ')},`);
  }
  return `#pragma once

#include <Arduino.h>

// Generated from web/HeliosWebPage.html by tools/Generate-WebAsset.cjs.
// Edit the HTML source, not this byte array.
constexpr size_t HELIOS_WEB_PAGE_SOURCE_SIZE = ${sourceBytes.length};
constexpr size_t HELIOS_WEB_PAGE_GZIP_SIZE = ${compressed.length};
const uint8_t HELIOS_WEB_PAGE_GZIP[] PROGMEM = {
${rows.join('\n')}
};
static_assert(sizeof(HELIOS_WEB_PAGE_GZIP) == HELIOS_WEB_PAGE_GZIP_SIZE,
              "Web page gzip size mismatch");
`;
}

function build(root, checkOnly) {
  const htmlPath = path.join(root, 'web', 'HeliosWebPage.html');
  const source = fs.readFileSync(htmlPath);
  const compressed = zlib.gzipSync(source, { level: 9, mtime: 0 });
  assert(zlib.gunzipSync(compressed).equals(source), 'Gzip round trip failed');
  const expected = renderHeader(compressed, source);
  const outputs = [
    path.join(root, 'HELIOS_HUNTER_ILI9341', 'HeliosWebPageGzip.h'),
    path.join(root, 'HELIOS_HUNTER_ST7789', 'HeliosWebPageGzip.h')
  ];
  for (const output of outputs) {
    if (checkOnly) {
      assert.equal(fs.readFileSync(output, 'utf8'), expected,
        `${path.relative(root, output)} is stale`);
    } else {
      fs.writeFileSync(output, expected);
    }
  }
  console.log(JSON.stringify({
    sourceBytes: source.length,
    gzipBytes: compressed.length,
    savedBytesPerFirmware: source.length - compressed.length,
    mode: checkOnly ? 'checked' : 'generated'
  }));
}

const args = process.argv.slice(2);
const checkOnly = args[0] === '--check';
const root = path.resolve(args[checkOnly ? 1 : 0] || '.');
build(root, checkOnly);
