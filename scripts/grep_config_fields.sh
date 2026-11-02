#!/bin/bash
for off in 2f8 2fc 300 304 308 310 318; do
  echo "==== +0x$off"
  grep -n -E "\+0x${off}\]" /root/main.text.disasm | head -30
done
