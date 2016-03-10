/**
* Ollie Langhorst
* Robotics Research for Dr. Krauss
* 1/7/2016
*/

#include <SPI.h>
#define receivelen 5
#define sendlen 5
#define isrPin 7
#define spiPin 6

const byte mask = B11111000;
int prescale = 1;

int n;
int nIn;
int nISR;
int nspiq;
int v1;
int v_out;
int echo;
unsigned char nlsb, nmsb, vlsb, vmsb, elsb, emsb;

int isrstate=0;
int spistate = 0;
int byte_counter;
int fresh;

uint8_t inbuffer [receivelen];
uint8_t outbuffer [sendlen];

void setup (void){
  nISR = 0;
  nspiq = 0;
  fresh = 0;
  byte_counter = 0;
  
  Serial.begin(115200);
  Serial.print("spi_polling_two_isrs from gmail 3/8/16 5:31PM");
  Serial.print("\n");
  
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  pinMode(isrPin, OUTPUT);
  digitalWrite(isrPin, LOW);

  pinMode(spiPin, OUTPUT);
  digitalWrite(spiPin, LOW);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // turn on SPI interrupt
  SPCR |= _BV(SPIE);
 
     //=======================================================
    // set up the Timer2 interrupt to trigger at 100Hz
    //=======================================================
    cli();          // disable global interrupts
    //OCR0A = 124;
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 8khz increments
    OCR2A = 40;// = (16*10^6) / (8000*8) - 1 (must be <256)
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
}

ISR (SPI_STC_vect){
  // - respond with 0 until fresh happens
  // - once fresh happens, start the real data transmission
  // ? when does byte_counter get reset ?
  //digitalWrite(spiPin, HIGH);
  //digitalWrite(spiPin, HIGH);

  SPDR = 7;
  nspiq++;
  
  /* if ( fresh == 0){ */
  /*   SPDR = 0; */
  /*   byte_counter = 0; */
  /*   nspiq++; */
  /* } */
  /* else { */
  /*   //SPDR = outbuffer[byte_counter]; */
  /*   SPDR = byte_counter; */
  /*   byte_counter++; */
  /*   if (byte_counter >= 3){ */
  /*     fresh = 0; */
  /*     byte_counter = 0; */
  /*     nspiq = 0; */
  /*   } */
  /* } */
  
    //SPDR = nspiq;
  //digitalWrite(spiPin, LOW);
  //------------------------
  // Debugging 3/7/16
  //------------------------
   if (spistate == 0){
      spistate=1;
      digitalWrite(spiPin, HIGH);
    }
    else{
      spistate=0;
      digitalWrite(spiPin, LOW);
    }
  /* if (fresh == 0){ */
  /*   //SPDR = fresh; */
  /*   SPDR = nspiq; */
  /*   nspiq++; */
  /*   byte_counter = 0; */
  /* } */
  /* else{ */
  /*   // leave fresh at 1 until we are done sending sendlen bytes */
  /*   inbuffer[byte_counter] = SPDR; */
  /*   Serial.print("SPI ISR"); */
  /*   Serial.print("\n"); */
  /*   SPDR = outbuffer[byte_counter]; */
  /*   byte_counter++; */
  /*   if (byte_counter >= sendlen){ */
  /*     fresh = 0; */
  /*     byte_counter = 0; */
  /*   } */
  /*   nspiq = 0; */
  /* } */
  //digitalWrite(spiPin, LOW);
}

void loop (void){
  delay(10);
}  


/* void loop (void){ */
/*   SPDR = fresh; */
/*   if (fresh == 1){ */
/*     //~ if (isrstate == 0){ */
/*       //~ isrstate=1; */
/*       //~ digitalWrite(isrPin, HIGH); */
/*     //~ } */
/*     //~ else{ */
/*       //~ isrstate=0; */
/*       //~ digitalWrite(isrPin, LOW); */
/*     //~ } */
/*     //~ if (nISR>0){ */
/*       //~ Serial.print(inbuffer[1]); */
/*       //~ Serial.print("\n"); */
/*     //~ } */
        
/*     if((SPSR & (1 << SPIF)) != 0)  { // Check to see if the 8th bit of a byte is received via spi */
/*       inbuffer[byte_counter] = SPDR; */
/*       //~ Serial.print(SPDR); */
/*       //~ Serial.print("\n"); */
/*       SPDR = outbuffer[byte_counter]; */
/*       byte_counter++; */
/*     } */
/*   fresh = 0; */
/*   } */
/*   byte_counter = 0; */
/* } */

ISR(TIMER2_COMPA_vect){     
    if (isrstate == 0){
      isrstate=1;
      digitalWrite(isrPin, HIGH);
    }
    else{
      isrstate=0;
      digitalWrite(isrPin, LOW);
    }

  fresh=1;
  nISR++;
  outbuffer[0] = 1;
  nlsb = (unsigned char)nISR;
  outbuffer[1] = nlsb;
  nmsb = getsecondbyte(nISR);
  outbuffer[2] = nmsb;
  nIn = reassemblebytes(inbuffer[1], inbuffer[2]);
  echo = nIn*nIn;
  elsb = (unsigned char)echo;
  outbuffer[3] = elsb;
  emsb = getsecondbyte(echo);
  outbuffer[4] = emsb;
}

unsigned char getsecondbyte(int input){
    unsigned char output;
    output = (unsigned char)(input >> 8);
    return output;
}

int reassemblebytes(unsigned char msb, unsigned char lsb){
    int output;
    output = (int)(msb << 8);
    output += lsb;
    return output;
}


