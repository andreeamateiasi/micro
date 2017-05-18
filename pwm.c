#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

void initLCD();
void putchLCD(char ch);
void putsLCD(char* ch);
void gotoLC(char line, char col);
void clrLCD();
char kbscan();
void sysinit();

#define C1 1
#define C2 2
#define C3 3

int main(){
   sysinit();
   
   unsigned char code_now=0x7f, code_ante;
   unsigned char kbhit=0,kbcode, loop_cnt=0;

   int lumi=50;  //luminozitatea
   char buf_lumi[4];

   unsigned char stare = C1;
   char buf_pit[4];
   unsigned long int a,b,c;

   TCCR2 = 0b00001110;
//   OCR2=250-1;
   TIMSK = 1<<OCIE2; // demascheaza intreruperea OCF2 de la timerul 2
//   SREG |= 1<<7; // demascheaza intreruperea INT - se seteaza I in SREG
   setbit(SREG, I);   

   

   while(1){
      if(loop_cnt==200){
         code_ante=code_now;
         code_now = kbscan();
         if(code_now!=0x7f && code_ante==0x7f){
            kbhit=1;
            kbcode=code_now;
         }
         loop_cnt=0;
      }

/*      //PWM start
      if(kbhit  && (kbcode == 'C' || kbcode == 'D')){
         if(kbcode == 'C'){
            //lumi=//...creste luminozitatea
            //...
         }
         if(kbcode == 'D'){
            //lumi=//...scade luminozitatea
            //...
         }
         gotoLC(2,11);
         putsLCD("L=");
         itoa(lumi,buf_lumi,10);
         putsLCD(buf_lumi);
         putsLCD("%  " );
      }
*/      //PWM end

      if(kbhit){
         gotoLC(1,1);
         kbhit=0;
         putchLCD(kbcode);
      }

/*	  //c pitagoreic start
      if( kbhit && kbcode>='0' && kbcode<='9'){
         kbhit=0;

         switch(stare){
         	case C1:   //cifra 0
               //sterge linia 1 a LCD-ului
               buf_pit[0]=kbcode;
               //scrie prima cifra
               stare=C2;
               break;

            case C2:
               buf_pit[1]=kbcode;
               //scrie a doua cifra
               stare=C3;
               break;
         
            case C3:
               buf_pit[2]=kbcode;
               buf_pit[3]='\0';
               //scrie a treia cifra
               //scrie "Busy..." pe linia 1, din coloana 5
               //calculeaza c

               for(a = 1;...){
                  for(b = 1...){
                  //...verifica daca a^2 + b^2 = c^2
                  }
               }
               
               gotoLC(1,5);
      		   if(...){
      		      putsLCD("       PYTH");
      		   }
      		   else{
      		      putsLCD("      NPYTH");
      		   }
               stare=C1;
               break;
         }//end switch     
      }
*/       // end c pitagoreic      

      kbhit=0;
      loop_cnt++;
   }//end while
}//end main

ISR(TIMER2_COMP_vect){
   static char cycles=0; 
   static char sec = 50, min = 59, h = 23; 
   static char timeDisplay=0;

   cycles++;
 
   if(cycles==k){
      cycles=0;
      sec++;
      if(sec == 60){
         sec = 0; 
         min++;
         if(min == 60){
            min = 0;
            h++;
            if(h == 24){
               h = 0;
            }
          }	
       }
    
       //sprintf(timeDisplay,"%2d:%02d:%02d", h, min, sec);
       gotoLC(1,1);
       if(h > 9){
          timeDisplay = h/10 + '0';
          putchLCD(timeDisplay);
				
       }
	else{
           putchLCD(' ');
	  //timeDisplay = h + '0';
   	  //putchLCD(timeDisplay);
	}
        timeDisplay = h % 10 + '0';
        putchLCD(timeDisplay);
	putchLCD(':');
	timeDisplay = min / 10 + '0';
	putchLCD(timeDisplay);
	timeDisplay = min % 10 + '0';
	putchLCD(timeDisplay);
	putchLCD(':');
	timeDisplay = sec / 10 + '0';
	putchLCD(timeDisplay);
	timeDisplay = sec % 10 + '0';
	putchLCD(timeDisplay);
				
        if(TIFR & 1<<TOV2){
     	   putsLCD("ERROR!");
   	   while(1){}
	}
	         
         //putsLCD(timeDisplay);
	      
    }


}//end ISR

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




















