#include "io430.h"
#include "LCD16x2c.h"

//Lozano Ramirez Angel Ivan     7CM7

unsigned int r = 0, g = 0, b = 0;

char num[5] = {0x30,0x30,0x30,0x30,0x30};

int main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  P1DIR_bit.P3 = 0;             //Entrada para boton 1 (controla color rojo)
  P1IES_bit.P3 = 0;
  P1IE_bit.P3 = 1;
  P1IFG_bit.P3 = 0;
  
  P1DIR_bit.P4 = 0;             //Entrada para boton 2 (controla color verde)
  P1IES_bit.P4 = 0;
  P1IE_bit.P4 = 1;
  P1IFG_bit.P4 = 0;
  
  P1DIR_bit.P5 = 0;             //Entrada para boton 3 (controla color azul)
  P1IES_bit.P5 = 0;
  P1IE_bit.P5 = 1;
  P1IFG_bit.P5 = 0;
  
  TA1CTL = TASSEL_2 + MC_1;
  TA1CCR0 = 254;
  TA1CCTL1 = OUTMOD_7;
  TA1CCR1 = r;        //ROJO
  P2SEL_bit.P1 = 1;     //P1.6 Parte del sistema temporizador
  P2DIR_bit.P1 = 1;     //P1.6 salida de comparacion del temporizador
  
  TACTL = TASSEL_2 + MC_1;
  TACCR0 = 254;
  TACCTL1 = OUTMOD_7;
  TACCR1 = g;         //VERDE
  P1SEL_bit.P6 = 1;     //P1.6 Parte del sistema temporizador
  P1DIR_bit.P6 = 1;     //P1.6 salida de comparacion del temporizador
  
  TA1CCTL2 = OUTMOD_7;
  TA1CCR2 = b;        //AZUL
  P2SEL_bit.P5 = 1;     //P1.6 Parte del sistema temporizador
  P2DIR_bit.P5 = 1;     //P1.6 salida de comparacion del temporizador
  
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
  
  while(1){
  }
}

/*#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void){
  switch(TA0IV){
    case 2:     break;
    case 10:    P1OUT ^= 0x01;
                break;
  }
}*/

#pragma vector=PORT1_VECTOR
__interrupt void boton(void){
  if(P1IFG_bit.P3 == 1){
    if(r >= 254){
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
  
  if(P1IFG_bit.P4 == 1){
    if(g >= 254){
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
  
  if(P1IFG_bit.P5 == 1){
    if(b >= 254){
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