import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import java.nio.file.Files;
import java.nio.file.Path;

public final class DumpAnchors extends GhidraScript {
    private static final long[] ADDRS = {
        0x2fa0e80L,
        0x2fa24a0L,
        0x2fa3190L,
        0x2fa3dd0L,
        0x2fb3a50L,
        0x2fb4e80L,
        0x2fb7110L,
        0x2fb7f90L,
        0x2fb8650L,
        0x2fb8f40L,
        0x2fc0020L,
        0x2fcc5a0L,
        0x2fd29c0L,
        0x2fba830L,
        0x2fba970L,
        0x2fb9960L,
        0x2fb9830L,
        0x2fb2050L,
        0x2fb2130L,
        0x2fb2040L,
        0x612efc0L,
        0x612daf0L
    };

    @Override
    public void run() throws Exception {
        DecompInterface dec = new DecompInterface();
        dec.openProgram(currentProgram);
        AddressSpace space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        StringBuilder out = new StringBuilder();
        for (long value : ADDRS) {
            Address addr = space.getAddress(value);
            out.append("// ==== ").append(Long.toHexString(value)).append(" ====\n");
            Function f = getFunctionAt(addr);
            if (f == null) {
                if (getInstructionAt(addr) == null) {
                    disassemble(addr);
                }
                f = createFunction(addr, "REA_" + Long.toHexString(value));
            }
            if (f == null) {
                out.append("// createFunction failed\n\n");
                continue;
            }
            DecompileResults result = dec.decompileFunction(f, 180, monitor);
            if (result != null && result.decompileCompleted()) {
                out.append(result.getDecompiledFunction().getC());
            } else {
                out.append("// decompile failed: ")
                   .append(result == null ? "null" : result.getErrorMessage())
                   .append("\n");
            }
            out.append("\n\n");
        }
        dec.dispose();
        Files.writeString(Path.of("/root/anchors_pseudo.c"), out.toString());
        println("wrote /root/anchors_pseudo.c");
    }
}
