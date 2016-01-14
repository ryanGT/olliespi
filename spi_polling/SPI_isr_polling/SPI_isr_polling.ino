/**
Ollie Langhorst
Robotics Research for Dr. Krauss
9/21/2015
*/

#define isrPin 5

int marker = 0;
unsigned char  n; 

int isrstate=0;

const byte mask = B11111000;
int prescale = 1;


void setup (void){
    Serial.begin(115200);

    pinMode(isrPin, OUTPUT);
    digitalWrite(isrPin, LOW);

    pinMode(MISO, OUTPUT);
    
    //=======================================================
    // set up the Timer2 interrupt to trigger at 100Hz
    //=======================================================
    cli();          // disable global interrupts
    //OCR0A = 124;
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = 30;// = (16*10^6) / (8000*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM21);
    // Set CS21 bit for 8 prescaler
    //  TCCR2B |= (1 << CS21);
    bitSet(TCCR2B, CS22);
    bitSet(TCCR2B, CS21);
    bitSet(TCCR2B, CS20);
    //!//bitClear(TCCR2B, CS20);
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
    sei();// re-enable global interrupts
    //=======================================================


    // initialize digital pin 7 as an output.
    DDRD |= _BV(PD7);

    // Enable SPI
    SPCR |= _BV(SPE);
    // Turn on interupt
    SPCR |= _BV(SPIE);

    Serial.print("SPI ISR polling");
    Serial.print("\n");  
}  

ISR (SPI_STC_vect){
    switch (marker)    {
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
  delay(10);
}  


ISR(TIMER2_COMPA_vect)
{     
  if (isrstate == 0){
     isrstate=1;
     digitalWrite(isrPin, HIGH);
     //digitalWrite(ledpin, HIGH);
  }
  else{
      isrstate=0;
      digitalWrite(isrPin, LOW);
      //digitalWrite(ledpin, LOW);
  }

  /* fresh=1; */
  /* sendindex=0; */
  /* nISR++; */
  /* outbuffer[0] = 1; */
  /* nlsb = (unsigned char)nISR; */
  /* outbuffer[1] = nlsb; */
  /* nmsb = getsecondbyte(nISR); */
  /* outbuffer[2] = nmsb; */
  /* nIn = reassemblebytes(inbuffer[1], inbuffer[2]); */
  /* echo = nIn*nIn; */
  /* elsb = (unsigned char)echo; */
  /* outbuffer[3] = elsb; */
  /* emsb = getsecondbyte(echo); */
  /* outbuffer[4] = emsb; */

  //analogWrite(pwmA, v1);
  //v_out = v1*v1;
  //fresh_out = 1;
  //out_ready = 0;
}
