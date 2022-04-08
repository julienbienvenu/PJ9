/*
 * File:   main.c
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

#include <stdio.h>
#include <stdlib.h>
#include "MI2C.h"
#include <p18f2520.h>
#include "coreFunctions.h"
#include "interruption.h"
#include "initialisation.h"
#include "global.h"

#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

//init global vars
unsigned char marche=0;
unsigned char nbVmesure=0;
volatile unsigned long vbat=0;
unsigned int dutyCycleMoteurD=0;
unsigned int dutyCycleMoteurG=0;
unsigned float dutyCycleServo=99.99;
unsigned int timer1ticks=0;
unsigned int Tmr1StartTime=64611;
int avancer_objet; //phase : avancer vers l'objet
int boucle = 0; //variable d'incrémentation

extern unsigned int ordre_telecommande;

int tmp = 0; //entier pour fonction avancer_pwm
int position_IR = 5; //position meilleure résonnance IR

//Texte à communiquer via fonction message UART

char boucle_msg[30]="boucle \r\n";

volatile unsigned int led=0b11111111;
char i2c_isUsed=0;

void main(void) {

    // L'initialisation se fait avant l'ordre telecommande

    // Initialisation variables
    avancer_objet = 0;
    ordre_telecommande = 0;

    //Initialisation du robot
    initialisation_total();

    //allumage led test
    PORTBbits.RB5=1;
        
    while(1)
    {
        
        if (ordre_telecommande == 1) //aucune action sans telecommande
        {
            if (boucle == 0) //un seul balayage
            {
                // Rechercher position objet
                position_IR = balayage(); //balayage à 180°

                //Alignement objet
                rotation(position_IR - 1); //rotation vers la position de l'objet

                //Mise à jour des variables
                avancer_objet = 1; //démarre le capteur IR par interruption Timer0
                boucle = 1; //évite de rentrer dans cette boucle de nouveau

                //Avancer vers l'objet
                reinit_pwm();
                tmp = rotation_pwm(0,0); //avancer vers l'objet -> arret par IR sur Timer0
            }
        }
        
       
    }
}