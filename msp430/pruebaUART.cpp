#include <msp430.h>

void TxUART(char *pData);


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  if (CALBC1_1MHZ==0xFF)		    // If calibration constant erased
  {											
    while(1);                               // do not load, trap CPU!!	
  }

  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;                      
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 0x65;                           // 1MHz 115200
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  
  while(1)
  {
    TxUART("Hello World\n");
    TxUART("Msp430G2553\n");
    TxUART("\n\n");
    for(int i=0;i<32760;i++);
  }
  
  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

void TxUART(char *pData)
{
  while(*pData)
  {
      while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
      UCA0TXBUF = *pData++;
  }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}