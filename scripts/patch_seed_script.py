from pathlib import Path
p = Path('/opt/node22/lib/node_modules/rea-agents/bridge/ghidra/ReaGhidraApplySeeds.java')
backup = Path('/mnt/d/RMUA2026-re/tools/ReaGhidraApplySeeds.java.orig')
if not backup.exists():
    backup.write_bytes(p.read_bytes())
text = p.read_text()
needle = '        String[] args = getScriptArgs();\n'
insert = '''        try {
            setAnalysisOption(currentProgram, "Disassemble Entry Points", "false");
            println("REA test: disabled Disassemble Entry Points");
        } catch (Exception e) {
            println("REA test: disable failed " + e);
        }
'''
if 'REA test: disabled' not in text:
    text = text.replace(needle, insert + needle, 1)
    p.write_text(text)
print('patched', p)
