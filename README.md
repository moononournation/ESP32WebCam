# ESP32 WebCam

## How to use

### Configure the project

```
idf.py menuconfig
```

* Set WiFi SSID and WiFi Password and Maximum retry under Example Configuration Options.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)
