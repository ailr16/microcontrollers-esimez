//Lozano Ramirez Angel Ivan     7CM7
//Termometro.

#include "io430.h"
#include "LCD16x2c.h"

unsigned char a = 0;
unsigned char msg[6] = {'T','e','m','p','=',' '};
unsigned char i = 0;
unsigned long int resADC = 0;
unsigned long int mVin = 0;

int main( void ){
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;//DCO configurado a 1MHz
  DCOCTL  = CALDCO_1MHZ;
  
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + MSC + REFON + ADC10ON + ADC10IE;
  ADC10CTL1 = INCH_5 + CONSEQ_0;
  ADC10AE0 = BIT5;
  
  P1DIR_bit.P3 = 0;
  P1REN_bit.P3 = 1;
  P1OUT_bit.P3 = 1;
  P1IE_bit.P3 = 1;
  P1IES_bit.P3 = 1;
  P1IFG = 0;
  
  P1DIR_bit.P6 = 1;
  
   __bis_SR_register(GIE);//Permiso a nivel Global GIE = 1

  Ini_Lcd();
  Cmd_Lcd(0x0C);
  for(i=0; i<6; i++)   Dato_Lcd(msg[i]);
  
  while(1){
    if(a == 1){
      Cmd_Lcd(0x86);
      for(i=0; i<9; i++)    Dato_Lcd(' ');      //Limpia el resultado anterior
      Cmd_Lcd(0x86);
      //Realiza el calculo de mV leidos por al ADC
      mVin = resADC*1500;
      mVin = mVin/1023;
      impNumDec(mVin,1);      //Imprime el resultado de la conversion con un decimal
      Dato_Lcd(0xDF);    Dato_Lcd('C');         //Imprime °C
      a = 0;
    }
    else;
  }
}

#pragma vector = PORT1_VECTOR
__interrupt void ADC10_INI(void){
  if(P1IFG_bit.P3 == 1 ) ADC10CTL0 |= ENC+ADC10SC;
  P1IFG = 0;
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10RESULTADO(void){
  //ADC10MEM <-- Resultado de la conversion de la señal analogica
  resADC = ADC10MEM;
  a = 1;
  P1OUT_bit.P6 = 1;
  __delay_cycles(10000);
  P1OUT_bit.P6 = 0;
}
