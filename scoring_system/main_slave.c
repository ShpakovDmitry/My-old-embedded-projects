#include <avr/io.h>
#include <util/delay_.h>
#include <avr/interrupt.h>

#include "scoring_system.h"


ISR(INT0_vect)	//
{
	handle_sensor1_event();
}

ISR(INT1_vect)
{
	handle_sensor2_event();
}

ISR(TIMER0_COMP_vect)
{
	manage_time();
	manage_LED_indicator();
	manage_audio_speaker();
}

void main(void)
{
	unsigned char data[packet_length];
	
	configure_system_slave();
	
	while(1)
	{
		listen_master_for_data(data);
		decode_data_and_execute(data);
		send_data_to_master(data);
	}
}








