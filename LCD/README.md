## This Sketch is a accumulation of several "how-tos"

The script is provided without any warrantee and has is.
The Sketch is a combination of examples on LCD over i2c, serial communication
and other personal ideas.

Most examples show how to achieve one or the other but not both.
Even when combined a new "feature" pops up, an "overflow", that exposes it self as garbage characters on screen.
This appends, sometimes, when receiving the first bites from serial.

### Features
- This sketch adds a class to "animate" or manipulate output to the screen.

### Description of the environment
- This Sketch is working, at least for me, in a LCD 16x2 HD44780 over i2c.
- The serial streaming program is LCDproc.
- For me right now is an Arduino nano.

### Solution to the overflow is the function reset
The function reset is triggered when the serial stream is initiated.
Even when the overflow occurs, the reset is able to restore functionality.
Because I only experienced this on "boot" no mitigation exists to recurrence of the overflow.

Please no not any assumptions on the safety of this code.
The stream of data revived over serial is not satanised there for an overflow may occur an any time.
IF your thinking of using this code in some risky environment please consider satanizing the stream and 
safeguard against potential failures.
