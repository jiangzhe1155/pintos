#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run alarm-zero )
sleep 3