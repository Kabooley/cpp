/*************************************************************
cat command 

コマンドライン引数からファイル名を一つ受け取り、標準出力へ書き込む。


***************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

static void do_head(FILE *f, long nlines);

int main(int argc, char *argv[]){

    // コマンドライン引数がないとき
    if( argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }
    // 取得先はstdinで取得する
    // オプションはargv[1]から取得する
    do_head(stdin, atol(argv[1]));
    exit(0);
}


static void do_head(FILE *f, long nlines) {
    int c;

    if(nlines <= 0) return;
    while((c = getc(f)) != EOF) {
        if(putchar(c) < 0) exit(1);
        if(c == '\n') {
            nlines--;
            if(nlines == 0) return;
        }
    }
}