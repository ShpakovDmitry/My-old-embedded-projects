void configure_master_ports(void);
void configure_slave_ports(void);

// MASTER
#define master_nRF24L01_DDR			DDRB
#define master_nRF24L01_CE_port			PORTB
#define master_nRF24L01_CSN_port		PORTB
#define master_nRF24L01_IRQ_port		PORTB
#define master_nRF24L01_POWER_port		PORTB

#define master_nRF24L01_CE_pin			PB0
#define master_nRF24L01_CSN_pin			PB1
#define master_nRF24L01_IRQ_pin			PB2
#define master_nRF24L01_POWER_pin		PB3


// SLAVE
	// PORTA
#define slave_LED_indicator_DDR			DDRA
#define slave_LED_indicator_A_port		PORTA
#define slave_LED_indicator_B_port		PORTA
#define slave_LED_indicator_C_port		PORTA
#define slave_LED_indicator_D_port		PORTA
#define slave_LED_indicator_E_port		PORTA
#define slave_LED_indicator_F_port		PORTA
#define slave_LED_indicator_G_port		PORTA

#define slave_LED_indicator_A_pin		PA0
#define slave_LED_indicator_B_pin		PA1
#define slave_LED_indicator_C_pin		PA2
#define slave_LED_indicator_D_pin		PA3
#define slave_LED_indicator_E_pin		PA4
#define slave_LED_indicator_F_pin		PA5
#define slave_LED_indicator_G_pin		PA6

#define slave_button_DDR			DDRA
#define slave_button_PORT			PORTA

#define slave_button_pin			PA7

	// PORTB
#define slave_nRF24L01_DDR			DDRB
#define slave_nRF24L01_CE_port			PORTB
#define slave_nRF24L01_CSN_port			PORTB
#define slave_nRF24L01_IRQ_port			PORTB
#define slave_nRF24L01_and_flash_POWER_port	PORTB

#define slave_nRF24L01_CE_pin			PB0
#define slave_nRF24L01_CSN_pin			PB1
#define slave_nRF24L01_IRQ_pin			PB2
#define slave_nRF24L01_and_flash_POWER_pin	PB3

#define slave_flash_DDR				DDRB
#define slave_flash_CSN_port			PORTB

#define slave_flash_CSN_pin			PB4

	// PORTC
#define slave_speaker_DDR			DDRC
#define slave_speaker_PORT			PORTC

#define slave_speaker_1_pin			PC0
#define slave_speaker_2_pin			PC1
#define slave_speaker_4_pin			PC2
#define slave_speaker_8_pin			PC3
#define slave_speaker_16_pin			PC4
#define slave_speaker_32_pin			PC5
#define slave_speaker_64_pin			PC6
#define slave_speaker_128_pin			PC7

	// PORTD
#define slave_LED_indicator_digit_DDR		DDRD
#define slave_LED_indicator_digit_PORT		PORTD

#define slave_LED_indicator_digit1_pin		PD4
#define slave_LED_indicator_digit2_pin		PD5
#define slave_LED_indicator_digit3_pin		PD6
#define slave_LED_indicator_digit4_pin		PD7







//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void configure_master_ports(void)
{
	// установка входов в МК для радио приёмопередатчика
	master_nRF24L01_DDR &= ~(1 <<  master_nRF24L01_IRQ_pin);
	
	// установка выходов из МК для радио приёмопередатчика
	master_nRF24L01_DDR |= (1 << master_nRF24L01_CE_pin) | (1 <<  master_nRF24L01_CSN_pin) | (1 << master_nRF24L01_POWER_pin);
	
}

//--------------------------------------------------------

void configure_slave_ports(void)
{
	// установка входов в МК для радио приёмопередатчика
	slave_nRF24L01_DDR &= ~(1 <<  slave_nRF24L01_IRQ_pin);
	
	// установка выходов из МК для радио приёмопередатчика
	slave_nRF24L01_DDR |= (1 << slave_nRF24L01_CE_pin) | (1 <<  slave_nRF24L01_CSN_pin) | (1 << slave_nRF24L01_and_flash_POWER_pin);

	
	// установка выходов из МК для flash памяти
	slave_flash_DDR |= (1 << slave_flash_CSN_pin);
	
	
	// установка выходов из МК для громкоговорителя, все порты являются выходами
	slave_speaker_DDR |=		(1 << slave_speaker_1_pin)	| \
					(1 << slave_speaker_2_pin)	| \
					(1 << slave_speaker_4_pin)	| \
					(1 << slave_speaker_8_pin)	| \
					(1 << slave_speaker_16_pin)	| \
					(1 << slave_speaker_32_pin)	| \
					(1 << slave_speaker_64_pin)	| \
					(1 << slave_speaker_128_pin);
	
	// установка выходов из МК для LED индикатора, все порты являются выходами
	slave_LED_indicator_DDR |=	(1 << slave_LED_indicator_A_pin) | \
					(1 << slave_LED_indicator_B_pin) | \
					(1 << slave_LED_indicator_C_pin) | \
					(1 << slave_LED_indicator_D_pin) | \
					(1 << slave_LED_indicator_E_pin) | \
					(1 << slave_LED_indicator_F_pin) | \
					(1 << slave_LED_indicator_G_pin);
	
	// установка выходов из МК для LED индикатора(цифры), все порты являются выходами
	slave_LED_indicator_digit_DDR |=	(1 << slave_LED_indicator_digit1_pin) | \
						(1 << slave_LED_indicator_digit2_pin) | \
						(1 << slave_LED_indicator_digit3_pin) | \
						(1 << slave_LED_indicator_digit4_pin);
	
	
	// установка входов в МК для кнопки
	slave_button_DDR &= ~(1 << slave_button_pin);
}

//--------------------------------------------------------










