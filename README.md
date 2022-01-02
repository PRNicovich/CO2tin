CO2 Tin. Measure local CO2 (and temp, humidity) in a device you can easily toss in a pocket.

Major components:
CO2 Sensor : https://www.adafruit.com/product/5187 ($49.50)
Microcontroller : https://www.adafruit.com/product/4600 ($7.50)
Display : https://www.adafruit.com/product/4440 ($12.50)
LiPo Battery : https://www.adafruit.com/product/1578 ($7.95)
LiPo Battery driver : https://www.adafruit.com/product/1944 ($14.95)
Wee mints tin - I found one from some cheap off-brand mints that was a hair bigger than the Altoids standard, but the Altoids tin should work. 
Small slide switch - https://www.adafruit.com/product/805 or equivalent
Hook up wire
Kapton tape
Hot glue 
Foam tape

Most connections are using the included STEMMA ports on the sensor, MCU, and display.  To save a bit of space I removed the connector on the display then soldered a short halved STEMMA cable to the pads on that board. Then Plugged that into the CO2 sensor, then onward to the MCU.  MCU, LiPo battery, and driver board in the base of the tin with the switch.  Battery plugs into specific port on the LiPo driver board. Everything is held down with foam tape or an abundance of hot glue. I needed to use Kapton tape in a few spots to keep pads from shorting to the tin case.  

In addition to the STEMMA ports, the following solder connections are required:
- Switch leads between LiPo driver GND and EN
- LiPo driver GND to MCU GND
- LiPo driver +5v to MCU +5v
- LiPo driver USB to LiPo driver +5v
- LiPo driver LB to MCU A3 

It's a fun game to get everything connected and packed into the tin.  To start I used a buddy's mill to cut a window in the lid for the display plus a slot for the USB-C and switch.  Solder the connections above in parts while you secure bits.  I started with the display, then CO2 sensor on the lid.  MCU and battery before the LiPo driver.  It took a few tries to not snap wires or short anything with the lid closed.

Code is in Arduino.  Set the board up to use the Arduino IDE:
https://learn.adafruit.com/adafruit-qt-py/arduino-ide-setup
and you can upload the code over USB-C. 

Switch on will turn on display. Charge over USB-C. Can also leave plugged in and it'll stay on. Sensor takes a little while to settle (120 sec nom).  The progress bar at the bottom of the display takes 120 seconds to transit the whole width to help keep track.  Temp tends to aim high from heat in the can.  You can open up the can to get it a bit more accurate.  The CO2 sensor is also auto-calibrating; take it outside for half an hour periodically and when you first start for it to find a baseline.  

Battery goes ~4 hrs on a charge in my experiments. 

Outside is ~425 ppm. Inside 500 ppm on up, depending on how many folks are in there breathing vs the air flow.  1000+ ppm is not great. 