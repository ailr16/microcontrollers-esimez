#include "io430.h"

char A = 0;             //Variable de 8 bits
char arreglo1[9] = {0x01,0x03,0x05,0x07,0x70,0x50,0x30,0x10,0x11};      //Arreglo 1 de 9 elementos de 1 byte
char arreglo2[9] = {0x02,0x04,0x06,0x08,0x80,0x60,0x40,0x20,0x22};      //Arreglo 2 de 9 elementos de 1 byte

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  //Configurar P1 como salida
  P1SEL = 0x00;
  P1SEL2 = 0x00;
  P1DIR = 0xFF;
  
  //Configurar la terminal P2.0 como entrada
  P2SEL = 0x00;
  P1SEL2 = 0x00;
  P2DIR_bit.P0 = 0;
  
  while(1){
    if(P2IN_bit.P0 == 1){
      if(A == 1)   A = 0;
      else      A = 1;
    }
    if(A == 1){
      for(char i=0; i<=8; i++){
        P1OUT = arreglo2[i];
        __delay_cycles(250);
      }
    }
    else{
      for(char i=0; i<=8; i++){
        P1OUT = arreglo1[i];
        __delay_cycles(250);
      }
    }
  }
}