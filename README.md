# arduino_doorman
A Doorman project with Arduino and 3D printer

# List of materials

## Main List:
- Arduino NANO (or any other model you want)
- PCA9685 controller
- Micro Servo 9G SG90 (5x)
- RGB LED (2x, one for each eye)
- 270ohms Resistor
- PIR Sensor

## Source Power
I've used a 9V rechargeble battery source, so I may recharge the battery and use it again.
To adjust the voltage, I made a "self made" voltage control:
- LM7805 transistor (1x)
- 1000uf/16v capacitor (1x)
- 100uf/3v capacitor (1x)

Or, you can use any other type of source power and a pre-built or commercial voltage control.

# Arduino
I've used a Arduino NANO, because of it's size, but you can use any other version.
Just remember: adjust the Code's pins definitions.

Attempt to these pre requisits:
- PCA9685 **MUST BE CONNECTED** to the SDA/SCL equivalent ports (check your Arduino specific version Pinout manual)
- RGB LEDs **MUST BE CONNECTED** to PWM port's
- PIR sensor can be connected to any free port
