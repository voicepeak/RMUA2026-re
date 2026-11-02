#!/bin/bash
JAR=/opt/ghidra_12.1.4_PUBLIC/Ghidra/Features/Base/lib/Base.jar
javap -classpath "$JAR" ghidra.app.plugin.core.analysis.AutoAnalysisManager > /root/aam_api.txt 2>&1
grep -i -E 'analyzer|enable|disable' /root/aam_api.txt | head -60
