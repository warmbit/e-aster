# e-aster
E-Aster is a beautiful 48mm circular disc with 61 WS2812B-2020 RGB LEDs

* Check its page on [Hackaday](https://hackaday.io/project/179746-e-aster)
* Buy it from [my Tindie store](https://tindie.com/stores/warmbit)

More code is on its way.

## How the LEDs are numbered?

You may need this information when programming your Arduino board.

![e-aster LED numbering](https://github.com/warmbit/e-aster/blob/main/e-aster_led_number.png)

Always be cautious about the peak current. How much is supported by your Arduino board? 

## Connect E-Aster to Arduino

There are just three wires to solder. 

* One power (red) to the VUSB or 5V power supply.  
* One ground (black) to the GND.
* One data (yellow or green) to anything pin, either analog or digital should be fine.
 
To run the Arduino IDE, remember to add your board first.
