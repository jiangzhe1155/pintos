#!/usr/bin/env bash

echo $(make clean)
echo $(make)
echo $(pintos --gdb -- run)

sleep 3