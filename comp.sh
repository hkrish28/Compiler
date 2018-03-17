#!/bin/sh
yacc -d exptree.y
lex exptree.l
gcc lex.yy.c y.tab.c
./a.out
