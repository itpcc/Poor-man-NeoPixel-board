# Poor man's NeoPixel neck oversized card

I do this project to try using NeoPixel LED strip for "illuminate shirt". The idea is, put NeoPixels on the neck-hanging oversized card, connect to network and drive LEDs using ESP8266, and control with whatever program that support TPM2.net ([Jinx](https://www.ledstyles.de/index.php?thread/22271-jinx-led-matrix-control-und-die-n%C3%A4chste-matrix-software/) for example) just for fun.

//@TODO: Insert example GIF

## What the hell are you doing.

My company have an outing party with "Neon" theme. At first, I'd like to use some funky "EL Wire", or even electroluminiscent shirt. But, when I check the price, I've to abandon that idea. At the same time, I see [Andreas Spiess's video about NeoPixel](https://www.youtube.com/watch?v=IIl5nDjfkjY). So, I do this project instead.

In this project, I create a 8x12 NeoPixel LEDs matrix on the wooden board, tie with a neck lanyard, hang the board inside a white T-shirt (to hide the eyesore wiring and make the light diffuse),  connect with ESP8266 to drive those LEDs, and use Jinx to make the shirt look like the shirt is illuminated.

## Resources needed to use

### Hardware

- ESP8266-06 (The greater version should be fine to use. I've this model laying around so I use that 🤷‍♂️)
- FTDI (Or any (?) USB to UART converter)
- NeoPixel LED strip. (Of course) I buy [this model from AliExpress](https://www.aliexpress.com/item/32682015405.html).
- Misc. electronic components. (See in [Hardwares/BOM.csv](./Hardwares/BOM.csv) for more, specific detail)
- Power supply. I use 2A-capable output powerbank since the strip use 5V power.
- Platform. I use an A4-szie processed wooden board because it's fit for the job (Easy to drill and attach LED strip, not too heavy, and CHEAP! I buy from the local stationery shop for ~ ¢30 )

### Software

- Sketch developing
	* Arduino (or VSCode with PlatformIO if you prefer)
	* ESP8266 developer SDK
	* 3rd party
		+ [FastLED](https://github.com/FastLED/FastLED) for LED driving. You can use Adafruit_NeoPixel if you like.
		+ [ESPAsyncUDP](https://github.com/me-no-dev/ESPAsyncUDP) for UDP packet handling.
- LED matrix controlling
	* Jinx for a comprehensive LED controlling via PC
	* Any application that can transmit UDP packet that comply TPM2.net protocol


## More detail: TBD

Cuz, at the time I create this repo, I've very sleepy and I've to prepare for the company outing I mentioned earlier.