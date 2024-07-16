#!/bin/bash
gcc -o libgv.so -shared -fPIC main.c -I/usr/include/graphviz -I/usr/include/lua5.3 -L/usr/lib -lgvc -lcgraph -llua5.3
