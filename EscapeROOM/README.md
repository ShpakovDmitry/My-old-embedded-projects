Project for EscapeRoom.

Project consists of two sources for two separate devices.

First source "med_panel.c" if for device that switchs ON UV lamp when pins of uC are correctly connected by external panel
with user friendly UI :) .  Pin connect correctness is checked by applying test voltage on uC pin an monitoring it on desired pin,
thus detecting correct connection made by user.

Second source "radio_morse_panel.c" is for device that plays preset Morse Code when correct sequence is pressed on external panel
with buttons.

Both projects utilizes AVR ATtiny2313 uC.
