//Lozano Ramirez Angel Ivan     7CM7
//Practica 6. Termostato electronico.   16.01.2021

#include "io430.h"

const unsigned char msg25[] = {"Temperatura baja" };
const unsigned char msg2535[] = {"Temperatura normal" };
const unsigned char msg35[] = {"Temperatura alta" };

unsigned char i = 0;
unsigned long int resADC = 0;
unsigned long int mVin = 0;

void impNumDec(unsigned int A, unsigned char decPos);

int main( void ){
  WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer to prevent time out reset
  BCSCTL1 = CALBC1_1MHZ;        //DCO Trabajando a 1MHz aprox.
  DCOCTL  = CALDCO_1MHZ;
  
  P1SEL  |= BIT1 + BIT2 ;               // Configurando las terminales P1.1 y P1.2
  P1SEL2 |= BIT1 + BIT2 ;               // como parte del sistema UART
  P1DIR &= ~BIT1;                       // P1.1 <-- RXD
  P1DIR |= BIT2;                        // P1.2 --> TXD
  UCA0CTL1 |= UCSSEL_2;                 // SMCLK
  
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + MSC + REFON + ADC10ON + ADC10IE;
  ADC10CTL1 = INCH_5 + CONSEQ_0;
  ADC10AE0 = BIT5;
    
  //--------9600bps----------------------------------------------
  UCA0BR0 = 104;                       // 1MHz 9600
  UCA0BR1 = 0;                         // 1MHz 9600
  UCA0MCTL = UCBRS0;                   // Modulation UCBRSx = 1
  //-------------------------------------------------------------
  
  UCA0CTL1 &= ~UCSWRST;                 // **Initialize USCI state machine**
  
  __bis_SR_register(GIE);    
  
  while(1){
    ADC10CTL0 |= ENC+ADC10SC;           //Habilita la conversion A-D
    mVin = resADC*1500;                 //Realiza el calculo de la tension
    mVin = mVin/1023;                   //de entrada en mV
    impNumDec(mVin,1);      //Imprime el resultado de la conversion con un decimal
    UCA0TXBUF = ' ';    while(!(IFG2&UCA0TXIFG));     //Imprime un caracter Espacio
    if(mVin < 250)      //Caso menor a 25 grados celsius
      for(i=0; i<sizeof(msg25)-1; i++){
      UCA0TXBUF = msg25[i];    while (!(IFG2&UCA0TXIFG));
      }
    else if(mVin >= 250 && mVin <= 350)     //Caso entre 25 y 35 grados Celsius
      for(i=0; i<sizeof(msg2535)-1; i++){
      UCA0TXBUF = msg2535[i];    while (!(IFG2&UCA0TXIFG));
      }
    else if(mVin > 350)         //Caso mayor a 35 grados Celsius
      for(i=0; i<sizeof(msg35)-1; i++){
      UCA0TXBUF = msg35[i];    while (!(IFG2&UCA0TXIFG));
      }
    UCA0TXBUF = 0x0A;    while(!(IFG2&UCA0TXIFG));     //NL
    UCA0TXBUF = 0x0D;    while(!(IFG2&UCA0TXIFG));     //CR
    __delay_cycles(250000);             //Retardo de 250ms
  }
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10RESULTADO(void){
  resADC = ADC10MEM;            //Guarda el resultado de la conversion A-D
  __delay_cycles(10000);        
}

void impNumDec(unsigned int A, unsigned char decPos){
  //Funcion para imprimir un numero menor a 65535 via puerto serie con un punto
  //decimal indicado por decPos.
  //Ejemplo de uso:    Para imprimir 12.3 la llamada a la funcion es:
  //                    impNumDec(123,1)
  //                   Para imprimir 1.23:      impNumDec(123,2)
  
  unsigned char m[6] = {0x30,0x30,0x30,0x30,0x30,0x30};
  unsigned char i = 0;
  if(A < 10000) i = 1;
  if(A < 1000)  i = 2;
  if(A < 100)   i = 3;
  if(A < 10)    i = 4;
  
  while(A >= 10000){
    m[0]++;
    A -= 10000;
  }
  while(A >= 1000){
    m[1]++;
    A -= 1000;
  }
  while(A >= 100){
    m[2]++;
    A -= 100;
  }
  while(A >= 10){
    m[3]++;
    A -= 10;
  }
  while(A >= 1){
    m[4]++;
    A -= 1;
  }
  
  for(unsigned char k = 5; k > 5 - decPos;k--)  m[k] = m[k-1];  //Recorre los
                                                                //numeros despues
                                                                //del punto
  m[5-decPos] = 0x2E;           //Coloca el caracter punto
  
  while(i<6){
    UCA0TXBUF = m[i];
    while (!(IFG2&UCA0TXIFG));
    i++;
    __delay_cycles(2000);
  }
}