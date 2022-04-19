#!/bin/bash
# Wake up the monitor (if not plugged in at boot or asleep)

export DISPLAY=:0
xrandr --auto
