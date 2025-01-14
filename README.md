# stack-chan-tester

日本語 | [English](README_en.md)

本リポジトリは[mongonta0716](https://github.com/mongonta0716/) さんが公開されている[stack-chan-tester](https://github.com/mongonta0716/stack-chan-tester)を[kim-xps12](https://github.com/kim-xps12)がM5Stack Basic V2.7のPortAに接続する専用に編集したものです．基本的な操作方法はオリジナルに準拠しています．

非常に便利なコードを公開していただいていること，この場をお借りして感謝申し上げます．ありがとうございます！！




---
以下原文
---

スタックチャンを作成するときに、PWMサーボの調整及びテストを行うためのアプリケーションです。<br>
stack-chan test application for pwm servo

# 対応キット
 BOOTHで頒布している [ｽﾀｯｸﾁｬﾝ M5GoBottom版 組み立てキット](https://mongonta.booth.pm/)の動作確認及び組立時の設定用に開発しました。ピンの設定を変えることによりスタックチャン基板にも対応可能です。

※ ArduinoFramework及びPWMサーボのみです。

# サーボのピンの設定
CoreS3はPort.C(G18, G17)、Core2はPort.C(G13,G14)、Fireは Port.A(G22,G21)、Core1は Port.C(G16,G17)を使うようになっています。違うピンを使用する場合は下記の箇所を書き換えてください。
https://github.com/mongonta0716/stack-chan-tester/blob/main/src/main.cpp#L7-L35

# サーボのオフセット調整
SG90系のPWMサーボは個体差が多く、90°を指定しても少しずれる場合があります。その場合は下記のオフセット値を調整してください。(90°からの角度（±）を設定します。)
調整する値は、ボタンAの長押しでオフセットを調整するモードに入るので真っ直ぐになる数値を調べてください。

https://github.com/mongonta0716/stack-chan-tester/blob/main/Stackchan_tester/Stackchan_tester.ino#L27-L28

# 使い方
* ボタンA： X軸、Y軸のサーボを90°に回転します。固定前に90°にするときに使用してください。
* ボタンB： X軸は0〜180, Y軸は90〜50まで動きます。
* ボタンC： ランダムで動きます。
    * ボタンC: ランダムモードの停止
* ボタンAの長押し：オフセットを調整して調べるモードに入ります。
    * ボタンA: オフセットを減らす
    * ボタンB: X軸とY軸の切り替え
    * ボタンC: オフセットを増やす
    * ボタンB長押し: 調整モードの終了

## CoreS3のボタン操作
CoreS3はCore2のBtnA、B、Cの部分がカメラやマイクに変わってしまったためボタンの扱いが変わりました。<br>
画面を縦に3分割して左：BtnA、中央：BtnB、右：BtnCとなっています。

# 必要なライブラリ（動作確認済バージョン）
※ 最新の情報はplatformio.iniを確認してください。最新で動かない場合はライブラリのバージョンを合わせてみてください。
- [M5Unified](https://github.com/m5stack/M5Unified) v0.1.6で確認
- [M5Stack-Avatar](https://github.com/meganetaaan/m5stack-avatar) v0.8.2で確認<br>v0.7.4以前はM5Unifiedに対応していないのでビルド時にM5の二重定義エラーが出ます。
- [ServoEasing](https://github.com/ArminJo/ServoEasing) v3.1.0で確認
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) v0.13.0で確認<br>CoreS3はv0.13.0じゃないと動かない場合があります。

ArduinoIDEでの詳しいライブラリの追加方法は下記のブログを参照してください。（日本語）

[ｽﾀｯｸﾁｬﾝ M5GoBottom版のファームウェアについて | M5Stack沼人の日記](https://raspberrypi.mongonta.com/softwares-for-stackchan/)


# ビルド方法
　v0.1はArduinoIDEでしたが、現在はPlatformIOでのビルドを想定しています。

# ｽﾀｯｸﾁｬﾝについて
ｽﾀｯｸﾁｬﾝは[ししかわさん](https://github.com/meganetaaan)が公開しているオープンソースのプロジェクトです。

https://github.com/meganetaaan/stack-chan

# author
 Takao Akaki

# LICENSE
 MIT