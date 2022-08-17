/*************************************************************
    grep command 

    accept options:

    -i: Not seperate uppercase and lowercase.
    -v: Search umatched.
***************************************************************/ 
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *f);

int main(int argc, char *argv[]){
    regex_t pat;
    int err;
    int i;

    if(argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
    if(err != 0) {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if(argc == 2) {
        do_grep(&pat, stdin);
    }
    else {
        for(i = 2; i < argc; i++) {
            FILE *f;
            f = fopen(argv[1], "r");
            if(!f) {
                perror(argv[1]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src) {
    char buf[4096];

    // fgets()は1行読み込んでbufへ格納する
    while(fgets(buf, sizeof buf, src)) {
        // 正規表現とパターンが一致するか検査する
        if(regexec(pat, buf, 0, NULL, 0) == 0) {
            // 一致したらbufを出力する
            fputs(buf, stdout);
        }
    }
}
