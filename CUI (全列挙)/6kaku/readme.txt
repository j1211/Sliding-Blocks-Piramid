[これは何？]

六角形をピラミッド型に並べたスライドパズルの全列挙プログラムです。計算量はピース数をnとして、O(n!)

こんな感じのを
     4
    3 1
   0 7 6
  9 5 2 8

こうじゃ
    0
   1 2
  3 4 5
 6 7 8 9

各番号が正六角形と対応していて、0が空白マスです。
スライドは、ブロックの移動ではなくて、数字の移動（空白マスとの交換）と考えます。


大きさN（正の整数）を入力すると、全列挙します。 (ピース数は1+2+…+N)
計算量が大きいので、N ≧ 4は非推奨です。（いつか作りたい）


[実行方法]
競技プログラミングのソースコードと同様の実行方法で大丈夫です。

例えばWindows / Visual Studio 2017であれば、コマンドプロンプトなどで、
１．cl /O2 solve1.cpp  (clはVisual Studioのビルドコマンド）
２．solve1.exe
とすればできます。


g++コンパイラであれば
g++ solve1.cpp
./a.out
でできると思いますが、未検証です。


-------------------入出力---------------------------
[入力]
N

Nは整数
1 ≦ N ≦ 4


[出力]
解析結果を出力します。


[入力例1]
3

[出力例1]
完成可能な個数 = 3628800
最大手数 = 34
[0, 8, 7, 5, 4, 3, 9, 2, 1, 6], cost = 34
最短0[回]で完成するパターンの数 = 1
最短1[回]で完成するパターンの数 = 2
最短2[回]で完成するパターンの数 = 6
最短3[回]で完成するパターンの数 = 21
最短4[回]で完成するパターンの数 = 60
最短5[回]で完成するパターンの数 = 142
最短6[回]で完成するパターンの数 = 323
最短7[回]で完成するパターンの数 = 778
最短8[回]で完成するパターンの数 = 1765
最短9[回]で完成するパターンの数 = 3631
最短10[回]で完成するパターンの数 = 6877
最短11[回]で完成するパターンの数 = 12309
最短12[回]で完成するパターンの数 = 21518
最短13[回]で完成するパターンの数 = 36289
最短14[回]で完成するパターンの数 = 57399
最短15[回]で完成するパターンの数 = 85829
最短16[回]で完成するパターンの数 = 123039
最短17[回]で完成するパターンの数 = 168391
最短18[回]で完成するパターンの数 = 219395
最短19[回]で完成するパターンの数 = 272543
最短20[回]で完成するパターンの数 = 322737
最短21[回]で完成するパターンの数 = 361937
最短22[回]で完成するパターンの数 = 381420
最短23[回]で完成するパターンの数 = 376817
最短24[回]で完成するパターンの数 = 348416
最短25[回]で完成するパターンの数 = 297177
最短26[回]で完成するパターンの数 = 229109
最短27[回]で完成するパターンの数 = 155771
最短28[回]で完成するパターンの数 = 89283
最短29[回]で完成するパターンの数 = 40490
最短30[回]で完成するパターンの数 = 12858
最短31[回]で完成するパターンの数 = 2269
最短32[回]で完成するパターンの数 = 193
最短33[回]で完成するパターンの数 = 4
最短34[回]で完成するパターンの数 = 1


例えば、[0, 8, 7, 5, 4, 3, 9, 2, 1, 6], cost = 34は
     0
    8 7
   5 4 3
  9 2 1 6
のようなパターンで、最短34手。
