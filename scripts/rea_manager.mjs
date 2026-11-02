import { spawn } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import readline from 'node:readline';

const base = '/mnt/d/RMUA2026-re/mcp';
const queueDir = path.join(base, 'queue');
const resultsDir = path.join(base, 'results');
fs.mkdirSync(queueDir, { recursive: true });
fs.mkdirSync(resultsDir, { recursive: true });

const child = spawn('rea', ['--mcp'], {
  env: process.env,
  stdio: ['pipe', 'pipe', 'pipe'],
});

const pending = new Map();
let nextId = 1;
const log = (...a) => console.log(new Date().toISOString(), ...a);

const rl = readline.createInterface({ input: child.stdout });
rl.on('line', (line) => {
  let msg;
  try {
    msg = JSON.parse(line);
  } catch {
    return;
  }
  if (msg.id !== undefined && pending.has(msg.id)) {
    const { resolve, timer } = pending.get(msg.id);
    pending.delete(msg.id);
    if (timer) clearTimeout(timer);
    resolve(msg);
  }
});

child.stderr.on('data', (d) => {
  fs.appendFileSync('/root/rea_mcp_stderr.log', d);
});
child.on('exit', (code) => {
  log('MCP server exited', code);
  process.exit(1);
});

function rpc(method, params, timeoutMs = 0) {
  const id = nextId++;
  const msg = { jsonrpc: '2.0', id, method, params };
  return new Promise((resolve, reject) => {
    let timer = null;
    if (timeoutMs > 0) {
      timer = setTimeout(() => {
        pending.delete(id);
        reject(new Error(`timeout after ${timeoutMs}ms`));
      }, timeoutMs);
    }
    pending.set(id, { resolve, timer });
    child.stdin.write(JSON.stringify(msg) + '\n');
  });
}

function sendNotification(method, params) {
  child.stdin.write(JSON.stringify({ jsonrpc: '2.0', method, params }) + '\n');
}

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

await rpc('initialize', {
  protocolVersion: '2024-11-05',
  capabilities: {},
  clientInfo: { name: 'rea-manager', version: '0.1' },
}, 60000);
sendNotification('notifications/initialized');
log('MCP initialized');

const tools = await rpc('tools/list', {}, 60000);
fs.writeFileSync(path.join(base, 'tools.json'), JSON.stringify(tools, null, 2));
log('tools listed');

while (true) {
  let files = [];
  try {
    files = fs.readdirSync(queueDir).filter((f) => f.endsWith('.json')).sort();
  } catch {
    files = [];
  }
  if (files.length === 0) {
    await sleep(500);
    continue;
  }
  const name = files[0];
  const reqPath = path.join(queueDir, name);
  let req;
  try {
    req = JSON.parse(fs.readFileSync(reqPath, 'utf8'));
  } catch {
    await sleep(500);
    continue;
  }
  fs.unlinkSync(reqPath);
  const id = req.id || name.replace(/\.json$/, '');
  const tool = req.tool;
  const args = req.args || {};
  const started = Date.now();
  let response;
  try {
    response = await rpc('tools/call', { name: tool, arguments: args }, 0);
  } catch (e) {
    response = { error: String(e) };
  }
  const elapsed = Date.now() - started;
  fs.writeFileSync(
    path.join(resultsDir, id + '.json'),
    JSON.stringify({ request: req, elapsed_ms: elapsed, response }, null, 2),
  );
  log(id, tool, elapsed + 'ms');
}
