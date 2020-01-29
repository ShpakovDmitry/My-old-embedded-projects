SREG = 0x3f
SPH = 0x3e
SPL = 0x3d
OCR0 = 0x3c
GICR = 0x3b
GIFR = 0x3a
TIMSK = 0x39
TIFR = 0x38
SPMCR = 0x37
TWCR = 0x36
MCUCR = 0x35
MCUCSR = 0x34
TCCR0 = 0x33
TCNT0 = 0x32
OSCCAL = 0x31
OCDR = 0x31
SFIOR = 0x30
TCCR1A = 0x2f
TCCR1B = 0x2e
TCNT1H = 0x2d
TCNT1L = 0x2c
OCR1AH = 0x2b
OCR1AL = 0x2a
OCR1BH = 0x29
OCR1BL = 0x28
ICR1H = 0x27
ICR1L = 0x26
TCCR2 = 0x25
TCNT2 = 0x24
OCR2 = 0x23
ASSR = 0x22
WDTCR = 0x21
UBRRH = 0x20
UCSRC = 0x20
EEARH = 0x1f
EEARL = 0x1e
EEDR = 0x1d
EECR = 0x1c
PORTA = 0x1b
DDRA = 0x1a
PINA = 0x19
PORTB = 0x18
DDRB = 0x17
PINB = 0x16
PORTC = 0x15
DDRC = 0x14
PINC = 0x13
PORTD = 0x12
DDRD = 0x11
PIND = 0x10
SPDR = 0x0f
SPSR = 0x0e
SPCR = 0x0d
UDR = 0x0c
UCSRA = 0x0b
UCSRB = 0x0a
UBRRL = 0x09
ACSR = 0x08
ADMUX = 0x07
ADCSRA = 0x06
ADCH = 0x05
ADCL = 0x04
TWDR = 0x03
TWAR = 0x02
TWSR = 0x01
TWBR = 0x00

RAMEND = 0x45f
FLASHEND = 0x3FFF

;Pin names
PA0 = 0
PA1 = 1
PA2 = 2
PA3 = 3
PA4 = 4
PA5 = 5
PA6 = 6
PA7 = 7

PB0 = 0
PB1 = 1
PB2 = 2
PB3 = 3
PB4 = 4
PB5 = 5
PB6 = 6
PB7 = 7

PC0 = 0
PC1 = 1
PC2 = 2
PC3 = 3
PC4 = 4
PC5 = 5
PC6 = 6
PC7 = 7

PD0 = 0
PD1 = 1
PD2 = 2
PD3 = 3
PD4 = 4
PD5 = 5
PD6 = 6
PD7 = 7

 XCK = PB0
  T0 = PB0
  T1 = PB1
INT2 = PB2
AIN0 = PB2
 OC0 = PB3
AIN1 = PB3
  SS = PB4
MOSI = PB5
MISO = PB6
 SCK = PB7

 RXD = PD0
 TXD = PD1
INT0 = PD2
INT1 = PD3
OC1B = PD4
OC1A = PD5
ICP1 = PD6
 OC2 = PD7

  SCL = PC0
  SDA = PC1
  TCK = PC2
  TMS = PC3
  TDO = PC4
  TDI = PC5
TOSC1 = PC6
TOSC2 = PC7

ADC0 = PA0
ADC1 = PA1
ADC2 = PA2
ADC3 = PA3
ADC4 = PA4
ADC5 = PA5
ADC6 = PA6
ADC7 = PA7



;SREG
I = 7
T = 6
H = 5
S = 4
V = 3
N = 2
Z = 1
C = 0

;SPH
SP10 = 2
SP9  = 1
SP8  = 0

;SPL
SP7 = 7
SP6 = 6
SP5 = 5
SP4 = 4
SP3 = 3
SP2 = 2
SP1 = 1
SP0 = 0

;GICR
INT1  = 7
INT0  = 6
INT2  = 5
IVSEL = 1
IVCE  = 0

;GIFR
INTF1 = 7
INTF0 = 6
INTF2 = 5

;TIMSK
OCIE2  = 7
TOIE2  = 6
TICIE1 = 5
OCIE1A = 4
OCIE1B = 3
TOIE1  = 2
OCIE0  = 1
TOIE0  = 0

