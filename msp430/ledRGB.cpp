//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"
#include "LCD16x2c.h"

unsigned int r = 0, g = 0, b = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;//DCO configurado a 1MHz
  DCOCTL  = CALDCO_1MHZ;
  
  P1DIR_bit.P4 = 0;             //Entrada para boton 1 (controla color rojo)
  P1IES_bit.P4 = 0;
  P1IE_bit.P4 = 1;
  P1IFG_bit.P4 = 0;
  
  P1DIR_bit.P5 = 0;             //Entrada para boton 2 (controla color verde)
  P1IES_bit.P5 = 0;
  P1IE_bit.P5 = 1;
  P1IFG_bit.P5 = 0;
  
  P1DIR_bit.P7 = 0;             //Entrada para boton 3 (controla color azul)
  P1IES_bit.P7 = 0;
  P1IE_bit.P7 = 1;
  P1IFG_bit.P7 = 0;
  
  TACTL = TASSEL_2+MC_1;
  TACCR0 = 255;
  TACCTL1 = OUTMOD_7;
  TACCR1 = g; //LED VERDE
  P1SEL_bit.P6 = 1;//P1.6 es parte del TimerA0_A3
  P1DIR_bit.P6 = 1;//P1.6 salida de comparación del TiemerA0_A3
  
  TA1CTL = TASSEL_2+MC_1;
  TA1CCR0 = 255;
  TA1CCTL1 = OUTMOD_7;
  TA1CCR1 = r; //LED ROJO
  P2SEL_bit.P1 = 1;
  P2DIR_bit.P1 = 1;
  
  TA1CCTL2 = OUTMOD_7;
  TA1CCR2 = b; //LED AZUL
  P2SEL_bit.P5 = 1;
  P2DIR_bit.P5 = 1;
  
  __bis_SR_register(GIE);
  
  Ini_Lcd();
  Cmd_Lcd(0x0E);
  Dato_Lcd(0x52);
  Dato_Lcd(0x3D);
  impNum(r);
  Cmd_Lcd(0x85);
  Dato_Lcd(0x47);
  Dato_Lcd(0x3D);
  impNum(g);
  Cmd_Lcd(0x8A);
  Dato_Lcd(0x42);
  Dato_Lcd(0x3D);
  impNum(b);
  
  while(1)
  {
  }
}

#pragma vector=PORT1_VECTOR
__interrupt void boton(void){
  if(P1IFG_bit.P4 == 1){
    if(r >= 255){
      r = 0;
      Cmd_Lcd(0x82);    Dato_Lcd(0x20);         //
      Cmd_Lcd(0x83);    Dato_Lcd(0x20);         //Limpia (imprime caracter espacio) numeros mayores a 9
      Cmd_Lcd(0x84);    Dato_Lcd(0x20);         //
    }
    else        r++;
    TA1CCR1 = r;
    Cmd_Lcd(0x82);
    impNum(r);
  }
  
  if(P1IFG_bit.P5 == 1){
    if(g >= 255){
      g = 0;
      Cmd_Lcd(0x87);    Dato_Lcd(0x20);         //
      Cmd_Lcd(0x88);    Dato_Lcd(0x20);         //Limpia (imprime caracter espacio) numeros mayores a 9
      Cmd_Lcd(0x89);    Dato_Lcd(0x20);         //
    }
    else        g++;
    TACCR1 = g;
    Cmd_Lcd(0x87);
    impNum(g);
  }
  
  if(P1IFG_bit.P7 == 1){
    if(b >= 255){
      b = 0;
      Cmd_Lcd(0x8C);    Dato_Lcd(0x20);         //
      Cmd_Lcd(0x8D);    Dato_Lcd(0x20);         //Limpia (imprime caracter espacio) numeros mayores a 9
      Cmd_Lcd(0x8E);    Dato_Lcd(0x20);         //
    }
    else        b++;
    TA1CCR2 = b;
    Cmd_Lcd(0x8C);
    impNum(b);
  }
  
  P1IFG = 0;
}



