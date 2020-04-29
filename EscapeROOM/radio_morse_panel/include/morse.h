#ifndef MORSE_H
#define MORSE_H

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

