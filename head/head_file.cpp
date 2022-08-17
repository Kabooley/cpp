/*************************************************************
cat command 

コマンドライン引数からファイル名を一つ受け取り、標準出力へ書き込む。


***************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

static void do_head(FILE *f, long nlines);

int main(int argc, char *argv[]){
    long nlines;

    // コマンドライン引数がないとき
    if( argc < 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }
    nlines = atol(argv[1]);
    // コマンドラインが2つだった時
    if(argc == 2) {
        do_head(stdin, nlines);
    }
    // それ以上だった時
    else {
        int i;

        for(i = 2; i < argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if(!f) {
                perror(argv[1]);
                exit(1);
            }
            do_head(f, nlines);
            fclose(f);
        }
    }
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