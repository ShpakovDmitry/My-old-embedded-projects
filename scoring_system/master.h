//*******************MASTER********************//

// функции
void configure_system_master(void);			// настройка устройства
void listen_server_for_data(unsigned char *data);	// получения данных от "сервера"
unsigned char send_data_to_slave(unsigned char *data);	// отправка данных на slave
void listen_slave_for_data(unsigned char *data);	// получения данных от slave
void send_data_to_server(unsigned char *data);		// отправка данных на "сервер"

	// внутренние функции
void no_answer_from_slave(unsigned char *data);		// записать в data информацию о том, что ответа от ведомого не было


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void configure_system_master(void)
{
	_delay_ms(start_up_delay);
	
	configure_master_ports();
	spi_init();
	uart_init();
	power_on_nRF24L01_on_master();
	configure_nRF24L01();
}

//--------------------------------------------------------

void listen_server_for_data(unsigned char *data)
{
	unsigned char i;
	
	for(i = 0; i < packet_length; i++)
	{
		*(data + i) = uart_getc();
	}
}

//--------------------------------------------------------

unsigned char send_data_to_slave(unsigned char *data)
{
	if( nRF24L01_send_data(data) == SEND_OK )
	{
		return SEND_OK;
	}
	else
	{
		return SEND_ERROR;
	}
}

//--------------------------------------------------------

void listen_slave_for_data(unsigned char *data)
{
	unsigned char retry;
	
	retry = 0;
	while( (nRF24L01_data_received() == DATA_NOT_RECEIVED) || (retry < retry_count) )
	{
		retry++;
		_delay_ms(delay_on_retry);
	}
	
	if(retry < retry_count)
	{
		nRF24L01_receive_data(data);
	}
	else
	{
		no_answer_from_slave(data);
	}
}

//--------------------------------------------------------

void send_data_to_server(unsigned char *data)
{
	unsigned char i;
	
	for(i = 0; i < packet_length; i++)
	{
		uart_putc( *(data + i) );
	}
}

//--------------------------------------------------------

void no_answer_from_slave(unsigned char *data)
{
	unsigned char i;
	
	*data = send_information;
	for(i = 1; i < packet_length; i++)
	{
		*(data + i) = slave_not_responding;
	}
}

//--------------------------------------------------------


















