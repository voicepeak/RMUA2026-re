import json
import sys
from pathlib import Path

path = Path(sys.argv[1])
d = json.loads(path.read_text())
ex = d['Exports']
im = d.get('Imports', [])

def obj_name(v):
    if not isinstance(v, int):
        return v
    if v > 0:
        return ex[v-1].get('ObjectName') if v-1 < len(ex) else v
    if v < 0:
        return im[-v-1].get('ObjectName') if -v-1 < len(im) else v
    return None

def field_path(x):
    try:
        return '/'.join(x['Variable']['New']['Path'])
    except Exception:
        return str(x)

def short(x, depth=0):
    if not isinstance(x, dict) or depth > 8:
        return str(x)
    t = x.get('$type', '').split('.')[-1].split(',')[0]
    if t == 'EX_StringConst' or t == 'EX_UnicodeStringConst':
        return repr(x.get('Value'))
    if t in ('EX_IntConst', 'EX_Int64Const', 'EX_ByteConst'):
        return str(x.get('Value'))
    if t == 'EX_FloatConst' or t == 'EX_DoubleConst':
        return str(x.get('Value'))
    if t == 'EX_NameConst':
        return 'name:' + str(x.get('Value'))
    if t in ('EX_VirtualFunction', 'EX_FinalFunction', 'EX_LocalFinalFunction'):
        return (x.get('VirtualFunctionName') or obj_name(x.get('StackNode'))) + '(' + ', '.join(short(p, depth+1) for p in (x.get('Parameters') or [])) + ')'
    if t == 'EX_CallMath':
        return 'callmath:' + str(obj_name(x.get('StackNode'))) + '(' + ', '.join(short(p, depth+1) for p in (x.get('Parameters') or [])) + ')'
    if t == 'EX_LocalVariable' or t == 'EX_InstanceVariable':
        return 'var:' + field_path(x)
    if t == 'EX_ObjectConst':
        return 'obj:' + str(obj_name(x.get('Value')))
    if t == 'EX_Self':
        return 'self'
    if t == 'EX_True':
        return 'true'
    if t == 'EX_False':
        return 'false'
    if t == 'EX_NoObject':
        return 'none'
    if t == 'EX_Nothing':
        return 'nothing'
    if t == 'EX_Context':
        return 'ctx[' + short(x.get('ContextExpression'), depth+1) + '].' + field_path(x.get('RValuePointer', {})) if x.get('RValuePointer') else 'ctx(' + short(x.get('ContextExpression'), depth+1) + ')'
    if t in ('EX_LetObj', 'EX_LetBool', 'EX_Let', 'EX_LetValueOnPersistentFrame'):
        return short(x.get('Variable'), depth+1) + ' = ' + short(x.get('Expression'), depth+1)
    if t == 'EX_Jump':
        return 'jump ' + str(x.get('CodeOffset'))
    if t == 'EX_JumpIfNot':
        return 'ifnot ' + short(x.get('BooleanExpression'), depth+1) + ' jump ' + str(x.get('CodeOffset'))
    if t == 'EX_Return':
        return 'return ' + short(x.get('ReturnExpression'), depth+1)
    if t == 'EX_EndOfScript':
        return 'end'
    # default: show immediate simple fields
    return t + '(' + ','.join(f'{k}={v}' for k,v in x.items() if k not in ('$type',) and isinstance(v,(str,int,float,bool)) ) + ')'

lines = []
for e in ex:
    bc = e.get('ScriptBytecode') or []
    if bc:
        lines.append('==== %s size %s' % (e.get('ObjectName'), e.get('ScriptBytecodeSize')))
        for i, ins in enumerate(bc):
            lines.append('  %3d: %s' % (i, short(ins)))
        lines.append('')
Path(sys.argv[2]).write_text('\n'.join(lines), encoding='utf-8')
print('wrote', sys.argv[2], len(lines), 'lines')
