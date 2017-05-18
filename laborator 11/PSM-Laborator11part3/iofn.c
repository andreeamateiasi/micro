#include <avr/io.h>

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

#define E  7
#define RW 6
#define RS 5

#define IReg 0
#define SReg 0
#define DReg 1

#define nop asm("nop"::);

unsigned char rd_LCDreg(unsigned char vRS){
   unsigned char data;
   DDRD=0;
   if(vRS==0)
      clrbit(PORTB,RS); // RS=0
   else
      setbit(PORTB,RS); // RS=1
   setbit(PORTB,RW); // RW=1
   nop;
   setbit(PORTB,E); // E=1
   nop; nop; nop; nop; nop; nop; 
   data=PIND;
   clrbit(PORTB,E); // E=0
   nop; nop; nop; nop; nop;
   DDRD=0xff;
   return data;
}

void wr_LCDreg(unsigned char vRS, unsigned char data){
   DDRD=0xff;
   PORTD=data;
   if(vRS==0)
      clrbit(PORTB,RS); // RS=0
   else
      setbit(PORTB,RS); // RS=1
   clrbit(PORTB,RW); // RW=0
   nop;
   setbit(PORTB,E); // E=1
   nop; nop; nop; nop;
   clrbit(PORTB,E); // E=0
   nop; nop; nop; nop; nop; nop; 
}

void ckbf(){
   while(rd_LCDreg(SReg) & 1<<7){}
}

void initLCD(){
	ckbf();
   // Function set
	//bit 7 6 5 4   3 2 1 0
	//    0 0 1 DL  N F — — 
	//    0 0 1  1  1 1 0 0
	wr_LCDreg(IReg,0x3C);

	ckbf();
   // entry mode set
	//bit 7 6 5 4   3 2  1  0
	//    0 0 0 0   0 1 I/D S
	//    0 0 0 0   0 1  1  0
	wr_LCDreg(IReg,0x6);

	ckbf();
   // Display on/off control
	//bit 7 6 5 4   3 2 1 0
	//    0 0 0 0   1 D C B    
	//    0 0 0 0   1 1 1 1
	wr_LCDreg(IReg,0xF);

	ckbf();
   //Clear display
	wr_LCDreg(IReg,0x01);

}

void putchLCD(char ch){
   ckbf();
   wr_LCDreg(DReg,ch);
}

void putsLCD(char* ch){
   ckbf();
   while(*ch){
      putchLCD(*ch++);
   }
}

void clrLCD(){
   ckbf();
   wr_LCDreg(IReg,0x01);
}

void gotoLC(unsigned char line, unsigned char col){
   ckbf();
   if((line==1||line==2)&&(col>=1&&col<=16))
      wr_LCDreg(IReg,0x80 + 0x40*(line-1) + (col-1));      
}

char kbscan(){
   unsigned char vDDRA;
   unsigned char cols;
   unsigned char La=0, Ca=0;
   unsigned char cod_intern;
   char cod_extern;
   char tabela_trans[]="123A456B789C*0#D";
   for( vDDRA = 1<<4; vDDRA > 0; vDDRA <<=1 ) {
      DDRA=vDDRA;
      nop; nop; nop; nop;
      cols=~PINA;
      cols=cols & 0b00001111;
      if(cols!=0)
	 break;
   }//end for
   if(vDDRA!=0){     
/*   
      switch(vDDRA){
	 case 0b00010000:{
	    La=0;
	    break;
	 }   
	 case 0b00100000:{
	    La=1;
	    break;
	 }   
	 case 0b01000000:{
	    La=2;
	    break;
	 }   
	 case 0b10000000:{
	    La=3;
	    break;
	 }   
      }
*/
      vDDRA >>=4;   
      while(vDDRA){
	 if(vDDRA&1)
	    break;
	 else{
	    La++;
	    vDDRA >>=1;
	 }
      }//end while
      
      while(cols) {
	 if(cols&1)
	    break;
	 else{
	    Ca++;
	    cols >>=1;
	 }
      }//end while
           
      cod_intern = La * 4 + Ca;   
      cod_extern = tabela_trans[cod_intern];
      
      return cod_extern;
   }
   else
      return 0x7f;
}//end kbscan