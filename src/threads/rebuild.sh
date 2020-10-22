#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run priority-donate-one)
sleep 3