#include "io430.h"
#include "LCD16x2c.h"

//Lozano Ramirez Angel Ivan     7CM7

unsigned int X = 0;

char num[5] = {0x30,0x30,0x30,0x30,0x30};

int main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  Ini_Lcd();
  impNum(X);
  
}
