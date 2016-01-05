<img src="https://github.com/A2TY/ceiling-light_2_terminal-device/blob/master/c2t_logo.png" alt="c2t logo" height="70" >
# Project Ceiling Light 2 Terminal Device

## 概要
このプロジェクトは，シーリングライトの電子端末化を行うものである．mosquittoサーバとしてRaspberry Piを使用し，クライアントにESP-WROOM-02を用いてMQTT通信により連携を行う．主な機能は，ネットワークリモコンおよび情報通知である．

## 機能
### ネットワークリモコン
スマートフォン・PCからWebSocketを用いて情報を送り，シーリングライトから赤外線LEDを用いてTVやエアコンに送信する．

### 情報通知
Twitterから情報を取得し，フルカラーLEDの点灯の違いによって情報を通知する．

**通知内容**
* 天気　　　　　　　tenki.jp （ 東京 ）　@tenkijp_tokyo
* 電車の遅延情報　　東武東上線運行情報【非公式】　@train_tbtoj


