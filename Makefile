airMain: airMain.o ssd1331.o pms5003.o dht.o
	gcc -Wall -o airMain airMain.o ssd1331.o pms5003.o dht.o -lwiringPi 
dht.o: ./dht11/dht.c ./dht11/dht.h
	gcc -Wall -c ./dht11/dht.c -lwiringPi
ssd1331.o: ./ssd1331/ssd1331.c ./ssd1331/ssd1331.h
	gcc -Wall -c ./ssd1331/ssd1331.c -lwiringPi
pms5003.o: ./pms5003/pms5003.c ./pms5003/pms5003.h
	gcc -Wall -c ./pms5003/pms5003.c
airMain.o: airMain.c ./ssd1331/ssd1331.h ./pms5003/pms5003.h ./dht11/dht.h
	gcc -Wall -c airMain.c -lwiringPi
clean:
	rm ssd1331.o airMain.o pms5003.o dht.o airMain
