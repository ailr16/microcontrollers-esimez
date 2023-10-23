//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"

unsigned char cnt = 0x00;             //Variable de 8 bits para contador
unsigned char sel = 0;             //Variable de 8 bits para seleccion de flanco

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
  
  //Configurar la terminal P2.0 como entrada y configurar la interrupcion en el pin
  P2SEL = 0x00;
  P2SEL2 = 0x00;
  P2DIR_bit.P0 = 0;
  P2IES_bit.P0 = 0;     //Flanco de subida
  P2IE_bit.P0 = 1;      //Permiso de interrupcion P2.0 (Permiso local)
  P2IFG_bit.P0 = 0;     //Borrado de bandera de interrupcion 
  
  //Configurar la terminal P2.3 como entrada y configurar la interrupcion en el pin
  P2DIR_bit.P3 = 0;
  P2IES_bit.P3 = 1;     //Flanco de bajada
  P2IE_bit.P3 = 1;      //Permiso de interrupcion P2.0 (Permiso local)
  P2IFG_bit.P3 = 0;     //Borrado de bandera de interrupcion
 
  __bis_SR_register(GIE);       //Permiso a nivel global
  
  //Configurar P1 como salida
  P1SEL = 0x00;
  P1SEL2 = 0x00;
  P1DIR = 0xFF;
  
  while(1){
    P1OUT = cnt;
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR (void)
{
  if (P2IFG_bit.P0 == 1){
    if(sel == 1) sel = 0;
    else sel = 1;
  }
  
  if (P2IFG_bit.P3 == 1 && sel == 0){
    if(cnt == 0x14) cnt = 0;
    else        cnt = cnt+1;
  }
  if((P2IFG_bit.P3 == 1 && sel == 1)){
    if(cnt == 0)    cnt = 0x14;
    else        cnt = cnt-1;
  }
  
  P2IFG_bit.P3 = 0;//Borrar bandera antes de salir de la rutina de atencion a interrupcion
  P2IFG_bit.P0 = 0;//Borrar bandera antes de salir de la rutina de atencion a interrupcion
}