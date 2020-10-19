#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run alarm-priority)
sleep 3