#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run priority-condvar)
sleep 3