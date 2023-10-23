//CASTILLO ZUÑIGA CHRISTIAN GABRIEL 7CM7
#include "io430.h"
#include "LCD16x2c.h"
unsigned int Longitud, t1, t2;
char Imprimir, Vez;
unsigned char FR[14]= {"Fuera de Rango"}; //Título LCD
void IntToLCD( unsigned int Aux); //Impresión números en LCD

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;  //DC0 configurado 1MHz
  DCOCTL = CALDCO_1MHZ;
  
  //Trigger
  P1DIR_bit.P0 = 1; //P1.0 como salida
  P1OUT_bit.P0 = 0; //Estado bajo en la salida
  
  //Sistema Temporizador Deteccion flanco de subida o de bajada
  TACTL = TASSEL_2 + MC_2;  //Timer0 SMClock + Modo coninuo
  TACCTL0 = CM_3 + CAP + CCIE;  //Registro de control TACCR0
  P1SEL_bit.P1 = 1; 
  P1DIR_bit.P1 = 0; //P1.1 Entrada de sincronía
  
  __bis_SR_register(GIE);  //Permiso a nivel Global = 1
  
  //Config LCD
  Ini_Lcd( ); //Inicializando LCD
  Cmd_Lcd(0x0C); //Apaga parpadeo del cursor
  
  while(1)
  {
    P1OUT_bit.P0 = 1; //Estado alto en la salida
    __delay_cycles(10); //Delay 10us
    P1OUT_bit.P0 = 0; //Estado bajo en la salida (termina pulso de 10us)
    
    if(Imprimir == 1 && Longitud <= 400)
    {
      Cmd_Lcd(0x01); //Limpia pantalla
      Cmd_Lcd(0xC4); //Carácter position 0x46
      Dato_Lcd('L'); //L
      Dato_Lcd('='); //=
      IntToLCD(Longitud); //Tiempo en ms
      Dato_Lcd('c'); //c
      Dato_Lcd('m'); //m
      Cmd_Lcd(0xC6); //Caracter position 0x48
      IntToLCD(Longitud); //Longitud en cm
      
      Imprimir = 0; //Deja de imprimir hasta haber algun cambio
    }
    
    if(Imprimir == 1 && Longitud > 400)
    {
      Cmd_Lcd(0x81); //Carácter position 0x01
      for(char i=0;i<=13;i++)
      {
        Dato_Lcd(FR[i]); //Imprime Fuera de Rango
      }
      Cmd_Lcd(0xC6); //Caracter position 0x48
      IntToLCD(Longitud); //Longitud en cm
    }
    
    __delay_cycles(500000); //Espera 0.5ms para volver a medir
  }
}

#pragma vector=TIMER0_A0_VECTOR  //Interrupcion Timer0
__interrupt void Deteccion_Flanco(void)
{
  Vez++;
  if( Vez == 1 )  //Si es la 1a vez que entra a la interrupcón es el t1 (flanco de subida de ECHO)
  {
    t1 = TACCR0;  //Guarda el tiempo 1
  }
  
  if( Vez == 2 )  //Si es la 2a vez que entra a la interrupcón es el t2 (flanco de bajada de ECHO)
  {
    t2 = TACCR0;  //Guarda el tiempo 2
    Longitud = (t2 - t1)/58;  //Conversion de 1us a cm    
    Imprimir = 1;  //Manda a imprimir
    Vez = 0; //Reinicia indicador de t1 y t2
  }
  
}

void IntToLCD( unsigned int Aux){
  unsigned char Centena, Decena, Unidad;  //Variables
  Centena = 0x30;  //Inicializo en cero hex del LCD
  Decena = 0x30;   //Inicializo en cero hex del LCD
  Unidad = 0x30;   //Inicializo en cero hex del LCD
  
  while(Aux >= 100)  //Obteniendo Centenas
  {
    Aux -= 100;  //Aux= Aux-100
    Centena++;
  }
  
  while(Aux >= 10)  //Obteniendo Decenas
  {
    Aux -= 10;  //Aux= Aux-10
    Decena++;
  }
  
  while(Aux >= 1)  //Obteniendo Unidades
  {
    Aux -= 1;  //Aux= Aux-1
    Unidad++;
  }
  Dato_Lcd(Centena);  //Muestra en el LCD Centena
  Dato_Lcd(Decena);   //Muestra en el LCD Decena
  Dato_Lcd(Unidad);   //Muestra en el LCD Unidad
}