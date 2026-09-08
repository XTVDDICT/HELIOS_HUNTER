#include "HeliosWeb.h"

#include <ArduinoJson.h>
#include <WiFi.h>

#include "HeliosMiner.h"

namespace {

const char INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html><html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>HELIOS_HUNTER</title><style>
:root{color-scheme:dark;--bg:#080b0b;--surface:#111715;--surface2:#0b100e;--line:#2a3631;--ink:#f2f6f3;--muted:#98a59f;--green:#43dd83;--red:#ef7474}*{box-sizing:border-box;letter-spacing:0}body{margin:0;background:var(--bg);color:var(--ink);font:14px/1.45 system-ui,-apple-system,"Segoe UI",sans-serif}header{height:64px;display:flex;align-items:center;gap:14px;padding:0 22px;border-bottom:1px solid var(--line);background:#0c110f;position:sticky;top:0;z-index:3}.brand{font-size:20px;font-weight:800}.brand b{color:var(--green)}.connection{margin-left:auto;color:var(--muted);display:flex;align-items:center;gap:8px}.dot{width:8px;height:8px;border-radius:50%;background:var(--red)}.dot.on{background:var(--green)}nav{display:flex;overflow:auto;padding:0 14px;border-bottom:1px solid var(--line);background:#0c110f}.tab{border:0;border-bottom:3px solid transparent;background:none;color:var(--muted);padding:13px 15px;font:inherit;font-weight:750;cursor:pointer}.tab.active{color:var(--ink);border-color:var(--green)}main{max-width:1080px;margin:auto;padding:21px 18px}.pane{display:none}.pane.active{display:block}.heading{display:flex;align-items:flex-end;justify-content:space-between;gap:16px;margin-bottom:17px}h1{font-size:25px;margin:0}h2{font-size:17px;margin:0 0 13px}.sub{color:var(--muted);margin-top:4px;overflow-wrap:anywhere}.badge{padding:5px 9px;border:1px solid var(--line);border-radius:5px;color:var(--muted);font-size:12px;font-weight:800}.badge.active{border-color:var(--green);color:var(--green)}.stats{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));border:1px solid var(--line);border-radius:7px;background:var(--surface);margin-bottom:18px}.coin-stats{grid-template-columns:repeat(3,minmax(0,1fr))}.stat{min-width:0;padding:15px;border-right:1px solid var(--line)}.stats .stat:last-child,.coin-stats .stat:nth-child(3n){border-right:0}.coin-stats .stat:nth-child(-n+3){border-bottom:1px solid var(--line)}.key{color:var(--muted);font-size:11px;font-weight:800;text-transform:uppercase}.value{font-size:20px;font-weight:750;margin-top:5px;overflow-wrap:anywhere}.section{border-top:1px solid var(--line);padding:19px 0}.grid{display:grid;grid-template-columns:1fr 1fr;gap:14px}.full{grid-column:1/-1}.field label{display:block;color:var(--muted);font-weight:700;margin-bottom:6px}.field input{width:100%;height:43px;border:1px solid var(--line);border-radius:5px;background:var(--surface2);color:var(--ink);padding:0 11px;font:inherit}.field input:focus{outline:2px solid #246b42;border-color:var(--green)}.toggle{display:flex;align-items:center;gap:10px;min-height:43px}.toggle input{width:19px;height:19px;accent-color:var(--green)}.actions{display:flex;gap:10px;flex-wrap:wrap;margin-top:16px}.btn{height:40px;border:1px solid var(--line);border-radius:5px;background:#19211e;color:var(--ink);padding:0 15px;font-weight:800;cursor:pointer}.btn.primary{background:var(--green);border-color:var(--green);color:#06110a}.btn.danger{border-color:#6b3636;color:#ffaaaa}.notice{display:none;margin-bottom:16px;padding:10px 12px;border:1px solid var(--green);border-radius:5px;color:var(--green)}.pool{font-family:ui-monospace,SFMono-Regular,Consolas,monospace;color:var(--muted);overflow-wrap:anywhere}@media(max-width:700px){header{padding:0 13px}.brand{font-size:16px}.connection span{display:none}nav{padding:0 3px}.tab{padding:12px 11px}main{padding:16px 12px}.stats,.coin-stats{grid-template-columns:1fr 1fr}.stat,.coin-stats .stat{border-right:1px solid var(--line);border-bottom:1px solid var(--line)}.stat:nth-child(2n){border-right:0}.stats .stat:nth-last-child(-n+2),.coin-stats .stat:nth-last-child(-n+2){border-bottom:0}.grid{grid-template-columns:1fr}.full{grid-column:auto}.heading{align-items:flex-start}.value{font-size:18px}}
.field select{width:100%;height:43px;border:1px solid var(--line);border-radius:5px;background:var(--surface2);color:var(--ink);padding:0 11px;font:inherit}.field select:focus{outline:2px solid #246b42;border-color:var(--green)}
</style></head><body><header><div class="brand">Helios<b>Pool</b> / HELIOS_HUNTER</div><div class="connection"><i id="dot" class="dot"></i><span id="connection">Connecting</span><strong id="ip"></strong></div></header><nav id="tabs"><button class="tab active" data-pane="home">Dashboard</button><button class="tab" data-pane="mining">Mining</button><button class="tab" data-pane="CHTA">CHTA</button><button class="tab" data-pane="WJK">WJK</button><button class="tab" data-pane="DGB">DGB</button><button class="tab" data-pane="BCH">BCH</button><button class="tab" data-pane="BTC">BTC</button><button class="tab" data-pane="device">Device</button></nav><main><div id="notice" class="notice"></div>
<section id="home" class="pane active"><div class="heading"><div><h1>Mining dashboard</h1><div class="sub">Independent SHA-256d Stratum miner</div></div><span id="homeBadge" class="badge">STARTING</span></div><div class="stats"><div class="stat"><div class="key">Engine</div><div id="engine" class="value">--</div></div><div class="stat"><div class="key">Hashrate</div><div id="hashrate" class="value">--</div></div><div class="stat"><div class="key">Accepted / Rejected</div><div id="shares" class="value">0 / 0</div></div><div class="stat"><div class="key">Best difficulty</div><div id="best" class="value">0</div></div></div><div class="section"><h2>Current pool</h2><div id="activePool" class="pool">--</div><div class="actions"><button id="openMining" class="btn primary">Mining settings</button><button id="stopMining" class="btn danger">Stop mining</button></div></div></section>
<section id="mining" class="pane"><div class="heading"><div><h1>Mining settings</h1><div class="sub">Connect to any compatible SHA-256d Stratum pool.</div></div><span class="badge">GLOBAL</span></div><form id="miningForm" class="section"><div class="grid"><label class="toggle full"><input id="enabled" type="checkbox">Enable mining</label><div class="field"><label for="poolHost">Pool host</label><input id="poolHost" maxlength="96" spellcheck="false"></div><div class="field"><label for="poolPort">Pool port</label><input id="poolPort" type="number" min="1" max="65535"></div><div class="field full"><label for="miningUsername">Wallet / pool username</label><input id="miningUsername" maxlength="128" autocomplete="off" spellcheck="false"></div><div class="field"><label for="worker">Worker name</label><input id="worker" maxlength="32" spellcheck="false"></div><div class="field"><label for="password">Pool password</label><input id="password" maxlength="64" spellcheck="false"></div></div><div class="actions"><button class="btn primary" type="submit">Save and apply mining settings</button></div></form></section>
<section id="device" class="pane"><div class="heading"><div><h1>Device</h1><div class="sub">Screen controls for this ST7789 CYD.</div></div><span class="badge">ST7789</span></div><form id="deviceForm" class="section"><div class="grid"><div class="field"><label for="brightness">Screen brightness (20-255)</label><input id="brightness" type="number" min="20" max="255"></div><div class="field"><label for="currency">Displayed currency</label><select id="currency"><option>USD</option><option>CAD</option><option>GBP</option></select></div><label class="toggle full"><input id="flipped" type="checkbox">Rotate screen 180 degrees</label></div><div class="actions"><button class="btn primary" type="submit">Save display settings</button></div></form></section><div id="coinPanes"></div></main><script>
const symbols=['CHTA','WJK','DGB','BCH','BTC'];let state=null;const $=s=>document.querySelector(s);const rate=k=>k>=1000?(k/1000).toFixed(2)+' MH/s':Math.round(k||0)+' kH/s';const money=(n,c)=>{n=Number(n);if(!Number.isFinite(n))return'--';return new Intl.NumberFormat(undefined,{style:'currency',currency:c,minimumFractionDigits:2,maximumFractionDigits:n>0&&n<.01?6:2}).format(n)};
function makePanes(){$('#coinPanes').innerHTML=symbols.map(s=>`<section id="${s}" class="pane"><div class="heading"><div><h1 data-name>${s}</h1><div class="sub">Wallet balance and market value</div></div><span class="badge">WALLET</span></div><div class="stats"><div class="stat"><div class="key">Address balance</div><div class="value" data-balance>-- ${s}</div><div class="sub" data-balance-status>WAITING</div></div><div class="stat"><div class="key" data-fiat-label>Value</div><div class="value" data-fiat>--</div></div><div class="stat"><div class="key">Miner hashrate</div><div class="value" data-rate>--</div></div><div class="stat"><div class="key">Accepted / Rejected</div><div class="value" data-shares>--</div></div></div><form class="section wallet-form" data-coin="${s}"><h2>Balance address</h2><div class="field"><label for="wallet-${s}">${s} wallet address</label><input id="wallet-${s}" name="wallet" maxlength="128" autocomplete="off" spellcheck="false"></div><div class="actions"><button class="btn" type="submit">Save wallet</button></div></form></section>`).join('')}
function showPane(name){document.querySelectorAll('.tab,.pane').forEach(x=>x.classList.remove('active'));document.querySelector(`.tab[data-pane="${name}"]`).classList.add('active');document.getElementById(name).classList.add('active')}function notice(text){const n=$('#notice');n.textContent=text;n.style.display='block';clearTimeout(notice.t);notice.t=setTimeout(()=>n.style.display='none',3500)}async function post(url,data){const r=await fetch(url,{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:new URLSearchParams(data)});if(!r.ok)throw Error(await r.text());await refresh()}function setIfClean(el,value){if(el.dataset.dirty!=='1')el.value=value??''}async function refresh(){try{state=await(await fetch('/api/status',{cache:'no-store'})).json();render()}catch(e){$('#connection').textContent='Offline';$('#dot').classList.remove('on')}}
function render(){$('#dot').classList.toggle('on',state.wifi);$('#connection').textContent=state.status;$('#ip').textContent=state.ip;$('#engine').textContent=state.hardwareSha?'HARDWARE SHA':'SOFTWARE';$('#hashrate').textContent=rate(state.hashrateKh);$('#shares').textContent=state.accepted+' / '+state.rejected;$('#best').textContent=Number(state.bestDifficulty||0).toPrecision(4);$('#activePool').textContent=state.pool;const hb=$('#homeBadge');hb.textContent=state.miningEnabled?state.status:'STOPPED';hb.classList.toggle('active',state.miningEnabled);setIfClean($('#poolHost'),state.poolHost);setIfClean($('#poolPort'),state.poolPort);setIfClean($('#miningUsername'),state.username);setIfClean($('#worker'),state.worker);setIfClean($('#password'),state.password);if($('#enabled').dataset.dirty!=='1')$('#enabled').checked=state.miningEnabled;setIfClean($('#brightness'),state.brightness);setIfClean($('#currency'),state.currency);if($('#flipped').dataset.dirty!=='1')$('#flipped').checked=state.flipped;symbols.forEach(s=>{const c=state.coins.find(x=>x.symbol===s),p=document.getElementById(s),value=state.currency==='CAD'?c.valueCad:state.currency==='GBP'?c.valueGbp:c.valueUsd;p.querySelector('[data-name]').textContent=c.name+' ('+s+')';p.querySelector('[data-balance]').textContent=c.balanceAvailable?Number(c.balance).toLocaleString(undefined,{maximumFractionDigits:8})+' '+s:'-- '+s;p.querySelector('[data-balance-status]').textContent=c.balanceStatus||'WAITING';p.querySelector('[data-fiat-label]').textContent=state.currency+' value';p.querySelector('[data-fiat]').textContent=c.balanceAvailable&&c.pricesAvailable?money(value,state.currency):'--';p.querySelector('[data-rate]').textContent=rate(state.hashrateKh);p.querySelector('[data-shares]').textContent=state.accepted+' / '+state.rejected;setIfClean(p.querySelector('input'),c.wallet)})}
$('#tabs').addEventListener('click',e=>{const b=e.target.closest('.tab');if(b)showPane(b.dataset.pane)});$('#openMining').addEventListener('click',()=>showPane('mining'));document.addEventListener('input',e=>{if(e.target.matches('input,select'))e.target.dataset.dirty='1'});document.addEventListener('change',e=>{if(e.target.matches('input[type=checkbox],select'))e.target.dataset.dirty='1'});document.addEventListener('submit',async e=>{e.preventDefault();try{if(e.target.id==='miningForm'){await post('/api/mining-settings',{enabled:$('#enabled').checked?'1':'0',host:$('#poolHost').value,port:$('#poolPort').value,username:$('#miningUsername').value,worker:$('#worker').value,password:$('#password').value});notice('Mining settings saved and applied')}else if(e.target.id==='deviceForm'){await post('/api/device',{brightness:$('#brightness').value,currency:$('#currency').value,flipped:$('#flipped').checked?'1':'0'});notice('Display settings saved')}else if(e.target.classList.contains('wallet-form')){await post('/api/wallet',{coin:e.target.dataset.coin,wallet:e.target.wallet.value});notice(e.target.dataset.coin+' wallet saved')}e.target.querySelectorAll('input,select').forEach(x=>delete x.dataset.dirty)}catch(err){notice(err.message)}});$('#stopMining').addEventListener('click',async()=>{try{await post('/api/mining',{enabled:'0'});notice('Mining stopped')}catch(err){notice(err.message)}});makePanes();refresh();setInterval(refresh,2000);
</script></body></html>
)HTML";

bool validCredential(const String& value) {
  if (value.length() > 128) return false;
  for (size_t i = 0; i < value.length(); ++i) {
    unsigned char c = static_cast<unsigned char>(value[i]);
    if (c < 33 || c > 126) return false;
  }
  return true;
}

}  // namespace

void HeliosWeb::begin(HeliosSettings* settings, HeliosBalances* balances,
                      HeliosSettingsCallback settingsCallback) {
  settings_ = settings;
  balances_ = balances;
  settingsCallback_ = settingsCallback;
  server_.on("/", HTTP_GET, [this]() { server_.send_P(200, "text/html; charset=utf-8", INDEX_HTML); });
  server_.on("/api/status", HTTP_GET, [this]() { handleStatus(); });
  server_.on("/api/wallet", HTTP_POST, [this]() { handleWallet(); });
  server_.on("/api/mining", HTTP_POST, [this]() { handleMining(); });
  server_.on("/api/mining-settings", HTTP_POST, [this]() { handleMiningSettings(); });
  server_.on("/api/device", HTTP_POST, [this]() { handleDevice(); });
  server_.onNotFound([this]() { sendError(404, "Not found"); });
  server_.begin();
}

void HeliosWeb::loop() { server_.handleClient(); }

void HeliosWeb::handleStatus() {
  const HeliosSettingsData& cfg = settings_->data();
  HeliosMiningStats stats = heliosMinerGetStats();
  JsonDocument document;
  document["wifi"] = WiFi.status() == WL_CONNECTED;
  document["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : String("--");
  document["status"] = stats.status;
  document["miningEnabled"] = cfg.miningEnabled;
  document["poolHost"] = cfg.miningPoolHost;
  document["poolPort"] = cfg.miningPoolPort;
  document["pool"] = cfg.miningPoolHost + ":" + cfg.miningPoolPort;
  document["username"] = cfg.miningUsername;
  document["hashrateKh"] = stats.hashrateKh;
  document["totalHashes"] = stats.totalHashes;
  document["accepted"] = stats.acceptedShares;
  document["rejected"] = stats.rejectedShares;
  document["submitted"] = stats.submittedShares;
  document["blocks"] = stats.blocksFound;
  document["bestDifficulty"] = stats.bestDifficulty;
  document["poolDifficulty"] = stats.poolDifficulty;
  document["hardwareSha"] = stats.hardwareSha;
  document["worker"] = cfg.worker;
  document["password"] = cfg.stratumPassword;
  document["brightness"] = cfg.brightness;
  document["flipped"] = cfg.flipped;
  document["currency"] = cfg.fiatCurrency == 1 ? "CAD" : cfg.fiatCurrency == 2 ? "GBP" : "USD";
  JsonArray coins = document["coins"].to<JsonArray>();
  for (size_t i = 0; i < HELIOS_COIN_COUNT; ++i) {
    const HeliosCoinProfile& profile = heliosCoinProfileAt(i);
    HeliosBalanceSnapshot balance = balances_->get(profile.id);
    JsonObject coin = coins.add<JsonObject>();
    coin["symbol"] = profile.symbol;
    coin["name"] = profile.name;
    coin["wallet"] = cfg.wallets[i];
    coin["configured"] = !cfg.wallets[i].isEmpty();
    coin["balanceAvailable"] = balance.available;
    coin["balance"] = balance.balance;
    coin["balanceStatus"] = balance.status;
    coin["pricesAvailable"] = balance.pricesAvailable;
    coin["priceUsd"] = balance.priceUsd;
    coin["priceCad"] = balance.priceCad;
    coin["priceGbp"] = balance.priceGbp;
    coin["valueUsd"] = balance.balance * balance.priceUsd;
    coin["valueCad"] = balance.balance * balance.priceCad;
    coin["valueGbp"] = balance.balance * balance.priceGbp;
  }
  String body;
  serializeJson(document, body);
  server_.sendHeader("Cache-Control", "no-store");
  server_.send(200, "application/json", body);
}

void HeliosWeb::handleWallet() {
  HeliosCoin coin;
  if (!server_.hasArg("coin") || !heliosCoinFromSymbol(server_.arg("coin"), coin)) { sendError(400, "Unknown coin"); return; }
  String wallet = server_.arg("wallet");
  wallet.trim();
  if (!wallet.isEmpty() && !validCredential(wallet)) { sendError(400, "Wallet contains invalid characters or is too long"); return; }
  settings_->setWallet(coin, wallet);
  balances_->requestRefresh(coin);
  sendOk();
}

void HeliosWeb::handleMining() {
  bool enabled = server_.arg("enabled") == "1";
  if (enabled && settings_->data().miningUsername.isEmpty()) { sendError(400, "Enter a wallet or pool username first"); return; }
  settings_->setMiningEnabled(enabled);
  if (settingsCallback_) settingsCallback_();
  sendOk();
}

void HeliosWeb::handleMiningSettings() {
  bool enabled = server_.arg("enabled") == "1";
  String host = server_.arg("host");
  String username = server_.arg("username");
  host.trim();
  username.trim();
  long portValue = server_.arg("port").toInt();
  if (host.isEmpty() || host.length() > 96 || host.indexOf(' ') >= 0) { sendError(400, "Enter a valid pool host"); return; }
  if (portValue < 1 || portValue > 65535) { sendError(400, "Pool port must be from 1 to 65535"); return; }
  if (!username.isEmpty() && !validCredential(username)) { sendError(400, "Pool username contains invalid characters or is too long"); return; }
  if (enabled && username.isEmpty()) { sendError(400, "Enter a wallet or pool username before enabling mining"); return; }
  settings_->setMining(host, static_cast<uint16_t>(portValue), username,
                       server_.arg("worker"), server_.arg("password"), enabled);
  if (settingsCallback_) settingsCallback_();
  sendOk();
}

void HeliosWeb::handleDevice() {
  if (server_.hasArg("brightness")) settings_->setBrightness(static_cast<uint8_t>(constrain(server_.arg("brightness").toInt(), 20, 255)));
  if (server_.hasArg("flipped")) settings_->setFlipped(server_.arg("flipped") == "1");
  if (server_.hasArg("currency")) {
    String currency = server_.arg("currency");
    settings_->setFiatCurrency(currency == "CAD" ? 1 : currency == "GBP" ? 2 : 0);
  }
  if (settingsCallback_) settingsCallback_();
  sendOk();
}

void HeliosWeb::sendOk() { server_.send(200, "application/json", "{\"ok\":true}"); }
void HeliosWeb::sendError(int status, const String& message) { server_.send(status, "text/plain; charset=utf-8", message); }
