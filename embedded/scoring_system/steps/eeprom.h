// определения относящиеся к eeprom
#define max_c1_addr	0x00
#define max_c2_addr	0x01

/*==============================Определения функций=======================================*/

unsigned char read_from_eeprom(unsigned char address);		// чтение из eeprom
void write_to_eeprom(unsigned char address, unsigned char x1);	// запись в eeprom




/*==============================Реализация функций=======================================*/
void write_to_eeprom(unsigned char address, unsigned char data)
{
	// ждать завершение записи
	while(EECR & (1 << EEWE))
	{
		;
	}
	// загружаем адрес записи
	EEARH = 0x00;
	EEARL = address;
	
	// загружаемые данные
	EEDR = data;

	// запись данных
	EECR |= (1 << EEMWE);
	EECR |= (1 <<EEWE);

}

unsigned char read_from_eeprom(unsigned char address)
{
	// ждать завершение записи
	while(EECR & (1 << EEWE))
	{
		;
	}

	// загружаем адрес записи
	EEARH = 0x00;
	EEARL = address;

	// начинаем чтение
	EECR |= (1 << EERE);

	// возвращаем считанные данные
	return EEDR;
}


