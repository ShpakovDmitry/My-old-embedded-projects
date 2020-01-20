// определения относящиеся к 7-ми сегментному индикатору
#define D1 PD7
#define D2 PD6
#define D3 PD5
#define D4 PD4


#define LED_A PA7
#define LED_B PA5
#define LED_C PA1
#define LED_D PA3
#define LED_E PA4
#define LED_F PA6
#define LED_G PA0
#define LED_DP PA2

#define zero  (1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D) | (1 << LED_E) | (1 << LED_F)
#define one   (1 << LED_B) | (1 << LED_C)
#define two   (1 << LED_A) | (1 << LED_B) | (1 << LED_D) | (1 << LED_E) | (1 << LED_G)
#define three (1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D) | (1 << LED_G)
#define four  (1 << LED_B) | (1 << LED_C) | (1 << LED_F) | (1 << LED_G)
#define five  (1 << LED_A) | (1 << LED_C) | (1 << LED_D) | (1 << LED_F) | (1 << LED_G)
#define six   (1 << LED_A) | (1 << LED_C) | (1 << LED_D) | (1 << LED_E) | (1 << LED_F) | (1 << LED_G)
#define seven (1 << LED_A) | (1 << LED_B) | (1 << LED_C)
#define eight (1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D) | (1 << LED_E) | (1 << LED_F) | (1 << LED_G)
#define nine  (1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D) | (1 << LED_F) | (1 << LED_G)

const digits[] = {zero, one, two, three, four, five, six, seven, eight, nine};

/*==============================Определения функций=======================================*/
void init_LED_indicator(void);					// инициализация индикатора
void LED_indicator(unsigned char x1, unsigned char x2);		// отображение чисел x1 и x2 на индикаторе



/*==============================Реализация функций=======================================*/
void init_LED_indicator(void)
{
	DDRD |= (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4);
	DDRA |= (1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D) | (1 << LED_E) | (1 << LED_F) | (1 << LED_G) | (1 << LED_DP); // 0xff
}

void LED_indicator(unsigned char x1, unsigned char x2)
{
	static unsigned char i = 0;
	
	if(i == 0)	// вывод 1-ой цифры на индикатор
	{
		PORTD &= ~( (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) );
		PORTA = digits[(x1 % 100) / 10];
		PORTD |= (1 << D1);
	}
	else if(i == 1)	// вывод 2-ой цифры на индикатор
	{
		PORTD &= ~( (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) );
		PORTA = digits[(x1 % 100) % 10];
		PORTA |= (1 << LED_DP);
		PORTD |= (1 << D2);
	}
	else if(i == 2)	// вывод 3-ей цифры на индикатор
	{
		PORTD &= ~( (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) );
		PORTA = digits[(x2 % 100) / 10];
		PORTD |= (1 << D3);
	}
	else if(i == 3)	// вывод 4-ой цифры на индикатор
	{
		PORTD &= ~( (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) );
		PORTA = digits[(x2 % 100) % 10];
		PORTD |= (1 << D4);
	}
	i++;
	if(i > 3)
	{
		i = 0;
	}
	
}


