### Sketch for arduino

Source created for quest room DoorDom.

Device consists of Arduino Nano, matrix keyboard connected to it, buzzer and relay.

When user enters correct sequence on keyboard, relay is switched on, also at every key press buzzer togled for a short time of period to indicate that key was pressed by user.

There is no way to change password when device is flashed.

Password validation is done by pushing pressed key code into cyclic buffer.

And at every key press buffer is compared with password sequence.

This is done to check valid key pressed five in a row.
