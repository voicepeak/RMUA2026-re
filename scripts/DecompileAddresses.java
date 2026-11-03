import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.file.Files;
import java.nio.file.Path;

/** Decompile selected addresses without claiming full-program analysis. */
public class DecompileAddresses extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) throw new IllegalArgumentException("output-file address...");
        DecompInterface dec = new DecompInterface();
        dec.openProgram(currentProgram);
        StringBuilder out = new StringBuilder();
        for (int i = 1; i < args.length; i++) {
            Address addr = toAddr(Long.decode(args[i]));
            out.append("// ADDRESS ").append(addr).append('\n');
            Function f = getFunctionAt(addr);
            if (f == null) {
                if (getInstructionAt(addr) == null) disassemble(addr);
                f = createFunction(addr, "selected_" + addr);
            }
            if (f == null) { out.append("// FUNCTION CREATION FAILED\n\n"); continue; }
            DecompileResults result = dec.decompileFunction(f, 120, monitor);
            if (result != null && result.decompileCompleted()) out.append(result.getDecompiledFunction().getC());
            else out.append("// DECOMPILE FAILED: ").append(result == null ? "null" : result.getErrorMessage());
            out.append("\n\n");
        }
        dec.dispose();
        Files.writeString(Path.of(args[0]), out.toString());
    }
}
