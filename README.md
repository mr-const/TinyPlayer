This firmware uses ESP8266Audio library for I2S audio playback.

## How to use
upload mp3 file named track.mp3 to SPIFFS, flash firmware and reboot

## Wiring
Wiring Wemos D1 Mini to Adafruit MAX98357A

ESP8266 has hardware I2S support so it's needed to wire dedicated pins

`5V -> Vin`
`G  -> GND`

`RX -> DIN`
`D8 -> BCLK`
`D4 -> LRC`