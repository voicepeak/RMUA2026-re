#!/bin/bash
grep -RIn -e '-Xmx' -e 'MAXMEM' -e 'maxHeap' -e 'heapSize' /opt/node22/lib/node_modules/rea-agents/dist 2>/dev/null | head -50
