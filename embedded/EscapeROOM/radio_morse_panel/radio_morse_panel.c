#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CARRIER_ON	(1 << PD5)
#define CARRIER_OFF	(0 << PD5)
#define DOT_DURATION 150
#define MAX_REPEAT 5
#define RED_LED PD3
#define GREEN_LED PD4
#define MAX_MORSE_PARTS 5	// according to ITU maximal parts in one letter is five ex. '2' = "..---"

// simply change the phrase to needed one, length is limited by flash memory size
const char MorsePhraseToPlay[] = "472";

typedef struct{
	char Symbol;
	char Code[MAX_MORSE_PARTS];
}MorseCode;

// Morse alphabet set; could be added another alphabet codes
const MorseCode morseCode[] = { 
	{'0', "-----"},
	{'1', ".----"},
	{'2', "..---"},
	{'3', "...--"},
	{'4', "....-"},
	{'5', "....."},
	{'6', "-...."},
	{'7', "--..."},
	{'8', "---.."},
	{'9', "----."}
};

// get number of elements in our morse alphabet set
const unsigned char NumOfMorseCodeElements = sizeof(morseCode) / sizeof(morseCode[0]);

unsigned char carrier = CARRIER_OFF;

void PlayUnit()
{
	unsigned char i;
	
	for(i = 0; i < DOT_DURATION; i++)
	{
		PORTD ^= ((1 << PD5) & carrier);
		_delay_ms(1);
	}
}

void PlayDot()
{
	carrier = CARRIER_ON;
	PlayUnit();
	carrier = CARRIER_OFF;
}

void PlayDash()
{
	carrier = CARRIER_ON;
	PlayUnit(); PlayUnit(); PlayUnit();
	carrier = CARRIER_OFF;
}

void PlayShortSpace()
{
	PlayUnit();
}

void PlayLetterSpace()
{
	PlayUnit(); PlayUnit(); PlayUnit();
}

void PlayWordSpace()
{
	PlayUnit(); PlayUnit(); PlayUnit(); PlayUnit(); PlayUnit(); PlayUnit(); PlayUnit();
}

unsigned char GetLetterPositionInMorseAlphabet(char Letter, const MorseCode *MorseAlphabet, const unsigned char NumOfElements){
	unsigned char LetterPosition;
	
	// simply compare each element in alphabet;
	for(LetterPosition = 0; LetterPosition < NumOfElements; LetterPosition++){
		if( (MorseAlphabet + LetterPosition)->Symbol == Letter)
			break;
	}
	
	return LetterPosition;
}

void PlayMorseLetter(char Letter, const MorseCode *MorseAlphabet, const unsigned char NumOfElements){
	unsigned char LetterPositionInAlphabet;
	unsigned char i;
	
	LetterPositionInAlphabet = GetLetterPositionInMorseAlphabet(Letter, MorseAlphabet, NumOfElements);
	
	// play each morse letter code element 
	for(i = 0; i < MAX_MORSE_PARTS && (MorseAlphabet + LetterPositionInAlphabet)->Code[i] != '\0'; i++){
		switch( (MorseAlphabet + LetterPositionInAlphabet)->Code[i] ){
			case '.' :
				PlayDot();
				break;
			case '-' :
				PlayDash();
				break;
		}
		PlayShortSpace();
	}
	
	return;
}

void PlayMorsePhrase(const char *MorsePhraseToPlay, const MorseCode *MorseAlphabet, const unsigned char NumOfElements){
		unsigned char i;
		
		// play each letter until 'MorsePhraseToPlay' end is not reached
		for(i = 0; MorsePhraseToPlay[i] != '\0'; i++){
			PlayMorseLetter(MorsePhraseToPlay[i], MorseAlphabet, NumOfElements);
			PlayLetterSpace();
		}
		
		PlayWordSpace();
		
		return;
}


unsigned char play_flag = 0;
unsigned char button = 0;

unsigned char button1 = ~(1 << PB6);
unsigned char button2 = ~(1 << PB1);
unsigned char button3 = ~(1 << PB4);

ISR(PCINT_vect)
{
	static unsigned char i = 0;
	unsigned char data = PINB;
	
	// delay while noise finishes and clear interrupt flag
	// to do not cause extra interrupt
	_delay_ms(500);
	EIFR |= 1 << PCIF;
	
	if(data == 0xff)
	{
		return;
	}
	if(!(data & 0x01) || !(data & 0x02) || !(data & 0x04) || !(data & 0x08) || \
	   !(data & 0x10) || !(data & 0x20) || !(data & 0x40) || !(data & 0x80))
	{
			if(i == 0)
			{
				PORTD &= ~( (1 << RED_LED) | (1 << GREEN_LED) );
				if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 1)
			{
				if(data == button2)
				{
					i = 2;
				}
				else if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 2)
			{
				if(data == button3)
				{
					play_flag = 1;
					i = 0;
					PORTD |= (1 << GREEN_LED);
				}
				else if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
					return;
				}
			}
			
	}
	else
	{
		i = 0;
	}
	
	if(play_flag)
	{
		play_flag = MAX_REPEAT;
		for(;play_flag > 0; play_flag--)
		{
			PlayMorsePhrase(MorsePhraseToPlay, morseCode, NumOfMorseCodeElements);
			
		}
	}
}


void main()
{
	PORTD &= ~( (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED) );
	DDRD |= (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED);
	PORTB = 0xff;
	DDRB = 0x00;
	unsigned char k;
	
	PCMSK = (1 << PCINT7) | (1 << PCINT6) | (1 << PCINT5) | (1 << PCINT4) | \
		 (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1) | (1 << PCINT0);
	GIMSK |= (1 << PCIE);
	SREG |= (1 << 7);
	
	while(1)
	{
		;
	}
}







