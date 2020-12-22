airMain: airMain.o ssd1331.o
	gcc -Wall -o airMain airMain.o ssd1331.o -lwiringPi 
ssd1331.o: ./ssd1331/ssd1331.c ./ssd1331/ssd1331.h
	gcc -Wall -c ./ssd1331/ssd1331.c -lwiringPi
airMain.o: airMain.c ./ssd1331/ssd1331.h 
	gcc -Wall -c airMain.c -lwiringPi
clean:
	rm ssd1331.o airMain.o airMain
