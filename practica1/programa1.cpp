//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"

unsigned char A = 0;             //Variable de 8 bits

unsigned char Arreglo1[33] = {0x81, 0x81, 0xFF, 0x81, 0x81, 0x00, 0x00,
                              0xE0, 0x18, 0x06, 0x01, 0x06, 0x18, 0xE0, 0x00, 0x00,
                              0x3F, 0x68, 0xC8, 0x68, 0x3F, 0x00, 0x00,
                              0xFF, 0xC0, 0x70, 0x1C, 0x06, 0xFF, 0x00, 0x00, 0x00, 0x00};
                              
unsigned char Arreglo2[28] = {0x02, 0x05, 0x05, 0x05, 0x02, 0x7C, 0x40,
                              0x44, 0xCA, 0x8A, 0x8A, 0x84, 0xF8, 0x00,
                              0x00, 0x00, 0x04, 0x0A, 0x0A, 0x0A, 0x04,
                              0xF8, 0x80, 0xC0, 0x40, 0x00, 0x00, 0x00};

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
  P2IES_bit.P0 = 0;     //Flanco de subida
  P2IE_bit.P0 = 1;      //Permiso de interrupcion P2.0 (Permiso local)
  P2IFG_bit.P0 = 0;     //Borrado de bandera de interrupcion 
  
  __bis_SR_register(GIE);       //Permiso a nivel global

  
  while(1){
    if(A == 1){
      for(char i=0; i<=28; i++){
        P1OUT = Arreglo2[i];
        __delay_cycles(2000);
      }
    }
    else{
      for(char i=0; i<=33; i++){
        P1OUT = Arreglo1[i];
        __delay_cycles(2000);
      }
    }
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void){
  if(P2IFG_bit.P0 == 1){
    if(A == 1)  A = 0;
    else        A = 1;
  }
  P2IFG_bit.P0 = 0;     //Borrar bandera antes de salir de la rutina de atencion a interrupcion
}
