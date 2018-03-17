#!/bin/sh
lex tr.l
gcc lex.yy.c
./a.out
