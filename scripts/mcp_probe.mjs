import { spawn } from 'node:child_process';
import readline from 'node:readline';

const child = spawn('rea', ['--mcp'], { env: process.env, stdio: ['pipe', 'pipe', 'pipe'] });
const rl = readline.createInterface({ input: child.stdout });
rl.on('line', (line) => {
  console.log('OUT', line.slice(0, 2000));
});
child.stderr.on('data', (d) => console.error('ERR', d.toString().slice(0, 2000)));
child.on('exit', (code) => console.error('EXIT', code));

function send(obj) {
  child.stdin.write(JSON.stringify(obj) + '\n');
}

send({
  jsonrpc: '2.0',
  id: 1,
  method: 'initialize',
  params: {
    protocolVersion: '2024-11-05',
    capabilities: {},
    clientInfo: { name: 'probe', version: '0.1' },
  },
});
setTimeout(() => send({ jsonrpc: '2.0', method: 'notifications/initialized' }), 500);
setTimeout(() => send({ jsonrpc: '2.0', id: 2, method: 'tools/list', params: {} }), 1000);
setTimeout(() => process.exit(0), 8000);
