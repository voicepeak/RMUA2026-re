#!/bin/bash
pgrep -af rea_manager.mjs || true
tail -30 /root/rea_manager.log
