//*******************MASTER********************//

void power_on_nRF24L01_on_master(void)

//*******************SLAVE*********************//

void power_on_nRF24L01_and_flash_on_slave(void)
void manage_time(void);





//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++MASTER

void power_on_nRF24L01_on_master(void)
{
	master_nRF24L01_POWER_PORT |= (1 << master_nRF24L01_POWER_pin);
	_delay_ms(start_up_delay);
}

//--------------------------------------------------------



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++SLAVE

void power_on_nRF24L01_and_flash_on_slave(void)
{
	slave_nRF24L01_and_flash_POWER_PORT |= (1 << slave_nRF24L01_and_flash_POWER_pin);
	_delay_ms(start_up_delay);
}

//--------------------------------------------------------

