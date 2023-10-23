//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"
#include "LCD16x2c.h"

unsigned int Pulso, Frec;
unsigned char a = 0, sel = 0;

int main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW+WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  P2SEL |= BIT7+BIT6;         //Acondicionando el reloj a cristal
  P1DIR_bit.P6 = 1;
  
  P1DIR_bit.P0 = 0;             //Entrada a medir
  P1IES_bit.P0 = 0;
  P1IE_bit.P0 = 1;
  P1IFG = 0;
  
  P2DIR_bit.P5 = 0;             //Entrada para boton
  P2IES_bit.P5 = 0;
  P2IE_bit.P5 = 1;
  P2IFG_bit.P5 = 0;
  
  TACTL = TASSEL_1+MC_1;
  TACCR0 = 32768;
  TACCTL0 = CCIE;
  
  TA1CTL = TASSEL_2+MC_1;             //Generacion de una señal de 1kHz
  TA1CCR0 = 191;        //Valor n-1
  TA1CCTL1 = OUTMOD_7;
  TA1CCR1 = TA1CCR0/2;        //Valor n-1 porque no se usa bandera
  P2SEL_bit.P1 = 1;
  P2DIR_bit.P1 = 1;
  
  __bis_SR_register(GIE);       //Permiso a nivel global 
  
  Ini_Lcd();
  
  while(1){
    if(a == 1){
      Cmd_Lcd(0x01);
      Dato_Lcd(0x66);
      Dato_Lcd(0x3D);
      impNum(Frec);             //NOTA: Esta funcion se codifico en la libreria del lcd
      Dato_Lcd(0x48);
      Dato_Lcd(0x7A);
      a = 0;
    }
    else;
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Segundo(void){
  P1OUT ^= BIT6;
  Frec = Pulso;
  a = 1;
  Pulso = 0x0000;
}

#pragma vector=PORT1_VECTOR
__interrupt void Acumula_Pulsos(void){
  if(P1IFG_bit.P0 == 1)  Pulso++;
  P1IFG = 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void boton(void){
  if(P2IFG_bit.P5 == 1){
    if(sel >= 3) sel = 0;
    else        sel++;
    switch(sel){
    case 0:
      TA1CCR0 = 191;
      break;
    case 1:
      TA1CCR0 = 117;
      break;
    case 2:
      TA1CCR0 = 99;
      break;
    default:
      break;
    }
  }
  P2IFG_bit.P5 = 0;
}