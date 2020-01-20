#include <avr/io.h>
#include <util/delay_.h>

#include "scoring_system.h"


void main(void)
{
	unsigned char data[packet_length];
	
	configure_system_master();		// настройка ведущего
	
	while(1)
	{
		listen_server_for_data(data);			// ждём данных от сервера
		if( send_data_to_slave(data) == SEND_OK)	// если передача данных ведомому прошла успешно, то ждём от него ответа
		{
			listen_slave_for_data(data);
		}
		send_data_to_server(data);			// отсылаем полученные данные на сервер
	}
	
}
