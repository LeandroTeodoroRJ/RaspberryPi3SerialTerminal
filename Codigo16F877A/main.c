//****************************************************************************
//                        RASPBERRY TERMINAL PIC
//****************************************************************************
//Compilador CCS PCWH Ver 4.104

#include <16F877.h>              
#include <string.h>
#include <16f8x_registradores.h>//MAPEAMENTO DE MEÓRIA DOS REGISTRADORES
                                                    

//****************************************************************************
//                    CONFIGURAÇÕES DO COMPILADOR
//****************************************************************************

#use fast_io(a)                  //modo de entrada e saida de dados que
#use fast_io(b)                  //o compilador nao configura o tris

#use delay(clock=4000000)        //clock usual
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT //

//#rom 0x2100={0b00010010,0x00}    //programa os primeiros dois endereços da eeprom
                                 

//****************************************************************************
//                                BIOS
//****************************************************************************
//****************************  ENTRADAS  ************************************

//*****************************  SAÍDAS  *************************************
// As definições a seguir são utilizadas para acesso aos pinos do display
// caso o pino RW não seja utilizado, comente a definição lcd_rw
#ifndef lcd_enable
   #define lcd_enable       pin_d5      // pino enable do LCD
   #define lcd_rs         pin_d4      // pino rs do LCD
   //#define lcd_rw      pin_e2      // pino rw do LCD
   #define lcd_d4         pin_d0      // pino de dados d4 do LCD
   #define lcd_d5         pin_d1      // pino de dados d5 do LCD
   #define lcd_d6         pin_d2      // pino de dados d6 do LCD
   #define lcd_d7         pin_d3      // pino de dados d7 do LCD
#endif
//****************************************************************************
//                        CONSTANTES E VARIÁVEIS
//****************************************************************************
#define  debouce     10
#define alarme_off_flag alarme_status,1

int   w_temp;
int   status_temp;
short int   dado_recebido;    //flag de novo dado recebido
int   count;


//***************************************************************************
//                               DRIVES
//***************************************************************************
#include "C:\DADOS\projeto\INCLUDES\usart.h"
#include "c:\dados\projeto\includes\lcd.h"

//****************************************************************************
//                       Rotinas da interrupcao
//****************************************************************************
//#inline                     //descomentar se for usar o in_line
recebe_dado_usart(){
   if (usart_rx()==novo_dado){  //Novo dado recebido
   dado_recebido=true;
   }
   else{
   //Se for erro reseta a USART
   usart_config(modo8bits, assincrono, tx_on, usart_off, rx_continuo);
   usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);
   }
}


//****************************************************************************
//                           INTERRUPÇÕES
//****************************************************************************
#int_global
void interrupcao(){
restart_wdt();
disable_interrupts(global);
#asm
movwf w_temp
swapf status,w
movwf status_temp
#endasm
      
   if (interrupt_rx_usart()==true){  //Foi interrupão da USART?
   recebe_dado_usart();
   }

//***************************************************************************
//                          FIM DA INTERRUPCAO
//***************************************************************************
enable_interrupts(global);
#asm
swapf status_temp,w
movwf status
swapf w_temp,f
swapf w_temp,w
#endasm
}


//****************************************************************************
//                             Sub-rotinas
//****************************************************************************
trata_dado_serial(){    
   if (dado_recebido==true){    //Recebeu um novo dado?
   usart_tx(usart_rx_dado);     //Devolve o dado recebido
   lcd_escreve('\f');   //Limpa o lcd
   lcd_escreve('\b');
   lcd_pos_xy(1,1);
   printf(lcd_escreve, "Rx: %u", usart_rx_dado);
   dado_recebido=false;
    if (usart_rx_dado==65){   //A=65
      lcd_pos_xy(1,2);
      lcd_escreve("Funfou!");      
      }
   
   }
}




//****************************************************************************
//                          Rotina Principal
//****************************************************************************

void main(){
//********************** CONFIGURAÇÕES INICIAIS *****************************
      setup_timer_0(rtcc_internal); //clock interno, prescaler 1:2
      setup_wdt(wdt_288ms);
            
      //Configurações da USART
      usart_baud_rate(high_speed, 25);
      usart_config(modo8bits, assincrono, tx_on, usart_on, rx_continuo);


      bit_set(option_reg,7);    //sem pull-up

//    Configuração do tris
//    Bit porta: 76543210
      tris_a = 0b00000000;
      tris_b = 0b00000000;
      tris_c = 0b10000000;
      tris_d = 0b00000000;
      
      enable_interrupts(GLOBAL); //Habilita interrupção de recepção da USART
      usart_int(tx_int_off, rx_int_on, int_9bit_off);
      
//    Inicialização do LCD
      lcd_ini();
      delay_ms(100);
      lcd_escreve('\f');   //Limpa o lcd
      lcd_escreve('\b');
//******************** INICIALIZAÇÃO DE VARIÁVEIS ***************************
      
//****************************************************************************
//                             INICIO DO LOOP
//****************************************************************************
      while(1){  
      restart_wdt();
      trata_dado_serial();
         
      }
//********************************* FIM **************************************
}


















































