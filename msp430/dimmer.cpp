#include "io430.h"
#include "LCD16x2c.h"

unsigned char msg[7] = {'t','i','e','m','p','o',':'};
unsigned int tmp = 8000;

//Lozano Ramirez Angel Ivan     7CM7

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  
  TACTL = TASSEL_2+MC_2;
  
  //La detección de la señal de sincronía
  TACCTL0 = CM_3+CAP+CCIE;
  P1SEL_bit.P1 = 1;
  P1DIR_bit.P1 = 0;
  
  //Señal de disparo
  TACCTL1 = CCIE;
  P2DIR_bit.P0 = 1;
  P2OUT_bit.P0 = 0;
  
  P1DIR_bit.P4 = 0;             //Entrada para boton 1 (incremento)
  P1IES_bit.P4 = 0;
  P1IE_bit.P4 = 1;
  P1IFG_bit.P4 = 0;
  
  P1DIR_bit.P5 = 0;             //Entrada para boton 2 (decremento)
  P1IES_bit.P5 = 0;
  P1IE_bit.P5 = 1;
  P1IFG_bit.P5 = 0;
  
  __bis_SR_register(GIE);//Permiso a nivel Global GIE = 1

  Ini_Lcd();
  Cmd_Lcd(0x0C);
  for(char i=0;i<7;i++) Dato_Lcd(msg[i]);
  impNum(tmp/1000);
  Cmd_Lcd(0x88);
  Dato_Lcd('m');
  Dato_Lcd('s');
  while(1)
  {}
}
#pragma vector=TIMER0_A0_VECTOR //TACCR0
__interrupt void Deteccion_Flanco(void)
{
  TACCR1 = TACCR0 + tmp; // 8ms a 1ms
}
// Timer0_A2 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Tigger(void)
{
  switch( TA0IV )
  {
    case  2:  P2OUT_bit.P0 = 1; //TACCR1
              __delay_cycles(50);
              P2OUT_bit.P0 = 0;
              break;    
    case 10:            // TAIFG no se usa
              break;
  }
}

#pragma vector=PORT1_VECTOR
__interrupt void boton(void){
  if(P1IFG_bit.P4 == 1){
    if(tmp >= 8000)      tmp = 8000;
    else        tmp += 1000;
    Cmd_Lcd(0x87);
    impNum(tmp/1000);
  }
  
  if(P1IFG_bit.P5 == 1){
    if(tmp <= 1000)      tmp = 1000;
    else        tmp-=1000;
    Cmd_Lcd(0x87);
    impNum(tmp/1000);
  }
  P1IFG = 0;
}