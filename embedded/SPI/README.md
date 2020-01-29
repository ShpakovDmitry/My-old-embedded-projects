Small source file for SPI interface init and transceicve routine.

Supports only for those AVR uC which has SPI interface

Includes:

```c
void spi_init(void);
unsigned char spi_transaction(char c); // Reads and sends byte simultaneously
```
