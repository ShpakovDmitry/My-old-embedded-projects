#ifndef BUTTON_H
#define BUTTON_H

#define DEBOUNCE_TIME 100

#define BTN_1_REL (RELEASED << PB0)
#define BTN_2_REL (RELEASED << PB1)
#define BTN_3_REL (RELEASED << PB2)
#define BTN_4_REL (RELEASED << PB3)
#define BTN_5_REL (RELEASED << PB4)
#define BTN_6_REL (RELEASED << PB5)
#define BTN_7_REL (RELEASED << PB6)
#define BTN_8_REL (RELEASED << PB7)

#define ALL_RELEASED	( BTN_1_REL | BTN_2_REL | BTN_3_REL | BTN_4_REL | \
						BTN_5_REL | BTN_6_REL | BTN_7_REL | BTN_8_REL )

enum ButtonState { PRESSED, RELEASED };

typedef struct {
	unsigned char pin : 3;
	char keyChar;
} Button;

static Button buttons[] = {
	{ PB0, '1'},
	{ PB1, '2'},
	{ PB2, '3'},
	{ PB3, '4'},
	{ PB4, '5'},
	{ PB5, '6'},
	{ PB6, '7'},
	{ PB7, '8'}
};

#endif // BUTTON_H
