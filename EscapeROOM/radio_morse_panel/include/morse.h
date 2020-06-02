#ifndef MORSE_H
#define MORSE_H

#define DOT_DURATION 150

void playUnit();
void playDot();
void playDash();
void playShortSpace();
void playLetterSpace();
void playWordSpace();
char* findMorseChar (char letter);
void playMorseLetter (char letter);
void playMorsePhrase (const char* morsePhraseToPlay);


#endif // MORSE_H

