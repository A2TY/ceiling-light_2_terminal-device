<img src="https://github.com/A2TY/ceiling-light_2_terminal-device/blob/master/c2t_logo.png" alt="c2t logo" height="70" >
# Project Ceiling Light 2 Terminal Device

## 概要
このプロジェクトは，シーリングライトの電子端末化を行うものである．プラットフォームにESP-WROOM-02を用いてハードウェアの開発を行う．主な機能は，ネットワークリモコンおよび情報通知である．

## 機能
### ネットワークリモコン
スマートフォン・PCからWebSocketを用いて情報を送り，シーリングライトから赤外線LEDを用いてTVやエアコンに送信する．

### 情報通知
WebAPIを用いて情報を取得し，フルカラーLEDの点灯の違いによって情報を通知する．

**現段階で考えている通知内容**
* 天気
* 電車の遅延情報

