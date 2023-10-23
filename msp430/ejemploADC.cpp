//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"
#include "LCD16x2c.h"

unsigned char a = 0;
unsigned char msg[6] = {'T','e','m','p','=',' '};
unsigned char i = 0;
unsigned int res = 0;

unsigned int convCelsius(unsigned int a);

int main(void){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;  //DC0 a 1MHz
  DCOCTL = CALDCO_1MHZ;
  
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE + REFON;             // + REF2_5V + REFON
  ADC10CTL1 = INCH_5 + CONSEQ_0;
  ADC10AE0 = BIT5;
    
  P1DIR_bit.P3 = 0;
  P1REN_bit.P3 = 1;
  P1OUT_bit.P3 = 1;
  P1IE_bit.P3 = 1;
  P1IES_bit.P3 = 1;
  P1IFG = 0;
    
  P1DIR_bit.P6 = 1;
    
  __bis_SR_register(GIE);
  
  Ini_Lcd();
  for(i=0; i<6; i++)   Dato_Lcd(msg[i]);
  
  while(1){
    if(a == 1){
      Cmd_Lcd(0x86);
      Dato_Lcd(' ');    Dato_Lcd(' ');
      Dato_Lcd(' ');    Dato_Lcd(' ');
      Cmd_Lcd(0x86);
      impNum(convCelsius(ADC10MEM));
    }
    else;
    a = 0;
  }
}


#pragma vector=PORT1_VECTOR
__interrupt void ADC10(void){
  if(P1IFG_bit.P3 == 1) ADC10CTL0 |= ENC + ADC10SC;
  a = 1;
  P1IFG = 0;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10RESULTADO(void){
  P1OUT_bit.P6 = 1;
  __delay_cycles(5000);         //10000
  P1OUT_bit.P6 = 0;
}

unsigned int convCelsius(unsigned int nadc){
  unsigned int t;  
  t = nadc*1.4663;
  return t;
}