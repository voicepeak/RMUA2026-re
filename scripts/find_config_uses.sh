#!/bin/bash
for off in 0 1 2 3 4 5 6; do
  echo "==== 0x2d$off"
  grep -E "^ (2f|30|31|32|33)[0-9a-f]+:.*0x2d$off\]" /root/main.text.disasm | head -40
done
