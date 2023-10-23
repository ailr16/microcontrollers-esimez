//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  //Configurar P1.0, P1.1, P1.2 como entrada y P1.7 como salida
  P1SEL = 0x00;
  P1SEL2 = 0x00;
  P1DIR_bit.P0 = 0;
  P1DIR_bit.P1 = 0;
  P1DIR_bit.P2 = 0;
  P1DIR_bit.P7 = 1;
    
  
  while(1){
    do P1OUT_bit.P7 = 0;
    while(((P1IN_bit.P0 == 1) || (P1IN_bit.P1 == 1)) || (P1IN_bit.P2 == 0));
    
    do P1OUT_bit.P7 = 1;
    while(((P1IN_bit.P0 == 0) || (P1IN_bit.P1 == 0)) && (P1IN_bit.P2 != 0));
  }
}