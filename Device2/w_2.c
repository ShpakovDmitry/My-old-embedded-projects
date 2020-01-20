// Created for DOORDom (C)  2019.



#include <Keypad.h>				// Keypad(),
#include <DFPlayer_Mini_Mp3.h>	// mp3_set_serial(), mp3_set_volume(),
#include <SoftwareSerial.h>		// SoftwareSerial, 

const byte MP3_1 = 1;
const byte MP3_2 = 2;
const byte MP3_3 = 3;
const byte MP3_4 = 4;
const byte MP3_5 = 5;

boolean LOCK_AFTER_FINAL_SEQUENCE = true;			// if true then hard reset will needed after correct key sequence
boolean SEND_DEBUG_INFO = false;					// if true then debug information will be send to terminal

const unsigned long BAUDRATE = 9600;			// baud rate for serial comunications 
const byte VOLUME = 20;							// volume of all songs playing
const byte MAXVOLUME = 30;						// max song volume, should not to be changed, as DFPlayer has max volume of 30
const byte EQUALIZER = 0;						// mp3 player equalizer preset 0-normal, 1-pop, 2-rock, 3-jazz, 4-classic, 5-bass
const byte MAXEQUALIZER = 5;					// mp3 player max number of equalizer
const unsigned long MP3_COMMAND_DELAY = 100;	// delay in ms after command is sent to mp3 player, for stabil player work
const unsigned long DEBOUNCE_TIME = 250;		// debounce time in ms
const unsigned long RELAY1_ON_TIME = 300;		// relay 1 on time time in ms
unsigned long int BUZZER_BEEP_LENGTH = 10;			// buzzer beep duration in miliseconds

const byte ROWS = 2;					// row count
const byte COLS = 3;					// column count
byte row_pins[ROWS] = {2, 3};			// pins for rows
byte col_pins[COLS] = {4, 5, 6};		// pins for columns
const byte SOFTWARE_RX_pin = 7;
const byte SOFTWARE_TX_pin = 8;
const byte DFPlayer_busy_pin = 9;
const byte BUTTON1_pin = 10;
const byte BUTTON2_pin = 11;
const byte BUTTON3_pin = 12;
const byte RELAY1_pin = 13;
const byte RELAY2_pin = 14;
const byte RELAY3_pin = 15;
const byte BUZZER_pin = 16;

char keypad_layout[ROWS][COLS] = 	// keypad layout chars
{
	{'1', '2', '3'},
	{'4', '5', '6'},
};

Keypad keypad = Keypad( makeKeymap(keypad_layout), row_pins, col_pins, ROWS, COLS);	// create keyboard class

const byte PASS_LEN = 5;	// password length, is limited to max 255 symbols :)
const char pass[PASS_LEN] = {'1','2','3','4','5'};	// password; note that charset should be in keypad_layout

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

void InitSerial(void);
void InitDFPlayer(void);
void InitPins(void);
boolean ButtonPressed(const byte BUTTON_pin);
void DFPlayer_command_delay(void);
void PlayInstructionMusic(void);
void WaitButton1Press(void);
void WaitButton2Press(void);
void WaitButton3Press(void);
void HandleKeyboard(void);


SoftwareSerial mySerial(SOFTWARE_RX_pin, SOFTWARE_TX_pin);




void setup()
{
	InitSerial();
	InitDFPlayer();
	InitPins();
	if(SEND_DEBUG_INFO == true)
		Serial.println("Init Complete");
}

void loop()
{
	if(SEND_DEBUG_INFO == true)
		Serial.println("Playing Instruction Music");
	PlayInstructionMusic();
	if(SEND_DEBUG_INFO == true)
		Serial.println("Waiting for button 1 press");
	WaitButton1Press();
	if(SEND_DEBUG_INFO == true)
		Serial.println("Waiting for button 2 press");
	WaitButton2Press();	
	if(SEND_DEBUG_INFO == true)
		Serial.println("Waiting for button 3 press");
	WaitButton3Press();
	if(SEND_DEBUG_INFO == true)
		Serial.println("Entering in keyboard handler");
	HandleKeyboard();
}





void InitSerial(void)
{
	if(SEND_DEBUG_INFO == true )
		Serial.begin(9600);	// to send debug information
	mySerial.begin(BAUDRATE);
}

void InitDFPlayer(void)
{
	DFPlayer_command_delay();
	mp3_set_serial(mySerial);
	
	DFPlayer_command_delay();
	mp3_set_volume(VOLUME % (MAXVOLUME + 1));
	
	DFPlayer_command_delay();
	mp3_set_EQ(EQUALIZER % (MAXEQUALIZER + 1));
	
	DFPlayer_command_delay();
}

void InitPins(void)
{
	pinMode(DFPlayer_busy_pin, INPUT);
	pinMode(BUTTON1_pin, INPUT);
	pinMode(BUTTON2_pin, INPUT);
	pinMode(BUTTON3_pin, INPUT);
	pinMode(RELAY1_pin, OUTPUT);
	pinMode(RELAY2_pin, OUTPUT);
	pinMode(RELAY3_pin, OUTPUT);
	pinMode(BUZZER_pin, OUTPUT);
	digitalWrite(RELAY1_pin, LOW);
	digitalWrite(RELAY2_pin, LOW);
	digitalWrite(RELAY3_pin, LOW);
	digitalWrite(BUZZER_pin, LOW);
}

boolean ButtonPressed(const byte BUTTON_pin)
{
	if(digitalRead(BUTTON_pin) == HIGH)
	{
		delay(DEBOUNCE_TIME);
		if(digitalRead(BUTTON_pin) == HIGH)
			return true;
	}
	else
		return false;
}


void DFPlayer_command_delay(void)
{
	delay(MP3_COMMAND_DELAY);
}

void PlayInstructionMusic(void)
{
	DFPlayer_command_delay();
	mp3_play(MP3_1);
	DFPlayer_command_delay();
	while( digitalRead(DFPlayer_busy_pin) == LOW)
		;	// wait until instruction sound ends
}


void WaitButton1Press(void)
{
	DFPlayer_command_delay();
	mp3_play(MP3_2);
	while( ButtonPressed(BUTTON1_pin) != true )
		;
	DFPlayer_command_delay();
	mp3_play(MP3_3);
	digitalWrite(RELAY1_pin, HIGH);
	delay(RELAY1_ON_TIME);
	digitalWrite(RELAY1_pin, LOW);
}

void WaitButton2Press(void)
{
	while( ButtonPressed(BUTTON2_pin) != true )
		;
	DFPlayer_command_delay();
	mp3_play(MP3_2);
}

void WaitButton3Press(void)
{
	while( ButtonPressed(BUTTON3_pin) != true )
		;
	DFPlayer_command_delay();
	mp3_play(MP3_4);
	digitalWrite(BUZZER_pin, HIGH);
	digitalWrite(RELAY2_pin, HIGH);
}

void HandleKeyboard(void)
{
	while(1)
	{
		char key = keypad.getKey();	// get key
		
		update_buffer_and_check(key);	// note function may be blocking if LOCK_AFTER_FINAL_SEQUENCE is true
	}
}

void buzzer_beep(unsigned long int duration)
{
	digitalWrite(BUZZER_pin, LOW);
	delay(duration);
	digitalWrite(BUZZER_pin, HIGH);
}


void final_sequence()
{
	digitalWrite(RELAY2_pin, LOW);
	DFPlayer_command_delay();
	mp3_play(MP3_5);
	digitalWrite(RELAY3_pin, HIGH);
	
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





















