CFLAGS=-c -Wall
OPTIMISATION=-O0

test: test.c fft_func.o fft_math.o fft_other.o fft_data_manipulation.o Makefile
	gcc test.c -o test fft_func.o fft_math.o fft_other.o fft_data_manipulation.o -lm $(OPTIMISATION)

fft_func.o: fft_func.c fft_math.h fft_func.h fft_data_manipulation.h Makefile
	gcc $(CFLAGS) fft_func.c $(OPTIMISATION)
fft_math.o: fft_math.h fft_math.c Makefile
	gcc $(CFLAGS) fft_math.c $(OPTIMISATION)
fft_other.o: fft_other.c fft_other.h fft_math.h fft_func.h Makefile
	gcc $(CFLAGS) fft_other.c $(OPTIMISATION)
fft_data_manipulation.o: fft_data_manipulation.c fft_data_manipulation.h fft_func.h Makefile
	gcc $(CFLAGS) fft_data_manipulation.c $(OPTIMISATION)
	
clean:
	rm test fft_func.o fft_math.o fft_other.o fft_data_manipulation.o
