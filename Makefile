CFLAGS=-std=c11 -g -static

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc funcdata *.o

.PHONY: test clean

# 分割コンパイルとリンク / 分割コンパイルとは / グローバル変数の宣言と定義
funcdata:
	cc -static -Wl,--omagic -o funcdata funcdata.c
