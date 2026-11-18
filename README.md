# ✨ Arduino UNO Q 星座運勢燈光控制器

這是一個結合 **Arduino UNO Q (App Lab)** 與網頁前端的物聯網（IoT）互動專題。使用者透過網頁選擇星座後，系統會即時從網路獲取今日星座運勢，並連動控制板子上的 **13×8 藍色 LED 矩陣**與 **4 顆板載 RGB LED**，將抽象的運勢轉化為實體視覺燈光回饋。

---

## 🌟 功能特色

* **即時線上運勢查詢：** 透過 Python 後端串接星座運勢 API（具備離線備援機制），取得今日運勢解析與 12 星座專屬幸運色。
* **13×8 藍色 LED 矩陣跑馬燈：** 選定星座後，板載 LED 矩陣會即時滾動顯示對應的星座英文名稱（如 `ARIES`、`LEO`）。
* **運勢主題按鈕切換（0～4 級）：** 網頁提供「整體運勢、愛情運勢、事業運勢、財富運勢」四個主題按鈕，點擊後會根據該項運勢等級（Lv.0 ～ Lv.4），點亮板載 **0 到 4 顆 RGB LED**。
* **專屬幸運色顯示：** RGB LED 亮起的顏色會自動對應為該星座當日的幸運色（如牡羊座烈焰紅、天蠍座神祕紫等）。

---

## 🛠️ 硬體與開發環境

* **開發板：** Arduino UNO Q (STM32U585 MCU + Linux MPU)
* **內建硬體使用：**
  * 13×8 藍色 LED 矩陣（`Arduino_LED_Matrix`）
  * 4 顆板載 RGB LED（LED 1～2 由 MPU `Leds` 控制；LED 3～4 由 MCU `RouterBridge` 控制）
* **開發平台：** Arduino App Lab
* **程式語言：** Python (`WebUI` Brick)、C++ (Arduino Sketch)、HTML / CSS / JavaScript

---

## 📁 專案結構

```text
star/
├── assets/
│   └── index.html       # 前端網頁介面（星座選單、運勢顯示、主題按鈕）
├── python/
│   └── main.py          # WebUI 伺服器、抓取今日運勢 API、透過 Bridge 控制硬體
├── sketch/
│   ├── sketch.ino       # MCU 端韌體：控制 13x8 矩陣跑馬燈與 LED 3、LED 4
│   └── sketch.yaml      # Sketch 設定檔
├── app.yaml             # Arduino App Lab 專案與 WebUI Brick 設定
└── README.md            # 專案說明文件
```
