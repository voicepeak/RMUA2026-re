#!/bin/bash
out=/root/lib.game_symbols.txt
grep -i -E 'AirSimROSGame|Game2024|Game2023|Route|Gate|Status|Ports|Circle|Tree|Wind' /root/lib.symbols.demangled > "$out"
wc -l "$out"
head -300 "$out"
