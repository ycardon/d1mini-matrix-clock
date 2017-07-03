# d1mini-matrix-clock

A clock, using a matrix led shield on a ESP8266 D1 mini.

## usage

On first boot, you must set your wifi credentials by connecting to the
'd1mini-clock' captive network.

## features

- gets internet time from NTP (NTPClient)
- apply summer time offset (Timezone)
- spawn a hotspot to get wifi connectivity (WiFiManager)
- over the air update (ArduinoOTA)
- remote telnet debug (RemoteDebug)
