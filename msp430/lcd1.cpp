#include "io430.h"
#include "LCD16x2c.h"

//Lozano Ramirez Angel Ivan     7CM7

char arreglo[18] = {'H','e','l','l','o',' ','w','o','r','l','d','!',':','-',')','.','.','.'};

int main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  Ini_Lcd();
  Cmd_Lcd(0x01);
  Cmd_Lcd(0x02);
  for(char i=0;i<18;i++)       Dato_Lcd(arreglo[i]);      
  while(1){
    Cmd_Lcd(0x18);
    __delay_cycles(300000);
  }
}
