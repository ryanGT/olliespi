/**
Ollie Langhorst
Robotics Research for Dr. Krauss
9/21/2015
*/

int marker = 0;
unsigned char  n; 

void setup (void){
	Serial.begin(115200);
	Serial.print("SPI SingleByte Loop");
	Serial.print("\n");  

	pinMode(MISO, OUTPUT);
	
	// initialize digital pin 7 as an output.
	DDRD |= _BV(PD7);

	// Enable SPI
	SPCR |= _BV(SPE);
	// Turn on interupt
	SPCR |= _BV(SPIE);
}  

ISR (SPI_STC_vect){
	switch (marker)	{
	case 0:
		n = SPDR;
		marker++;
	break;
	case 1:
		SPDR = n*n;
		marker=0;
	break;           
	}
}

void loop (void){
}  