;TIFR
OCF2  = 7
TOV2  = 6
ICF1  = 5
OCF1A = 4
OCF1B = 3
TOV1  = 2
OCF0  = 1
TOV0  = 0

;SPMCR
SPMIE  = 7
RWWSB  = 6
RWWSRE = 4
BLBSET = 3
PGWRT  = 2
PGERS  = 1
SPMEN  = 0

;TWCR
TWINT = 7
TWEA  = 6
TWSTA = 5
TWSTO = 4
TWWC  = 3
TWEN  = 2
TWIE  = 0

;MCUCR
SM2   = 7
SE    = 6
SM1   = 5
SM0   = 4
ISC11 = 3
ISC10 = 2
ISC01 = 1
ISC00 = 0

;MCUCSR
JDT   = 7
ISC2  = 6
JTRF  = 4
WDRF  = 3
BORF  = 2
EXTRF = 1
PORF  = 0

;TCCR0
FOC0  = 7
WGM00 = 6
COM01 = 5
COM00 = 4
WGM01 = 3
CS02  = 2
CS01  = 1
CS00  = 0

;SFIOR
ADTS2 = 7
ADTS1 = 6
ADTS0 = 5
ACME  = 3
PUD   = 2
PSR2  = 1
PSR10 = 0

;TCCR1A
COM1A1 = 7
COM1A0 = 6
COM1B1 = 5
COM1B0 = 4
FOC1A  = 3
FOC1B  = 2
WGM11  = 1
WGM10  = 0

;TCCR1B
ICNC1 = 7
ICES1 = 6
WGM13 = 4
WGM12 = 3
CS12  = 2
CS11  = 1
CS10  = 0

;TCCR2
FOC2  = 7
WGM20 = 6
COM21 = 5
COM20 = 4
WGM21 = 3
CS22  = 2
CS21  = 1
CS20  = 0

;ASSR
   AS2 = 3
TCN2UB = 2
OCR2UB = 1
TCR2UB = 0

;WDTCR
WDTOE = 4
WDE   = 3
WDP2  = 2
WDP1  = 1
WDP0  = 0

;UBRRH
URSEL  = 7
UBRR11 = 3
UBRR10 = 2
UBRR9  = 1
UBRR8  = 0

;UCSRC
URSEL = 7
UMSEL = 6
UPM1  = 5
UPM0  = 4
USBS  = 3
UCSZ1 = 2
UCSZ0 = 1
UCPOL = 0

;EEARH
EEAR8 = 0

;EECR
EERIE = 3
EEMWE = 2
EEWE  = 1
EERE  = 0

;SPSR
SPIF  = 7
WCOL  = 6
SPI2X = 0

;SPCR
SPIE = 7
SPE  = 6
DORD = 5
MSTR = 4
CPOL = 3
CPHA = 2
SPR1 = 1
SPR0 = 0

;UCSRA
RXC  = 7
TXC  = 6
UDRE = 5
FE   = 4
DOR  = 3
PE   = 2
U2X  = 1
MPCM = 0

;UCSRB
RXCIE = 7
TXCIE = 6
UDRIE = 5
RXEN  = 4
TXEN  = 3
UCSZ2 = 2
RXB8  = 1
TXB8  = 0

;ACSR
ACD   = 7
ACBG  = 6
ACO   = 5
ACI   = 4
ACIE  = 3
ACIC  = 2
ACIS1 = 1
ACIS0 = 0

;ADMUX
REFS1 = 7
REFS0 = 6
ADLAR = 5
MUX4  = 4
MUX3  = 3
MUX2  = 2
MUX1  = 1
MUX0  = 0

;ADCSRA
ADEN  = 7
ADSC  = 6
ADATE = 5
ADIF  = 4
ADIE  = 3
ADPS2 = 2
ADPS1 = 1
ADPS0 = 0

;TWAR
TWA6  = 7
TWA5  = 6
TWA4  = 5
TWA3  = 4
TWA2  = 3
TWA1  = 2
TWA0  = 1
TWGCE = 0

;TWSR
TWS7 = 7
TWS6 = 6
TWS5 = 5
TWS4 = 4
TWS3 = 3
TWS1 = 1
TWS0 = 0













