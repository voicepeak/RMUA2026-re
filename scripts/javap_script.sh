#!/bin/bash
JAR=/opt/ghidra_12.1.4_PUBLIC/Ghidra/Features/Base/lib/Base.jar
javap -classpath "$JAR" ghidra.app.script.GhidraScript > /root/ghidra_script_api.txt 2>&1
grep -i -E 'analysis|option' /root/ghidra_script_api.txt | head -60
