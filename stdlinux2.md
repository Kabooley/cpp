# ふつうのLinuxプログラミング第2版

## 5: ストリーム

#### ストリームの簡単な説明

ストリームはLinuxシステムを構成する3つの重要な概念の一つである。

ここでいうストリームはバイトストリームのことで、

つまり一方からバイト列を読み取って他方へ読み取ったバイト列を書き込むストリームのことである。

ストリームは何にでも繋げることができる。

たとえば、

ファイル、プロセス、端末、ネットワーク（先のプロセス）などである。

`$cat text.txt > out.txt`としたら、

text.txtとcatコマンドプロセスをストリームでつないでtext.txtのバイト列を読み取ってcatプロセスへ渡している。

catプロセスは処理結果を出力するがその出力先がout.txtで、

catプロセスとout.txtはストリームでつながれcatプロセスからの出力をout.txtへストリームを経由して書き込んでいるのである。

このような一例からわかる通り、

あらゆる読み取り、書き込みはストリームがないと実現できない。

(すくなくともLinuxシステムでは)

#### ファイル・ディスクリプタ

ストリームを使うにはファイルディスクリプタという、カーネルによってストリームに割り当てられた番号を、使う側が知ることが必須となる。

以下、`fd`と呼ぶ。

書籍によると、プロセスはストリームを様々な事情から「直接使うことはできない」。

代わりにfdをプロセスへ教えることでその番号によってストリームを呼び出すことを許可している。

fdは一意な番号である。

#### ストリームにかかわるシステム・コール

open()
read()
write()
close()

open()は戻り値としてfdを返す。

開きたいファイルのpathや書き込み専用で開くのか読み取り専用で開くのかなどを指定する。

close()は引数としてfdを要求する。

