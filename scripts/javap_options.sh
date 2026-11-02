#!/bin/bash
JAR=/opt/ghidra_12.1.4_PUBLIC/Ghidra/Framework/Utility/lib/Utility.jar
javap -classpath "$JAR" ghidra.framework.options.Options > /root/options_api.txt 2>&1
grep -i -E 'getOption|getValue|getType|getName|setValue' /root/options_api.txt | head -40
