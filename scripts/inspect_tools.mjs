import fs from 'node:fs';
const data = JSON.parse(fs.readFileSync('D:/RMUA2026-re/mcp/tools.json', 'utf8'));
const tools = data.result.tools;
console.log('count', tools.length);
for (const t of tools) console.log(t.name);
const wanted = process.argv.slice(2);
for (const w of wanted) {
  const t = tools.find((x) => x.name === w);
  console.log('\n=== ' + w + ' ===');
  if (!t) {
    console.log('not found');
    continue;
  }
  console.log(t.description);
  console.log(JSON.stringify(t.inputSchema, null, 2));
}
