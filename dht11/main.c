#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dht.h"

int main( void )
{

	int data[5] = { 0, 0, 0, 0, 0 };
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
	if ( wiringPiSetup() == -1 )
		exit( 1 );
 
	while ( 1 )
	{
		read_dht11_dat(data);
		delay( 3000 ); 
	}
 
	return(0);
}
