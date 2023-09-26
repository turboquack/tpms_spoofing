# tpms_spoofing
Simulate TPMS sensor<br>
![alt text](https://github.com/turboquack/tpms_spoofing/blob/main/pictures/device.jpg)<br>
This device will appear as one of two TPMS sensors in [rtl_433](https://github.com/merbanan/rtl_433/tree/master)<br>
I recorded the original signal using [URH]([https://github.com/merbanan/rtl_433/tree/master](https://github.com/jopohl/urh))<br>
# Hardware
CC1101<br>
D1 Mini V3 NodeMCU ESP8266EX<br>
# Software
[SmartRC-CC1101-Driver-Lib_V2.5.7](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib)<br>
# Supported devices
Mitsubishi Lancer as Citroen TPMS :heavy_check_mark:<br>
Suzuki Grand Vitara as Hyundai VDO :heavy_check_mark:<br>

# Usage
Inside variable ```buffers``` you can change all the values (id,pressure,temp etc). For more info check out [Citroen](https://github.com/merbanan/rtl_433/blob/master/src/devices/tpms_citroen.c) and [Hyundai](https://github.com/merbanan/rtl_433/blob/master/src/devices/tpms_hyundai_vdo.c)
