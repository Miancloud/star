import datetime
import json
import urllib.request
from arduino.app_utils import *
from arduino.app_bricks.web_ui import WebUI

ui = WebUI()

ZODIAC_INFO = {
    "aries":       {"name": "牡羊座", "colorName": "烈焰紅",   "rgb": [255, 0, 0]},
    "taurus":      {"name": "金牛座", "colorName": "大地綠",   "rgb": [0, 255, 0]},
    "gemini":      {"name": "雙子座", "colorName": "亮麗黃",   "rgb": [255, 255, 0]},
    "cancer":      {"name": "巨蟹座", "colorName": "珍珠白",   "rgb": [255, 255, 255]},
    "leo":         {"name": "獅子座", "colorName": "帝王橘",   "rgb": [255, 100, 0]},
    "virgo":       {"name": "處女座", "colorName": "靛青藍",   "rgb": [0, 150, 255]},
    "libra":       {"name": "天秤座", "colorName": "浪漫粉",   "rgb": [255, 0, 150]},
    "scorpio":     {"name": "天蠍座", "colorName": "神祕紫",   "rgb": [180, 0, 255]},
    "sagittarius": {"name": "射手座", "colorName": "寶石藍",   "rgb": [0, 50, 255]},
    "capricorn":   {"name": "摩羯座", "colorName": "琥珀棕",   "rgb": [255, 80, 0]},
    "aquarius":    {"name": "水瓶座", "colorName": "水藍青",   "rgb": [0, 255, 255]},
    "pisces":      {"name": "雙魚座", "colorName": "夢幻紫紅", "rgb": [255, 0, 255]},
}

# 根據等級 (0~4) 控制 4 顆板載 RGB LED 亮起對應數量與幸運色
def update_4_leds(level: int, r: int, g: int, b: int):
    # LED 1 (MPU 控制，bool 開關)
    if level >= 1:
        Leds.set_led1_color(r > 0, g > 0, b > 0)
    else:
        Leds.set_led1_color(False, False, False)

    # LED 2 (MPU 控制，bool 開關)
    if level >= 2:
        Leds.set_led2_color(r > 0, g > 0, b > 0)
    else:
        Leds.set_led2_color(False, False, False)

    # LED 3 (MCU 控制，PWM 0~255)
    if level >= 3:
        Bridge.call("set_led3_color", r, g, b)
    else:
        Bridge.call("set_led3_color", 0, 0, 0)

    # LED 4 (MCU 控制，bool 開關)
    if level >= 4:
        Bridge.call("set_led4_color", r > 0, g > 0, b > 0)
    else:
        Bridge.call("set_led4_color", False, False, False)

# 初始化時先將 LED 1 和 2 關閉
update_4_leds(0, 0, 0, 0)

def handle_horoscope(sign="aries"):
    sign = str(sign).lower()
    info = ZODIAC_INFO.get(sign, ZODIAC_INFO["aries"])

    try:
        url = f"https://ohmanda.com/api/horoscope/{sign}/"
        req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
        with urllib.request.urlopen(req, timeout=5) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            horoscope_text = data.get("horoscope", "")
    except Exception:
        horoscope_text = (
            f"今日{info['name']}的能量場相當活躍！適合主動推進計畫與人際交流，"
            f"配戴或使用幸運色「{info['colorName']}」能為你帶來好運。"
        )

    seed = datetime.date.today().day + len(sign)
    levels = {
        "overall": seed % 5,
        "love":    (seed + 2) % 5,
        "career":  (seed + 4) % 5,
        "wealth":  (seed + 1) % 5,
    }

    return {
        "sign": sign.upper(),
        "name": info["name"],
        "colorName": info["colorName"],
        "rgb": info["rgb"],
        "text": horoscope_text,
        "levels": levels
    }

def handle_led(sign="ARIES", level="0", r="0", g="0", b="0"):
    sign_str = str(sign).upper()
    lvl = int(level)
    rv, gv, bv = int(r), int(g), int(b)

    print(f"[執行更新] 星座: {sign_str}, 等級: {lvl}, RGB: ({rv}, {gv}, {bv})")
    
    # 1. 更新 13x8 藍色矩陣跑馬燈
    Bridge.call("set_sign_text", sign_str)
    # 2. 更新 4 顆 RGB LED (亮 0~4 顆)
    update_4_leds(lvl, rv, gv, bv)

    return {"status": "ok", "sign": sign_str, "level": lvl}

ui.expose_api("GET", "/api/horoscope", handle_horoscope)
ui.expose_api("GET", "/api/led", handle_led)

if __name__ == "__main__":
    App.run()