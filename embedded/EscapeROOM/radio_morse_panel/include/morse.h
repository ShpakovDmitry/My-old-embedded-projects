#ifndef MORSE_H
#define MORSE_H

#define DOT_DURATION_JIFFIES 15000UL
#define CARRIER_CYCLE_DURATION_JIFFIES 100UL

char* findMorseChar (char letter);
void playMorseLetter (char letter);
void playMorsePhrase (const char* morsePhraseToPlay);


#endif // MORSE_H

