// Created for DOORDom (C)  2019.


#include <Keypad.h>	// Keypad(), setHoldTime(), setDebounceTime(),

unsigned long int BUZZER_BEEP_LENGTH = 10;			// buzzer beep duration in miliseconds
unsigned long int BUZZER_FINAL_BEEP_LENGTH = 1000;	// buzzer final beep duration in miliseconds
unsigned long int RELAY_ON_TIME = 5000;				// relay on time at final sequence
unsigned long int DEBOUNCE_TIME = 250;				// debounce time in ms

boolean LOCK_AFTER_FINAL_SEQUENCE = true;			// if true then hard reset will needed after correct key sequence
boolean SEND_DEBUG_INFO = false;					// if true then debug information will be send to terminal

const byte buzzer_pin = 10;	// pin to which connect buzzer
const byte relay_pin  = 11;	// pin to which connect relay

const byte ROWS = 4;	// row count
const byte COLS = 4;	// column count
byte row_pins[ROWS] = {2, 3, 4, 5};	// pins for rows
byte col_pins[COLS] = {6, 7, 8, 9};	// pins for columns

char keypad_layout[ROWS][COLS] = 	// keypad layout chars
{
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

Keypad keypad = Keypad( makeKeymap(keypad_layout), row_pins, col_pins, ROWS, COLS);	// create keyboard class

const byte PASS_LEN = 5;	// password length, is limited to max 255 symbols :)
const char pass[PASS_LEN] = {'5','4','5','5','3'};	// password; note that charset should be in keypad_layout

// cyclic buffer implementation
typedef struct
{
	char c_buff[PASS_LEN];
	unsigned char pos;
	unsigned char size;
}cyclic_buffer;

// functions to handle cyclic buffer
void c_buff_init(cyclic_buffer *buff);								// init c_buffer, buffer size depends on PASS_LEN variable!
void c_buff_add(cyclic_buffer *buff, const char ch);				// add char to c_buffer
boolean c_buff_find_sequence(cyclic_buffer *buff, const char *seq);	// returns TRUE if sequence found, else FALSE

cyclic_buffer c_buff;	// define cyclic buffer variable to use with keyboard

void buzzer_beep(unsigned long int duration);	// function to make buzzer beep during duration in ms; is blocking function
void relay_on(unsigned long int duration);		// function to switch relay on during duration in ms; is blocking function
void final_sequence();							// function to call at fianl sequence, when correct pass is entered
void infinite_loop();							// infinite loop
void update_buffer_and_check(const char key);	// function to handle buffer update, correct sequence check and to do final actions


void setup()
{
	if(SEND_DEBUG_INFO == true )
		Serial.begin(9600);	// to send debug information
	pinMode(buzzer_pin, OUTPUT);	// output pin for buzzer
	pinMode(relay_pin, OUTPUT);	// output pin for relay
	digitalWrite(buzzer_pin, LOW);	// set low at init
	digitalWrite(relay_pin, LOW);	// set low at init

	keypad.setDebounceTime(DEBOUNCE_TIME);	// to eliminate contact bounce
	
	c_buff_init(&c_buff);		// init cyclic buffer. buffer size depends directly from PASS_LEN variable
}

void loop()
{
	char key = keypad.getKey();	// get key
	
	update_buffer_and_check(key);	// note function may be blocking if LOCK_AFTER_FINAL_SEQUENCE is true
}


// function implementation
void buzzer_beep(unsigned long int duration)
{
	digitalWrite(buzzer_pin, HIGH);
	delay(duration);
	digitalWrite(buzzer_pin, LOW);
}

void relay_on(unsigned long int duration)
{
	digitalWrite(relay_pin, HIGH);
	delay(duration);
	digitalWrite(relay_pin, LOW);
}

void final_sequence()
{
	buzzer_beep(BUZZER_FINAL_BEEP_LENGTH);
	relay_on(RELAY_ON_TIME);
	
	if(LOCK_AFTER_FINAL_SEQUENCE == true)
		infinite_loop();
}

void infinite_loop()
{
	if(SEND_DEBUG_INFO == true)
		Serial.println("Entering infinite loop. Bye bye");
	while(1)
	{
		;
	}
}

void update_buffer_and_check(const char key)
{	
	if(key != NO_KEY)		// if key pressed, make a beep, then add char to buffer, then 
	{						// check for password sequence, and if sequence found then call final function
		if(SEND_DEBUG_INFO == true)
		{
			Serial.print("Key pressed: ");
			Serial.println(key);
		}

		buzzer_beep(BUZZER_BEEP_LENGTH);				// make beep that user hears that key was pressed
		
		c_buff_add(&c_buff, key);						// update key buffer
		if(c_buff_find_sequence(&c_buff, pass) == true)	// check for sequence
		{
			if(SEND_DEBUG_INFO == true)
				Serial.println("Correct sequence found");
			final_sequence();	// call final sequence
		}
		else
		{
			if(SEND_DEBUG_INFO == true)
				Serial.println("Correct sequence not found");
		}
	}
}

void c_buff_init(cyclic_buffer *buff)
{
	buff->pos = 0;
	buff->size = PASS_LEN;
}

void c_buff_add(cyclic_buffer *buff, const char ch)		// add char to c_buffer
{
	*(buff->c_buff + buff->pos) = ch;
	if(buff->pos == buff->size - 1)	//if we reach end of c_buff then go to start
	{
		buff->pos = 0;
	}
	else
	{
		buff->pos += 1;				// else go to next c_buff position
	}
}

boolean c_buff_find_sequence(cyclic_buffer *buff, const char *seq)	// returns true if sequence found, else false
{
	unsigned char i;
	boolean flag = true;
	
	for(i = 0; i < buff->size && seq[i] != '\0'; ++i)
	{
		if( *(buff->c_buff + (buff->pos + i) % (buff->size) ) != seq[i] )
			flag = false;
	}
	
	return flag;
}














