//Lozano Ramirez Angel Ivan     7CM7
//Examen 3. Termostato electronico.   16.01.2021

#include "io430.h"
#include "LCD16x2c.h"

unsigned char i = 0;
char sel = 'r';
unsigned long int resADC = 0;
unsigned long int resLCD = 0;
unsigned long int resPWM = 0;
char a = 0;

int main( void ){
  WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer to prevent time out reset
  BCSCTL1 = CALBC1_1MHZ;        //DCO Trabajando a 1MHz aprox.
  DCOCTL  = CALDCO_1MHZ;
  
  //////////////////////////////Configuracion ADC//////////////////////////////
  ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
  ADC10CTL1 = INCH_7 + CONSEQ_0;
  ADC10AE0 = BIT7;              //Seleccion del pin P1.7 como entrada del ADC
  
  ////////////////////Configuracion PWM, entradas y salidas////////////////////
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
  
  TACTL = TASSEL_2 + MC_1;
  TACCR0 = 5000;
  TACCTL1 = OUTMOD_7;
  TACCR1 = 0; //LED VERDE
  P1SEL_bit.P6 = 1;//P1.6 es parte del TimerA0_A3
  P1DIR_bit.P6 = 1;//P1.6 salida de comparación del TiemerA0_A3
  
  TA1CTL = TASSEL_2 + MC_1;
  TA1CCR0 = 4999;
  TA1CCTL1 = OUTMOD_7;
  TA1CCR1 = 0; //LED ROJO
  P2SEL_bit.P1 = 1;
  P2DIR_bit.P1 = 1;
  
  TA1CCTL2 = OUTMOD_7;
  TA1CCR2 = 0; //LED AZUL
  P2SEL_bit.P5 = 1;
  P2DIR_bit.P5 = 1;
  
  __bis_SR_register(GIE);    
  
  Ini_Lcd();
  Cmd_Lcd(0x0C);
    Dato_Lcd('R');        Dato_Lcd('=');
  Cmd_Lcd(0x83);
    Dato_Lcd(' ');      Dato_Lcd('0');
  Cmd_Lcd(0x8A);
    Dato_Lcd('G');        Dato_Lcd('=');
  Cmd_Lcd(0x8D);
    Dato_Lcd('0');
  Cmd_Lcd(0xC5);
    Dato_Lcd('B');        Dato_Lcd('=');
  Cmd_Lcd(0xC8);
    Dato_Lcd('0');
  
  while(1){
    ADC10CTL0 |= ENC + ADC10SC;           //Habilita la conversion A-D
    if(a == 1){
      resLCD = resADC*255;
      resLCD = resLCD/1023;
      resPWM = resLCD*4999;
      resPWM = resPWM/255;
      switch(sel){
      case 'r':
          TA1CCR1 = resPWM; //LED ROJO
          Cmd_Lcd(0x83);  Dato_Lcd(' ');  Dato_Lcd(' ');  Dato_Lcd(' '); //Limpia
          Cmd_Lcd(0x83);  impNum(resLCD);
        break;
      case 'g':
          TACCR1 = resPWM; //LED VERDE
          Cmd_Lcd(0x8D);  Dato_Lcd(' ');  Dato_Lcd(' ');  Dato_Lcd(' '); //Limpia
          Cmd_Lcd(0x8D);  impNum(resLCD);
        break;
      case 'b':
          TA1CCR2 = resPWM; //LED AZUL
          Cmd_Lcd(0xC8);  Dato_Lcd(' ');  Dato_Lcd(' ');  Dato_Lcd(' '); //Limpia
          Cmd_Lcd(0xC8);  impNum(resLCD);
        break;
      default:
        break;
      }
      a = 0;
    }
    else;
    __delay_cycles(40000);             //Retardo de 40ms
  }
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10RESULTADO(void){
  resADC = ADC10MEM;            //Guarda el resultado de la conversion A-D
  __delay_cycles(5000);         //Retardo de 5ms
  a = 1;
}

#pragma vector=PORT1_VECTOR
__interrupt void boton(void){
  if(P1IFG_bit.P3 == 1) sel = 'r';        //Seleccionar rojo
  if(P1IFG_bit.P4 == 1) sel = 'g';        //Seleccionar verde
  if(P1IFG_bit.P5 == 1) sel = 'b';        //Seleccionar azul
  else;
  P1IFG = 0;
}