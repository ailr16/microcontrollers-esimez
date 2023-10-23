#include "io430.h"

const unsigned char LET1 [] = {"CARACTER NO VALIDO\r\n" };
const unsigned char LET2 [] = {"OK\r\n" };

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //DCO Trabajando a 1MHz aprox.
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  
  P1SEL  |= BIT1 + BIT2 ;               // Configurando las terminales P1.1 y P1.2
  P1SEL2 |= BIT1 + BIT2 ;               // como parte del sistema UART
  P1DIR &= ~BIT1;                       // P1.1 <-- RXD
  P1DIR |= BIT2;                        // P1.2 --> TXD
  UCA0CTL1 |= UCSSEL_2;                 // SMCLK
  //--------9600bps----------------------------------------------
  UCA0BR0 = 104;                       // 1MHz 9600
  UCA0BR1 = 0;                         // 1MHz 9600
  UCA0MCTL = UCBRS0;                   // Modulation UCBRSx = 1
  //-------------------------------------------------------------
  //--------19200bps---------------------------------------------
  //UCA0BR0 = 52;                        // 1MHz 19200
  //UCA0BR1 = 0;                         // 1MHz 19200
  //-------------------------------------------------------------
  //--------38400bps---------------------------------------------
  //UCA0BR0 = 26;                        // 1MHz 38400
  //UCA0BR1 = 0;                         // 1MHz 38400
  //-------------------------------------------------------------
  //--------115200bps----------------------------------------------
  //UCA0BR0 = 8;                         // 1MHz 115200
  //UCA0BR1 = 0;                         // 1MHz 115200
  //UCA0MCTL = UCBRS2+UCBRS1;            // Modulation UCBRSx = 6
  //-------------------------------------------------------------
  
  UCA0CTL1 &= ~UCSWRST;                 // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                      // Enable USCI_A0 RX interrupt
  
  __bis_SR_register(GIE);    
  
  while(1){}

}

//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  if(UCA0RXBUF == 'T')
  {for(char i = 0; i <= sizeof LET2 -1 ; i++)
    {UCA0TXBUF = LET2[i];
     while (!(IFG2&UCA0TXIFG))              
      {}
    }
  }
  else
  {for(char i = 0; i <= sizeof LET1 -1 ; i++)
    {UCA0TXBUF = LET1[i];
      while (!(IFG2&UCA0TXIFG)) {}
    }
  }
}

