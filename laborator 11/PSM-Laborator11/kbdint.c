#include <avr/io.h>
#include <avr/interrupt.h>

#define NOKEY 0x7f
#define DELAY 200

void initLCD();
void putchLCD( char ch);
void putsLCD ( char* ch);
void clrLCD ();
void gotoLC (unsigned char line, unsigned char col);
char kbscan();

volatile unsigned long int delay;
volatile unsigned char kbhit = 0;
volatile char kbcode;

int main(){ 
   //initializari pentru LCD
   PORTB=0;
   DDRB=0b11100000;
   DDRD=0xff;
   initLCD();
   // initializari pentru tastatura
   PORTA = 0;
   
   TCCR0 = 0b01001100; // timerul 0
   TIMSK = 1<<TOIE0; // demascheaza intreruperea TOV de la timerul 0
   SREG |= 1<<7; // demascheaza intreruperea INT - se seteaza I in SREG
   
   while(1){ //bucla principala   
      if(kbhit){
	 if((kbcode>='0')&&(kbcode<='9')){
	    kbhit=0;
	    gotoLC(1,1);
	    putchLCD (kbcode);	    
	 }
	 else if(kbcode=='#'){
	    kbhit=0;
	    gotoLC(1,1);
	    putsLCD("                ");
	    gotoLC(1,1);
	    putsLCD ("Calculez...");
//	    for(delay=0; delay<150000; delay++){} // 1 sec
	    for(delay=0;delay<600000UL;delay++){} // 4 sec
	    clrLCD();
	 }
	 else
	    kbhit=0;
      }
   }//end while
}

ISR(TIMER0_OVF_vect){
   char code_ante = NOKEY;
   static char code_now = NOKEY;
   code_ante = code_now;
   code_now = kbscan();
   if( code_ante == NOKEY && code_now != NOKEY){
      kbhit=1;
      kbcode=code_now;
   }
} // end ISR