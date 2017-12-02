#include <msp430.h>

int main(void)
{
  P1DIR = BIT2;
  P1SEL = BIT2;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 6;                              // 1MHz 9600 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
                                            // over sampling
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  TA0CCTL1 = OUTMOD_7;//SET/RESET MODE
  TA0CTL = TBSSEL_2 + MC_1; //SMCLK, up mode
  TA0CCR0 = 255; //Full Cycle
  TA0CCR1 = 0; //Sets the duty cycle to 50%

  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  __no_operation();                         // For debugger

  while(1)
   {
      switch(__even_in_range(UCA0IV, USCI_UCTXIFG)) //Equivalent to switch(UCA0IV) but generates more efficient code
      {
          case USCI_NONE: break; //Handles the case where RX = NULL

          case USCI_UCRXIFG: //Handles the case of an input being received.
          {TA0CCR1 = UCA0RXBUF;}
          case USCI_UCTXIFG: break; //In the case of the transmitting do nothing.

          default: break; // Covers the case to do nothing if somehow comes out of bounds.
      }
   }
}
