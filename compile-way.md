# Basic compile way

参考：

https://ezoeryou.github.io/cpp-intro/#gcc-c%E3%82%B3%E3%83%B3%E3%83%91%E3%82%A4%E3%83%A9%E3%83%BC

## GCC

```bash
# GCCの基本的な使い方は以下のとおり。
g++ その他のオプション -o 出力するファイル名 ソースファイル名
# ソースファイル名は複数指定することができる。
$ g++ -o abc a.cpp b.cpp c.cpp

# -std=はC++の規格を選択するオプションだ。C++17に準拠したいのであれば-std=c++17を指定する。

# -Wallはコンパイラーの便利な警告メッセージのほとんどすべてを有効にするオプションだ。コンパイラーによる警告メッセージはプログラムの不具合を未然に発見できるので、このオプションは指定すべきだ。

# --pedantic-errorsはC++の規格を厳格に守るオプションだ。規格に違反しているコードがコンパイルエラー扱いになる。
$ g++ -std=c++17 -Wall --pedantic-errors -o 出力ファイル名 入力ファイル名
```

#### ヘッダファイルの省略

`./basic/all.h`を用意して、次のコマンドを打つようにするならばcppファイルにincludeが必要なくなる

```bash
$ g++ -include all.h -o program main.cpp
```
```CPP
int main()
{
    std::cout << "hello" ;
}
```

#### precompiled header

プログラムで変更しないファイルを事前にコンパイルしておくと、変更した部分だけコンパイルすればいいのでコンパイル時間の短縮になる。

all.hを事前コンパイル済ヘッダにしてみる。

```bash
$ g++ -std=c++17 -Wall --pedantic-errors -x c++-header -o all.h.gch all.h
```

ここまで、我々はソースファイルをコンパイルして実行可能ファイルを生成し、プログラムを実行する方法について学んできた。これまでに学んできたことを一連のコマンドで振り返ってみよう。
```bash
$ ls
all.h main.cpp
$ cat all.h
#include <iostream>
$ cat main.cpp
int main() { std::cout << "hello"s ; }
```

まず、カレントディレクトリーにはall.hとmain.cppがある。この2つのファイルは実行可能ファイルを生成するために必要なファイルだ。今回、その中身は最小限にしてある。本当のall.hは、実際には前回書いたように長い内容になる。

```bash
$ g++ -std=c++17 -Wall --pedantic-errors -x c++-header -o all.h.gch all.h
$ ls
all.h all.h.gch main.cpp
```

次に、ソースファイルのコンパイルを高速化するために、ヘッダーファイルall.hから、コンパイル済みヘッダーファイルall.h.gchを生成する。

```bash
$ g++ -std=c++17 -Wall --pedantic-errors -include all.h -o program main.cpp
$ ls
all.h all.h.gch main.cpp program
```

プリコンパイル済みヘッダーファイルall.h.gchとC++ソースファイルmain.cppから、実行可能ファイルprogramを生成する。

```bash
$ ./program
hello
```

実行可能ファイルprogramを実行する。

これで読者はC++のプログラミングを学び始めるにあたって必要なことはすべて学んだ。さっそくC++を学んでいきたいところだが、その前にもう1つ、ビルドシステムを学ぶ必要がある。

## Make 

GNU Make

#### 依存関係の解決

とにかくMakefileに依存関係を記すと

```bash 
make
```

とするだけで依存関係を解決してコンパイルしてくれる

## 入門用の環境構築

今回構築する環境のファイル名とその意味は以下のとおり。

main.cpp
C++のコードを書く all.h
標準ライブラリのヘッダーファイルを書く all.h.gch
コンパイル済みヘッダー program
実行可能ファイル Makefile
GNU Makeのルールを書く
使い方は以下のとおり。

make
コンパイルする make run
コンパイルして実行 make clean
コンパイル結果を削除

```bash
# GCCに与えるコンパイラーオプションを変数にまとめる。

gcc_options = -std=c++17 -Wall --pedantic-error
# 言語はC++17、すべての警告を有効にし、規格準拠ではないコードはエラーとする。

# プログラムをコンパイルする部分は以下のとおり。

program : main.cpp all.h all.h.gch
    g++ $(gcc_options) -include all.h $< -o $@

all.h.gch : all.h
    g++ $(gcc_options) -x c++-header -o $@ $<
# 実行可能ファイルprogramと、コンパイル済みヘッダーall.h.gchをコンパイルするルールだ。

# PHONYターゲットは以下のとおり。

run : program
    ./program

clean :
    rm -f ./program
    rm -f ./all.h.gch

.PHONY : run clean
# makeでコンパイル。make runで実行。make cleanでコンパイル結果の削除。
```
Makefile全体は以下のようになる。

```
gcc_options = -std=c++17 -Wall --pedantic-errors

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

- `$@`: ターゲット

`$@`はルールのターゲットを指す。

- `$<`: ルールの最初の事前要件のファイル名

たとえば、以下ならばAになる。

```
target: A B C
    echo $<
```

というMakefileをmakeすると

```
$make
echo A
```

と出力される

- `$^`すべての事前要件

`$<`が最初の事前要件ならば`$^`は事前要件全てを指す。

先の例ならABCすべてである。

NOTE: Makefileで行の先頭にスペースを入れる場合は、そこは「タブ」で空けること

でないと

*** missing separator. Stop.

というエラーが出る

