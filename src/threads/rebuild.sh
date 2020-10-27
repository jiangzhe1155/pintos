#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run priority-donate-sema)
sleep 3