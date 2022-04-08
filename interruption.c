/*
 * File:   interruption.c
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

#include <p18cxxx.h>
#include "interruption.h"
#include "global.h"
#include "MI2C.h"
#include "coreFunctions.h"

volatile char str[30]="vbat=|0";
char message_fin[30]="fin test\r\n";
char message_batt[30]="vbatt ok\r\n";
char message_batt_bad[30]="vbatt faible\r\n";
char arret_telecommande[30]="arret bouton\r\n";
char avancer_IR_msg[30]="Mesure IR en avancant \r\n";

unsigned int vbat_receive = 12;
unsigned int cmpt_batt = 0;

unsigned int i=0;
unsigned int j=0;
unsigned int avant=0;
unsigned int cpt=2;
unsigned int distance = 0;
unsigned int temporisation = 0;

unsigned int irg_value = 0;
unsigned int ird_value = 0;

// Initialisation variable phase avancer vers objet
extern int avancer_objet;

// Initialisation batt values
volatile int ubatt = 0;
volatile int bat[4] = {0, 0, 0, 0};

//high priority vector
#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR
void HighISR(void)
{
   
   //Interruption télécommande
   if(INTCONbits.INT0IF)
   {
      INTCONbits.INT0IF = 0; //rechargement interruption
      
      tempo(2); //temporisation pour appui long
      printf("Interruption telecommande \r\n");
      telecommande(); //fonction ordre telecommande
      tempo(2); //temporisation pour appui long
   }

   //IT Timer0
   if(INTCONbits.TMR0IF)
   {
        //Timer0
        INTCONbits.TMR0IF = 0; //rechargement du timer
        TMR0H = 0xFD;
        TMR0L = 0xAF;  //rechargement 24465

        //FOnction de temporisation
        temporisation++;

        //Clignotement led test vie
        PORTBbits.RB5=1;

        //Mesure IR
        ecrireInt(avancer_objet);

        if (avancer_objet == 1) //se déplace vers la cible
        {
            ecrireChar(avancer_IR_msg); //communication UART
            ird_value = mesure_ir_droit(); //mesure_IR
            irg_value = mesure_ir_gauche(); //mesure_IR

            distance = (irg_value + ird_value)/2;

            affichage_distance_led(distance);

            if (ird_value > 100) //sécurité contact asymétrique
            {
                reinit_pwm(); //Arret moteur - fin de séquence
                test_led(); //chenillard pour avertir de la fin
            }
            else if (irg_value > 100) //sécurité  contact asymétrique
            {
                reinit_pwm(); //Arret moteur - fin de séquence
                test_led(); //chenillard pour avertir de la fin
            }
        }

        //Valeur batterie sur Timer0
        
        ADCON0bits.CHS = 2; //selection de AN2
        ADCON0bits.ADON=1; //forcer activation convertisseur
        ADCON0bits.GO = 1; //debut mesure
        while(ADCON0bits.GO == 1)
        {}
        vbat_receive=ADRESH; //resultat de la mesure
        
        //Initialisation tableau batterie
        if (cmpt_batt<4) {
            bat[cmpt_batt] = vbat_receive;
            cmpt_batt++;
        }
        else {
            //Decalage tableau mesures
            bat[0] = bat[1];
            bat[1] = bat[2];
            bat[2] = bat[3];
            bat[3] = vbat_receive;

            //Moyenne tableau
            ubatt = (bat[0]+bat[1]+bat[2]+bat[3])/4;

            //Batterie ok
            if (ubatt >= 180) {
                ecrireInt(ubatt);
            }
            
            //Interruption batterie faible
            else {

                ecrireChar(message_batt_bad);
                ecrireInt(ubatt);
                
                reinit_pwm(); //arrêt des moteurs
                avant = 7; //interruption des manoeuvres
                
            }

            if (avancer_objet == 0) {
                affichage_led_batterie(ubatt);
            }
            
        }
   }

   else
       
       PORTBbits.RB5=0; //clignotement led

     
   //IT ADC
   if(PIR1bits.ADIF==1)
   {
      ADCON0bits.GO=0;
      PIR1bits.ADIF=0;
   }

   //Timer 2
   if(PIR1bits.TMR2IF){
   }
}
