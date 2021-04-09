CFLAGS=-std=c11 -g -static

# 分割コンパイルとリンク / 分割コンパイルとは / グローバル変数の宣言と定義
main_var:
	cc -static -Wl,--omagic -o main_var main_var.c

clean:
	rm -f main_var
