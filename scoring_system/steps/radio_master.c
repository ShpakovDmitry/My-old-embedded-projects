#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "scoring_system.h"
#include "spi_uart.h"
#include "nRF24L01.h"
//#include "eeprom.h"
//#include "LED.h"

// глобальные переменные
unsigned char data[packet_length];	// массив для пакета данных


void LED_indicator(unsigned char x1, unsigned char x2);		// отображение чисел x1 и x2 на индикаторе
unsigned char read_from_eeprom(unsigned char address);		// чтение из eeprom
void write_to_eeprom(unsigned char address, unsigned char x1);	// запись в eeprom
void init_start_game(void);					// считывание макс. счёта; лучшего времени
void init_nRF24L01(void);					// инициализация радио приёмо-передатчика
unsigned char read_register_nRF24L01(const reg);
void write_register_nRF24L01(const reg, unsigned char data);
void receive_data_nRF24L01(unsigned char *dat);
void transmit_data_nRF24L01(unsigned char *dat);
void pwr_up_nRF24L01(void);
void pwr_down_nRF24L01(void);
void enter_receiver_mode_nRF24L01(void);
void enter_transmitter_mode_nRF24L01(void);


unsigned char uart_getc_test(void)
{
	static unsigned int i;

	i++;
	if(i <= 11000)
	{
		return 0xff;
	}
	if(i <= 22000 && i > 11000)
	{
		return 0x00;
	}
	if(i > 22000)
	{
		i = 0;
		return 0x00;
	}
}


void main(void)
{
	unsigned char i, a, j = 0, x = 0;
	_delay_ms(1000);	// для стабилизации напряжения
	uart_init();
	spi_init();
	init_nRF24L01();

	DDRA = 0x03;

	// попробуем определить возможна ли связь с вторым приёмо-передатчиком
	// попытки будут продолжаться пока связь не будет установленна
	enter_transmitter_mode_nRF24L01();
	data[0] = 0xff;	// команда, которую второй мк. игнорирует.
	do
	{
		transmit_data_nRF24L01(data);
		_delay_ms(100);
		a = read_register_nRF24L01(STATUS);
		if(a & (1 << MAX_RT))
		{
			a |= (1 << MAX_RT);
			write_register_nRF24L01(STATUS, a);
			PORTA = 0x01;	// зажигаем красный светодиод(связь не была уснановленна)
			
			write_register_nRF24L01(FLUSH_TX, 0);
		}
	}while( (a & (1 << TX_DS)) == 0);

	// сброс флага прерывания успешной передачи 
	a |= (1 << TX_DS);
	write_register_nRF24L01(STATUS, a);
	write_register_nRF24L01(FLUSH_TX, 0);

	// если связь налаженна, то зажигаем зелёный светодиод
	PORTA = 0x02;
	// вход в бесконечный цикл
	while(1)
	{
		// получаем пакет из 32-х байт от компьютера
		for(i = 0; i < packet_length; i++)
		{
			data[i] = uart_getc();
		}
		
		// отсылаем полученный пакет данных
		enter_transmitter_mode_nRF24L01();
		i = 0;
		do
		{
			transmit_data_nRF24L01(data);
			_delay_ms(10);
			a = read_register_nRF24L01(STATUS);
			if(a & (1 << MAX_RT))
			{
				a |= (1 << MAX_RT);
				write_register_nRF24L01(STATUS, a);
				PORTA = 0x01;	// зажигаем красный светодиод(связь не была уснановленна)
				write_register_nRF24L01(FLUSH_TX, 0);
			}
			i++;
		}while( ((a & (1 << TX_DS)) == 0) && (i < 100)  );
		
		// если передача не удалась, то отослать данные назад с номером ошибки: 
		if((a & (1 << TX_DS)) == 0)
		{
			data[0] = send_information;
			data[1] = slave_not_responding;
			for(i = 0; i < packet_length; i++)
			{
				uart_putc(data[i]);
			}
			write_register_nRF24L01(FLUSH_TX, 0);
		}
		else	// если всётаки удалось передать данные, то слушаем ответный пакет данных
		{
			PORTA = 0x02;	// зажигаем зелёный светодиод
			// сброс флага прерывания TX_DS
			a = read_register_nRF24L01(STATUS);
			a |= (1 << TX_DS);
			write_register_nRF24L01(STATUS, a);
			write_register_nRF24L01(FLUSH_TX, 0);

			// переходим в режим приёмника, для приёма данных
			enter_receiver_mode_nRF24L01();
			
			while( (PINB & (1 << IRQ)) && (i < 100) ) 
			{
				i++;	// пока не появится прерывание передатчика или макс. количество попыток не будет достигнуто
				_delay_ms(10);
			}
			// если прерывание произошло из-за того, что данные пришли, то считать их и отослать
			a = read_register_nRF24L01(STATUS);
			if(a & (1 << RX_DR))
			{
				receive_data_nRF24L01(data);
				// сброс флага прерывания
				a |= (1 << RX_DR);
				write_register_nRF24L01(STATUS, a);
				data[packet_length - 1] = x;
				for(i = 0; i < packet_length; i++)
				{
					uart_putc(data[i]);
				}
				x++;
				write_register_nRF24L01(FLUSH_RX, 0);
			}
			else
			{
				PORTA = 0x01;	// зажигаем красный светодиод
				// если не удалось принять данные, то отсылаем ошибку
				data[0] = send_information;
				data[1] = data_not_received;
				for(i = 0; i < packet_length; i++)
				{
					uart_putc(data[i]);
				}
				write_register_nRF24L01(FLUSH_RX, 0);
			}



		}


	}
	
}



/*========================Реализация функций============================*/
























