#ifndef GPIO_H
#define GPIO_H

#define SET_GPIO_DIR_OUT(dir, bit) ( (dir) |=  (1 << (bit)) )
#define SET_GPIO_DIR_IN(dir, bit)  ( (dir) &= ~(1 << (bit)) )
#define GET_GPIO_INPUT(port, bit)  ( (port) &  (1 << (bit)) )
#define SET_GPIO_HI(port, bit) ( (port) |=  (1 << (bit)) )
#define SET_GPIO_LO(port, bit) ( (port) &= ~(1 << (bit)) )

#endif // GPIO_H
