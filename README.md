# Remote Camera
Solar power remote camera personal project

## Hardware

- Orange LRS transceiver (https://hobbyking.com/en_us/orangerx-open-lrs-433mhz-transmitter-100mw-compatible-with-futaba-radio.html)
	- Arduino (mini)
	- RFM22 transceiver module
		- datasheet (https://www.sparkfun.com/datasheets/Wirel	ess/General/RFM22.PDF)
	- (possible replacement) NRF24 transceiver
		- antenna range extension mod (https://www.instructables.com/id/Enhanced-NRF24L01/)
- Raspberry Pi
- old computer/laptop
- solar panels
	- need power calculations (setup draw and panel output)


![the link should be here](https://c1.staticflickr.com/9/8050/8449935217_1c4ba62779_b.jpg)
![the link should be here](http://www.hobbytronics.co.uk/image/data/tutorial/arduino-hardcore/atmega328-tqfp-arduino-pinout.jpg)

## Software

- Raspberry Pi Script
	- python script for Serial com <-> Arduino
		- install raspbian
		- install pip
		- install python serial lib
	- telnet/http/? for read/write camera data
		- install telnet
- Arduino Code
	- RadioHead Lib (http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF22.html)
		- software SPI setup for custom pins
		- set max power
	- Serail Comm
- Server Code
	- Serial Comm
	- Web Server
	- ngrok/localtunnel expose 
- Xiaomi Yi Script
	- allow live updates / time lapse ? 
	- resources 
		- http://nutseynuts.blogspot.com/2015/06/xiaomi-yi-action-cam-custom-scripts.html
		- http://www.tawbaware.com/xiaomiyi.htm
