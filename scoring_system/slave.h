//*******************SLAVE*********************//

// глобальные переменные
unsigned char time_summary[time_bytes];
unsigned char time_between_goals[time_bytes];
unsigned int player_one_score;
unsigned int player_two_score;

// функции
void configure_system_slave(void);			// настройка устройства
void listen_master_for_data(unsigned char *data);	// получение данных от master
void decode_data_and_execute(unsigned char *data);	// расшифровка полученной комманды, выполнение соответствующих действий
void send_data_to_master(unsigned char *data);		// отправка данных на master

	// внутренние функции
void configure_interrupts(void);
void unknown_command(unsigned char *data);
void send_score_and_time_to_master(unsigned char *data);
void load_data_to_flash(unsigned char *data);
void change_game_rules(unsigned char *data);
void pause_game(void);
void continue_game(void);
void reset_game(void);
void change_score_and_time(unsigned char *data);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void configure_system_slave(void)
{
	_delay_ms(start_up_delay);
	
	configure_slave_ports();		// "io_port_configuration.h"
	spi_init();				// для обмена данными с nRF24L01 и flash памятью "spi_uart.h"
	uart_init();				// для отладки	"spi_uart.h"
	power_on_nRF24L01_and_flash_on_slave();	// включение питания для nRF24L01 и flash памяти "low_level_functions.h"
	configure_nRF24L01();			// настройка радио приёмопередатчика "nRF24L01.h"
	configure_flash();			// настройка flash памяти "flash.h"
	configure_game();			// настройка игры(чтение правил, ...) "game.h"
	configure_buttons();			// настройка кнопок "buttons.h"
	configure_interrupts();			// настройка прерываний "slave.h"
}

//--------------------------------------------------------

void listen_master_for_data(void)
{
	while(nRF24L01_data_received() == DATA_NOT_RECEIVED)
	{
		_delay_ms(delay_on_retry);
	}
	
	nRF24L01_receive_data(data);
}

//--------------------------------------------------------

void decode_data_and_execute(unsigned char *data)
{
	if((*data) == score_and_time)
	{
		send_score_and_time_to_master(data);
	}
	else
	if((*data) == change_rules)
	{
		change_game_rules(data);
	}
	else
	if((*data) == change_score_and_time)
	{
		change_score_and_time(data);
	}
	else
	if((*data) == pause_game)
	{
		pause_game();
	}
	else
	if((*data) == continue_game)
	{
		continue_game();
	}
	else
	if((*data) == reset_game)
	{
		reset_game();
	}
	else
	if((*data) == load_data_to_flash)
	{
		load_data_to_flash(data);
	}
	else
	{
		unknown_command(data);
	}
	
}

//--------------------------------------------------------

void send_data_to_master(unsigned char *data)
{
	while( nRF24L01_send_data(data) == SEND_ERROR  ) 
	{
		// бесконечно пытаться передавать данные
	}	
}

//--------------------------------------------------------

void unknown_command(unsigned char *data)
{
	unsigned char i;
	
	*data = send_information;
	for(i = 1; i < packet_length; i++)
	{
		*(data + i) = unknown_command;
	}
}

//--------------------------------------------------------

void configure_interrupts(void)
{
	//TODO
}

//--------------------------------------------------------

void send_score_and_time_to_master(unsigned char *data)
{
	*data = 
