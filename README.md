# [humans-in-space](https://github.com/rickhewes/humans-in-space/)

Simple [esp8266](https://www.wemos.cc/en/latest/d1/d1_mini.html) and [OLED shield](https://www.wemos.cc/en/latest/d1_mini_shield/oled_0_66.html) showing how many humans are in spce right now.

Here is the display in action:

![esp8266 and shield](https://github.com/rickhewes/humans-in-space/blob/master/assets/humans-in-space-demo.jpg "esp and shield")

Uses [this](http://api.open-notify.org/) API for data. Thank you.

## Installation

Install the support libraries:
    [HCuOLED](https://forum.hobbycomponents.com/viewtopic.php?t=1817)
    [WiFiManager](https://github.com/tzapu/WiFiManager)
    [ArduinoJson](https://arduinojson.org/)


## Configure the WiFi

Connect to the HumansInSapce Access Point and configure your WiFI credentials.

See this [tutorial](https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password)