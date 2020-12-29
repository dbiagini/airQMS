#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./ssd1331/ssd1331.h"

#define BUTTON_PIN 3

#define PM25_LOW 35
#define PM25_HIGH 75

#define PM1_LOW 35
#define PM1_HIGH 75

#define PM10_LOW 50
#define PM10_HIGH 110

char value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int pms[3] = {0,0,0};

volatile int numberOfPresses = 0;

void btnInterrupt(){
	numberOfPresses++;
	numberOfPresses %= 3;
}

int pmsSim(int* values){
   int error = 0;
   if( values!=NULL ){
	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);
	values[0] = rand() % 100;	
	values[1] = rand() % 100;
	values[2] = rand() % 100;
   }
   else
	   error =1;
   return(error);

}

void drawPage(int page){

	time_t now;
    	struct tm *timenow;
	//draw time//
	time(&now);
        timenow = localtime(&now);
	
	char hour[3] = {(char) value[timenow->tm_hour / 10], (char) value[(timenow->tm_hour % 10)], '\0'}; 
	char minutes[3] = {(char) value[timenow->tm_min / 10], (char) value[(timenow->tm_min % 10)], '\0'};
	
	SSD1331_string(0, 0, hour, 12,1, BLUE);
	SSD1331_string(12, 0, " : ", 12, 1, RED);
	SSD1331_string(22, 0, minutes,12, 1, GREEN);

  //color coding values//
  int colors[3] = {BLUE, BLUE, BLUE};
  if ((pms[0] >= PM1_LOW) &&  (pms[0] < PM1_HIGH)) colors[0] = YELLOW;
  else if(pms[0] >= PM1_HIGH) colors[0] = RED;
  if ((pms[1] >= PM25_LOW) &&  (pms[1] < PM25_HIGH)) colors[1] = YELLOW;
  else if(pms[1] >= PM25_HIGH) colors[1] = RED;
  if ((pms[2] >= PM10_LOW) &&  (pms[2] < PM10_HIGH)) colors[2] = YELLOW;
  else if(pms[2] >= PM10_HIGH) colors[2] = RED;

	switch(page){
	
		case 0:

			SSD1331_char1616(0, 13, value[pms[0] / 10], colors[0]);
			SSD1331_char1616(16, 13, value[pms[0] % 10 ], colors[0]);
			SSD1331_string(40, 13, "Pm 1.0", 12, 1, WHITE); 
			//printf("pm 1.0 %d \n", pms[0]);

			SSD1331_char1616(0, 31, value[pms[1] / 10], colors[1]);
			SSD1331_char1616(16, 31, value[pms[1] % 10 ], colors[1]);
			SSD1331_string(40, 31, "Pm 2.5", 12, 1, WHITE); 
			
			SSD1331_char1616(0, 49, value[pms[2] / 10], colors[2]);
			SSD1331_char1616(16, 49, value[pms[2] % 10 ], colors[2]);
			SSD1331_string(40, 49, "Pm 10", 12, 1, WHITE); 
		
			//printf("pm 2.5 %d \n", pms[1]);
			//printf("pm 10 %d \n", pms[2]);

			break;
		case 1:
			//draw pm 2.5//
			SSD1331_char3216(18, 18, value[pms[1] / 10], colors[1]);
			SSD1331_char3216(42, 18, value[pms[1] % 10 ], colors[1]);
			SSD1331_string(18, 49, "ug/m3 Pm 2.5", 12, 1, WHITE); 

			break;
		case 2:
			//draw pm 10.0//
			SSD1331_char3216(18, 18, value[pms[2] / 10], colors[2]);
			SSD1331_char3216(42, 18, value[pms[2] % 10 ], colors[2]);
			SSD1331_string(18, 49, "ug/m3 Pm 10", 12, 1, WHITE); 
			break;
		default:
			break;
	}

}

int main(int argc, char **argv)
{
    if(wiringPiSetup() < 0)
    {
    	printf("Unable to setup Wiring PI\n");
	return -1;
    }
    
    if( wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &btnInterrupt) < 0) {
    	printf("unable to setup ISR for button");
	return -1;
    }
    printf("OLED run Press Ctrl + C to exit.\n");
    SSD1331_begin();
    SSD1331_clear();

    while(1)
    {
        int status = pmsSim(pms);
        //SSD1331_string(0, 52, "MUSIC", 12, 0, WHITE); 
        //SSD1331_string(64, 52, "MENU", 12, 1, WHITE); 
	if(status == 0){
        	//printf("pm 2.5 %d \n", pms[1]);
		//printf("pm 10 %d \n", pms[2]);
		drawPage(numberOfPresses);
		SSD1331_display();
		//printf("%d\n", numberOfPresses);

	} else {
	 return 1;
	}
	delay(1000);
    }

    return 0;
}
