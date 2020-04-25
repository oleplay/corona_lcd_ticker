# Import needed Libraries
import RPi.GPIO as GPIO
import os

# Use GPIO Numbers not internal pin numbers
GPIO.setmode(GPIO.BCM)

# Which GPIO to use
pin = 23

# Setup GPIO pin and set internal pull up resistor
GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

try:
    
    # Wait for PIN to be pulled to GND
    GPIO.wait_for_edge(pin, GPIO.FALLING)
    
    
    
    print("Now reloading data")

except KeyboardInterrupt:
    # clean up GPIO on CTRL+C exit
    GPIO.cleanup()
    
    pass
    
GPIO.cleanup()           # clean up GPIO on normal exit 