#include <Arduino_RouterBridge.h>
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

// ================= 13x8 藍色 LED 矩陣緩衝區與內建 5x7 字型 =================
uint8_t frame[8][13];
String currentSign = "READY";
int scrollOffset = 0;
unsigned long lastScrollTime = 0;
const int SCROLL_INTERVAL = 90;

const uint8_t FONT_5X7[][5] = {
  {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (0)
  {0x7F, 0x49, 0x49, 0x49, 0x36}, // B (1)
  {0x3E, 0x41, 0x41, 0x41, 0x22}, // C (2)
  {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D (3)
  {0x7F, 0x49, 0x49, 0x49, 0x41}, // E (4)
  {0x7F, 0x09, 0x09, 0x09, 0x01}, // F (5)
  {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G (6)
  {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H (7)
  {0x00, 0x41, 0x7F, 0x41, 0x00}, // I (8)
  {0x20, 0x40, 0x41, 0x3F, 0x01}, // J (9)
  {0x7F, 0x08, 0x14, 0x22, 0x41}, // K (10)
  {0x7F, 0x40, 0x40, 0x40, 0x40}, // L (11)
  {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M (12)
  {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N (13)
  {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O (14)
  {0x7F, 0x09, 0x09, 0x09, 0x06}, // P (15)
  {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q (16)
  {0x7F, 0x09, 0x19, 0x29, 0x46}, // R (17)
  {0x46, 0x49, 0x49, 0x49, 0x31}, // S (18)
  {0x01, 0x01, 0x7F, 0x01, 0x01}, // T (19)
  {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U (20)
  {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V (21)
  {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W (22)
  {0x63, 0x14, 0x08, 0x14, 0x63}, // X (23)
  {0x07, 0x08, 0x70, 0x08, 0x07}, // Y (24)
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z (25)
  {0x00, 0x00, 0x00, 0x00, 0x00}  // Space (26)
};

// 更新 13x8 跑馬燈文字
void set_sign_text(String sign) {
  sign.toUpperCase();
  currentSign = "  " + sign + "  ";
  scrollOffset = 0;
}

// 控制 LED 3 (PWM 腳位)
void set_led3_color(int r, int g, int b) {
  analogWrite(LED3_R, r);
  analogWrite(LED3_G, g);
  analogWrite(LED3_B, b);
}

// 控制 LED 4 (數位腳位，LOW = 亮，HIGH = 滅)
void set_led4_color(bool r, bool g, bool b) {
  digitalWrite(LED4_R, r ? LOW : HIGH);
  digitalWrite(LED4_G, g ? LOW : HIGH);
  digitalWrite(LED4_B, b ? LOW : HIGH);
}

int getCharIndex(char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a';
  return 26;
}

void updateMatrixScroll() {
  memset(frame, 0, sizeof(frame));
  int totalCols = currentSign.length() * 6;

  for (int x = 0; x < 13; x++) {
    int virtualCol = (scrollOffset + x) % totalCols;
    int charPos = virtualCol / 6;
    int colInChar = virtualCol % 6;

    if (colInChar < 5) {
      int idx = getCharIndex(currentSign[charPos]);
      uint8_t colData = FONT_5X7[idx][colInChar];
      for (int y = 0; y < 7; y++) {
        frame[y + 1][x] = (colData >> y) & 0x01;
      }
    }
  }

  matrix.renderBitmap(frame, 8, 13);
  scrollOffset = (scrollOffset + 1) % totalCols;
}

void setup() {
  pinMode(LED4_R, OUTPUT);
  pinMode(LED4_G, OUTPUT);
  pinMode(LED4_B, OUTPUT);

  set_led3_color(0, 0, 0);
  set_led4_color(false, false, false);

  matrix.begin();
  set_sign_text("READY");

  Bridge.begin();
  Bridge.provide("set_sign_text", set_sign_text);
  Bridge.provide("set_led3_color", set_led3_color);
  Bridge.provide("set_led4_color", set_led4_color);
}

void loop() {
  if (millis() - lastScrollTime >= SCROLL_INTERVAL) {
    lastScrollTime = millis();
    updateMatrixScroll();
  }
}