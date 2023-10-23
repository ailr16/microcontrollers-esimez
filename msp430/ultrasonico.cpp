//Lozano Ramirez Angel Ivan     7CM7

#include "io430.h"
#include "LCD16x2c.h"

unsigned int d = 0,
             t = 0;

unsigned char imp = 0,
              cnt = 0,
              estadoBoton = 0;

unsigned char mensajeError[14]= {"Fuera de Rango"}; //Título LCD

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;  //DC0 a 1MHz
  DCOCTL = CALDCO_1MHZ;
  
  P1DIR_bit.P0 = 1;     //P1.0 para trigger
  P1OUT_bit.P0 = 0;
  
  P1DIR_bit.P2 = 0;     //P1.2 para boton
  P1IES_bit.P2 = 0;
  P1IE_bit.P2 = 1;
  P1IFG_bit.P2 = 0;
  
  TACTL = TASSEL_2 + MC_2;      //Temporizador
  TACCTL0 = CM_3 + CAP + CCIE;
  P1SEL_bit.P1 = 1; 
  P1DIR_bit.P1 = 0; //P1.1 Entrada para echo
  
  __bis_SR_register(GIE);
  
  //Config LCD
  Ini_Lcd( ); //Inicializar LCD
  Cmd_Lcd(0x0C); //Apagar parpadeo del cursor
  
  while(1){
    P1OUT_bit.P0 = 0;
    __delay_cycles(5);          //5us apagagado para asegurar un pulso
    P1OUT_bit.P0 = 1;           //Estado alto en la salida
    __delay_cycles(10);         //10us en estado alto
    P1OUT_bit.P0 = 0;           //Regresar al estado bajo
    
    if(estadoBoton){
      if(imp){                  //Imprime la distancia
        Cmd_Lcd(0x01);
        Dato_Lcd('d');
        Dato_Lcd('=');
        impNum(d);              //Esta funcion esta declarada y definida en el archivo LCD16x2c.h
        if(d > 99){             //Si la distancia es mayor a 99 el mesnaje se imprime de una forma
          Cmd_Lcd(0x85);
          Dato_Lcd('c');
          Dato_Lcd('m');
        }
        else{                   //En elotro caso, se imprime de otra forma
          Dato_Lcd('c');
          Dato_Lcd('m');
        }
        imp = 0;
        estadoBoton = 0;
      }
      
      else{
        Cmd_Lcd(0x01);
        for(char i=0;i<=13;i++)   Dato_Lcd(mensajeError[i]); //Imprime Fuera de Rango
        imp = 0;
        estadoBoton = 0;
      }
    }
    else;
    
    __delay_cycles(120000);
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Deteccion_Flanco(void){
  cnt++;
  if( cnt == 1 ){                 //Flanco de subida 
    TACTL |=  TACLR;              //Limpia registros asociados al temporizador
    TACTL = TASSEL_2 + MC_2;      //Configura Temporizador
  }
  
  if( cnt == 2 ){               //Flanco de bajada
    t = TACCR0;
    if(t > 24000)  d = 0;       //Límite de distancia medible
    else{
      d = t/58;                 //Calcula la distancia
      imp = 1;                  //Activa bandera para imprimir en LCD
    }
    cnt = 0;
  }
  TA1CTL &= ~TAIFG;             //Limpia bandera de interrupción
}

#pragma vector=PORT1_VECTOR
__interrupt void boton(void){
  if(P1IFG_bit.P2) estadoBoton = 1;
  else  estadoBoton = 0;
  P1IFG = 0;                    //Limpia bandera de interrupción
}