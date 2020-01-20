#include <avr/io.h>
#include <util/delay.h>

#include "functions.h"


int main(void)
{
	unsigned int last = 0x00, next = 0x00, current_new = 0x00, current_old = 0x00;
	int i;
	
	init_registers();
	
	//Бесконечный цикл
	while(1)
	{
		/*next = get_from_register();
		for(i = 0; i < 8; i++)
		{
			if(current_new & (1 << i))
			{
				if( (last & (1 << i)) >= (next & (1 << i)) )
				{
					current_new |= (1 << i);
				}
				else
				{
					current_new &= ~(1 << i);
				}
			}
			else
			{
				if( (last & (1 << i)) < (next & (1 << i)) )
				{
					current_new |= (1 << i);
				}
				else
				{
					current_new &= ~(1 << i);
				}
			}
		}
		last = next;
		if(current_old != current_new)
		{
			//play_sound( INVALID );
			//play_sound( CORRECT );
			play_sound( BUTTON_PUSH );
		
		}
		current_old = current_new;
		send_to_register(current_new);
		_delay_ms(10);
		*/
		send_to_register(next++);
		_delay_ms(500);
	}
	
	return 0;
}
