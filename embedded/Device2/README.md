Source for questroom DoorDom.

Device is build using Arduino nano with connected keypad, buttons ,MP3 module, buzzer and relays.

The source may be compiled using Arduino IDE.

Arduino nano module reacts on button press and plays sound using MP3 module according to algorithm.
Then password should be entered to activate relays which controls next part of quest.
Pasword validation is implemented using cyclic buffer which leads to check correct password sequence
indepedent of amount of keys pressed.
