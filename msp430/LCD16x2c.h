//******************************************************************************
// Programa para controlar un LCD de 16 caracteres por 2 líneas, utilizando tres 
// terminales del microcontrolador
//
//                MSP430G2553
//        3.3V -----------------       SN74HC164            LCD 16 x 2
//        /|\ |                 |     ___________            _______
//         |--| Vcc         P2.4|--->|CLOCK    Qa|---DB4--> |  ___  |-->5V
//            |             P2.3|-+->|A        Qb|---DB5--> | |   | |
//            |                 | |->|B        Qc|---DB6--> | |   | |
//            |                 |    |         Qd|---DB7--> | |   | |
//            |                 |    |         Qe|---RS --> | | L | |
//            |                 |    |           |          | | C | |
//            |                 |    |        CLR|-->5Vcc   | | D | |
//	      |                 |    |___________|          | |   | |
//            |                 |                           | |   | |
//            |             P2.2|-------------------- E --> | |___| |RW -->GND
//            |                 |                           |_______|----->GND
//            |             Vss |-->GND                                 
//            |                 |                                      
// JULIO 2019  IPN                                                        
// EDGAR R CALDERÓN DÍAZ
//****************************************************************************** 
#ifndef LCD16x2c_H_
#define LCD16x2c_H_

void Ini_Lcd(void);
void Cmd_Lcd(unsigned char CMD);
void Dato_Lcd(unsigned char DATO);
void Send_10b(unsigned int Dat_Ser);
void impNum(unsigned int A);
void impNumDec(unsigned int A, unsigned char decPos);

#define CLK  BIT4
#define AB   BIT3
#define E    BIT2

//Función de inicialización LCD para un Bus de datos de 4Bits
void Ini_Lcd(){
  P2SEL &= ~(E+AB+CLK);           //Configuración de las terminales P2.2, P2.3 y P2.4 para controlar el
  P2SEL2 &= ~(E+AB+CLK);          //74HC164 y el LCD
  P2DIR |= E+AB+CLK;
  P2OUT &= ~(E+AB+CLK);
  
  __delay_cycles(15000);        //Since VDD reaches more than 4.5V wait more than 15ms
  
                                //Function Set(Interface data length is 8-bit)
  Send_10b(0x063);              //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(40);           //| 0| 0 | 0 | 1 | 1 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 0 | 0 | 1 | 1 |
                                //Wait more than 40 us
  
  Send_10b(0x062);              //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(40);           //| 0| 0 | 0 | 1 | 1 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 0 | 0 | 1 | 0 |
                                //Wait more than 40 us
  
  Send_10b(0x048);             //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(40);           //| 0| 0 | 0 | 1 | 0 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 1 | 0 | 0 | 0 |
                                //Wait more than 40 us
  
  Send_10b(0x00F);             //|RS|R/W|DB7|DB6|DB5|DB4|
  __delay_cycles(40);           //| 0| 0 | 0 | 0 | 0 | 0 |
                                //|RS|R/W|DB7|DB6|DB5|DB4|
                                //| 0| 0 | 1 | 1 | 1 | 1 |
                                //Wait more than 40 us
  Cmd_Lcd(0x01);
}

void Cmd_Lcd(unsigned char CMD){
  unsigned int Dat10b;
  if ((CMD & 0xFC) <= 0x02){
    Send_10b(CMD);
    __delay_cycles(1640);
  }
  else{
    Dat10b = CMD;
    Dat10b = (Dat10b & 0x00F0) << 1;
    CMD = (CMD & 0x0F);
    Dat10b |= CMD;                                                                                                                                                                                                                                                             
    Send_10b(Dat10b);
    __delay_cycles(40);
  }
}

void Dato_Lcd(unsigned char DATO){
  unsigned int Dat10b;
  Dat10b = DATO;
  Dat10b = (Dat10b & 0x00F0) << 1;
  DATO = (DATO & 0x0F);
  Dat10b |= DATO+BIT9+BIT4;
  Send_10b(Dat10b);
  __delay_cycles(40);
}

void Send_10b(unsigned int Dat_Ser){
  for( char i=0; i<=9; i++){
    if((Dat_Ser & BIT9) != 0){
      P2OUT |= AB;
      P2OUT |= CLK;                 
      P2OUT &= ~CLK; 
      Dat_Ser <<= 1;
      if(i == 4){
        P2OUT |= E;                 
        P2OUT &= ~E; 
      }
    }
    else{
      P2OUT &= ~AB;
      P2OUT |= CLK;                
      P2OUT &= ~CLK; 
      Dat_Ser <<= 1;
      if(i == 4){
        P2OUT |= E;                
        P2OUT &= ~E; 
      }
    }
  }
  P2OUT |= E;                 
  P2OUT &= ~E;
}

void impNum(unsigned int A){
  //Funcion para imprimir un numero menor a 65535 en Lcd
  unsigned char m[5] = {0x30,0x30,0x30,0x30,0x30};
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
  while(i<5){
    Dato_Lcd(m[i]);
    i++;
  }
}

void impNumDec(unsigned int A, unsigned char decPos){
  //Funcion para imprimir un numero menor a 65535 en LCD con un punto decimal
  //indicado por decPos.
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
  
  for(unsigned char k = 5; k > 5 - decPos;k--)  m[k] = m[k-1];
  m[5-decPos] = 0x2E;
  
  while(i<6){
    Dato_Lcd(m[i]);
    i++;
  }
}

#endif /*LCD16x2c_H_*/