開いたfd(ストリーム）は必ず閉じなくてはならない。open()したらclose()は必須である。

#### catコマンドを作る

##### errno変数

ストリームにかかわるシステムコールにおいて一貫したルール

成功なら0以上の整数を返す
失敗なら-1を返す

また失敗した原因を示す定数があって、システムコールが失敗するとこの定数がグローバル変数errnoへセットされる。

実践:

Makefileの`run`に読み取るファイルのpathを追加する

```
run : program
	./program all.h
```

```bash
$ make run
```

結果all.hがcatコマンドで呼びされたのと同じ結果になった

#### stdio

システムコールは遅い。

ストリーム関係のシステムコールをちまちま使うとハチャメチャ処理が遅くなるらしい。

stdioライブラリの関数をしすてむこーるの代わりに使うと遅い問題が解決される。

stdioはシステムコールの問題をカバーして利便性を向上させたライブラリである。

stdioではバッファリングを使っているからシステムコールより早い。

stdioでは独自のバッファを用意して、

システムコールで予めある程度のデータ量を読み取ってバッファに保存しておいて、

stdioの呼出に応じてバッファからデータを提供するため

直接システムコールを呼び出してfdからデータを取得するよりもバッファからデータを読み取るから早い。

システムコールではfdを使っていたが、stdioでは代わりにFILE型というtypedefを代わりに用いる。

FILEにはfd情報のほかにstdioが使っている内部バッファ情報が入っている。

stdioのストリームメソッドはfdの代わりにFILEを使うことで読み書きができるようになる。

##### stdio版catコマンドを作る

作ってみてわかるのは、バッファを自分で用意する必要がなくなったこと。

#### headコマンドを作る

```bash
# Linuxコマンドとしてのhead
# 最初の5行を表示する
$ cat all.h | head 5

# 今回作るプログラムを使う場合
$ cat all.h | ./head 5 
```

ということで、

ストリームでcatから書込みが行われて、オプションも引き取る

なので、

catからのfdを取得して、コマンドラインからのオプションを読み取る

ポイント：stdin

```cpp
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
```

実行コマンドは`cat all.h | ./head 5`なので、

コマンド上ではcatの出力先が./headとなる

その出力ストリームをstdinとしてプログラムは受け入れる。

ファイルも扱えるheadコマンド

つまり、

`$ ./head NUMBER [FILENAME]`という具合。

## gdbを使ったデバグ

g++やgccでコンパイルするときに`-g`オプションをつける。

そうしてコンパイルしたのち、

`gdb ./program`というようにgdbコマンドでプログラムを実行する。

ということでMakefileを編集する。

```makefile
# -gを追加した
gcc_options = -std=c++17 -Wall --pedantic-errors -g

program : main.cpp all.h all.h.gch
	g++ $(gcc_options) -include all.h $< -o $@

all.h.gch : all.h
	g++ $(gcc_options) -x c++-header -o $@ $<

run : program
	./program

clean :
	rm -f ./program
	rm -f ./all.h.gch

.PHONY : run clean
```
gdbをinstallする

```bash
$ sudo apt install gdb -y
```

まぁテキスト見てもらった方が速いですわ。


## grepコマンドを作る

grepコマンドの基本的な使い方

```bash
# all.hのファイルの中で`stdio.h`という文字列を含む行をすべて表示する
$ grep all.h stdio.h
#include <stdio.h>

# 2つ以上の検索キーワードのとき
$ grep all.h sys | grep file
#include <filesystem>
```


## 11: プロセスとは何か

素のコンピュータ、つまり**CPUとメモリ一つずつではプロセスは一つしか動かせない。**

しかし通常コンピュータを動かすには何十、何百のプロセスが動いている。

なぜか。

それはコンピュータがプロセスに対してメモリとCPUがたくさんあるように見せかけているからである。

現代的なコンピュータではカーネルとハードウェアの力でこれを実現してい。

この見せかけのメモリとCPUを「仮想CPU」「仮想メモリ」とでもよんでおく。

#### 仮想メモリの仕組み

プログラムが動くにはメモリに展開されなくてはならない。

展開されるメモリの場所はカーネルとCPUによって割り当てるが、

この時物理的なメモリ番地をプログラムに割り当てるのではなくて、

仮想上のアドレスをプログラムに割り当てる。

仮想上のメモリアドレスは完全に物理メモリアドレスに割り当てる必要がないので、

仮想上のメモリアドレスは実際にコンピュータが持つ物理メモリ容量よりも多く見せかけることができるのである。

だから実際のメモリよりもおおく見せかけることができるので、

コンピュータは多くのプロセスを同時に展開することができるのである。

プロセスがメモリにアクセスする必要が出来たときはカーネルとCPUがその都度仮想的なメモリアドレスを実際のメモリアドレスに翻訳してメモリにアクセスできるように仕掛けている。

プロセスから見えるアドレスは論理アドレス、実際のアドレスを物理アドレスと区別される。

プロセスに用意されたメモリ全体のことをアドレス空間という。

アドレス空間はページという単位で区切られている。

論理アドレスを物理アドレスに割り当てるときもページ単位で対応付けられる。

64bitアーキテクチャというコンピュータの設計方式がある。

IntelやAMDなどのCPUを作る企業が作り出したコンピュータ設計方式で、

これは何かというと、

64bitのunsigned intのポインタが表現できる約172億GBのアドレス空間のことである。

つまり64bitアーキテクチャというのは見せかけのメモリアドレスがどのくらいの大きさになるのかの指標になるのである。

#### ページング

物理メモリが不足したらカーネルは物理メモリの拡張としてSSDやHDDを物理メモリとして使いだす。

カーネルはあまり使われていないページを適当に探してストレージに記録して、論理アドレスとの接続を解除する。

プロセスがそのページにアクセスしたときにカーネルはプロセスを一時停止してその間にストレージからページを読みだして、

論理アドレスに読みだしたページを割り当ててプロセスを再開する。

こうすることで、物理メモリが不足しているときでもプロセスとしては普段通りにメモリ使えているように見せかけることができる。

このようにページ単位でストレージへ対比することをページング、

プロセス全体をストレージへ対比することはスワッピングという。

#### メモリ・マップト・ファイル

ファイルをメモリとしてアクセスできるようにすること。

つまり、

そのメモリを読み出すとファイルを読みだしたことになり、そのメモリに書き込みをするとファイルに書き込んだことになる、

そんな仕組みのこと。
