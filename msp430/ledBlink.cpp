
#include "io430.h"

char a = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  //Configurar P1.6 como salida
  P1SEL = 0x00;
  P1SEL2 = 0x00;
  P1DIR_bit.P0 = 1;
  P1DIR_bit.P6 = 1;
  
  while(1){
    P1OUT_bit.P0 = 1;
    P1OUT_bit.P6 = 0;
    __delay_cycles(500000);
    P1OUT_bit.P0 = 0;
    P1OUT_bit.P6 = 1;
    __delay_cycles(500000);
  }
}
