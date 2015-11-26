/**
Ollie Langhorst
Robotics Research for Dr. Krauss
9/21/2015
*/

int marker = 0;
unsigned char  n; 
int fresh = 0;

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
        fresh = 1;
	switch (marker)	{
	case 0:
		n = SPDR;
		marker++;
		fresh = 1;
	break;
	case 1:
		SPDR = n*n;
		marker=0;
		fresh = 1;
	break;           
	}
}

void loop (void){
  if (fresh > 0){
      fresh = 0;
      Serial.print("in fresh");
      Serial.print(SPDR);
      Serial.print("\n");
    }
  delay(10);
}  
