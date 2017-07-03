# d1mini-matrix-clock

A clock, using a matrix led shield on a ESP8266 D1 mini.

## usage

On first boot, you must connect to the 'd1mini-clock' captive network and
set your local wifi credentials.

## features

- gets internet time from NTP (NTPClient)
- apply summer time offset (Timezone)
- spawn a hotspot to get wifi connectivity (WiFiManager)
- over the air update (ArduinoOTA)
- remote telnet debug (RemoteDebug)
