/*
 * File:   coreFunctions.c
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

#include <p18cxxx.h>
#include "global.h"
#include "MI2C.h"
#include <stdlib.h>
#include <stdio.h>

int rotation_pwm(int dirg, int dird);
void ecrireChar(char c[30]);
void ecrireInt( int k);
void tempo(unsigned int T);
void reinit_pwm(void);
float moyenne(int batterie[4]);
void ecrireFloat( float k);
void telecommande(void);
void affichage_distance_led(int distance);
int mesure_ir_gauche(void);
int mesure_ir_droit(void);
void initialisation_total(void);

unsigned int irg_val;
unsigned int ird_val;
unsigned int distance_IR;
unsigned int ordre_telecommande; //0 -> arrêt, 1 -> action
unsigned volatile char touche[3]; //fonction lecture telecommande

char message[30]="Projet 9 \r\n";
char message_bis[30]="bienvenu bouton\r\n";
char message_init[30]="initialisation ok\r\n";
char message_led[30]="affichage leds \r\n";

int test = 0; //incrémentation des rotations du robot
int tour = 0; //incrémentation des différentes mesures IR
int meilleur_choix = 0; //indice de la position de tour avec meilleure résonnance
unsigned int i_tour = 0; //variable d'incrémentation
extern unsigned int temporisation;
extern unsigned int avant;
int moyenne_IR[18];

void initialisation_total(void)
{
    //Initialisations
    Initialisation();
    InitialiserMoteurs();

    //Communication UART
    ecrireChar(message);
    ecrireChar(message_bis);
    ecrireChar(message_init);//envoie message UART

    // Allumage des leds
    ecrireChar(message_led);
    test_led(); //chenillard
}

void tempo(unsigned int T){

    temporisation = 0;
    
    while (temporisation < 1)
    {

    }
}

//ecriture char UART
void ecrireChar(char c[30]){
    printf("%s",c);
}

//ecriture entier UART
void ecrireInt( int k){
    printf("vbatt = %d\r\n",k);
}

void ecrireFloat( float k){
    printf("vbatt = %f\r\n",k);
}

int rotation_pwm(int dirg, int dird)
{
    if (ordre_telecommande == 1)
    {
        //Communication UART
        char message1[30]="moteur_pwm\r\n";//message UART
        ecrireChar(message1);//message

        PORTAbits.RA6=dird; //ordre sens de rotation
        PORTAbits.RA7=dirg; //ordre sens de rotation

        dutyCycleMoteurD = 75; //75% par test sur le robot
        dutyCycleMoteurG = 75; //75% par test sur le robot

        CCPR1L = dutyCycleMoteurD * 1; //commande rotation
        CCPR2L = dutyCycleMoteurG * 1; //commande rotation
    }

    return 1;
}

void reinit_pwm(void)
{
    dutyCycleMoteurD = 0; //nulle pour arrêt rotation
    dutyCycleMoteurG = 0; //nulle pour arrêt rotation
    
    PORTAbits.RA6=0; //sens de rotation nulle par défaut
    PORTAbits.RA7=0; //sens de rotation nulle par défaut

    CCPR1L = dutyCycleMoteurD * 1; //commande rotation
    CCPR2L = dutyCycleMoteurG * 1; //commande rotation
}

int maximun_list(int tab[18])
{
    //Fonction pour définir l'indice maximun d'une liste
    
    int indice = 0;
    unsigned int i;

    for(i=0; i< 12; i++)
    {
        if (tab[i] > tab[indice])
            indice = i;
    }

    return indice;
}

int mesure_ir(void)
{
    ird_val = 0; //entier valeur_IR
    irg_val = 0; //entier valeur_IR
    
    ADCON0bits.CHS = 0; // Sélection de AN0
    ADCON0bits.ADON = 1; // Activation du convertisseur
    ADCON0bits.GO = 1; // debut mesure
    while(ADCON0bits.GO == 1) // Attente de la conversion par scrutation
    {}
    ird_val = ADRESH; // résultat de la mesure
    
    printf("Capteur gauche : %d \r\n",ird_val); //communication UART
    
    ADCON0bits.CHS = 1; // Sélection de AN1
    ADCON0bits.ADON = 1; // Activation du convertisseur
    ADCON0bits.GO = 1; // debut mesure
    while(ADCON0bits.GO == 1) // Attente de la conversion par scrutation
    {}
    irg_val = ADRESH; // résultat de la mesure
    
    printf("Capteur droit : %d \r\n",irg_val); //communication UART

    //Securite en cas de défaut d'un des deux IR (cf : pb de carte)
    if (irg_val > 180)
    {
         distance_IR = ird_val;
    }
    else if (ird_val > 180)
    {
         distance_IR = irg_val;
    }
    else
    {
        distance_IR = (irg_val + ird_val) / 2;
    }

    printf("Distance moyenne : %d \r\n",distance_IR); //communication UART
    return distance_IR;

}

int mesure_ir_droit(void)
{
    irg_val = 0; //entier valeur_IR

    // CAPTEUR GAUCHE
    //PIR1bits.TMR2IF=0;
    ADCON0bits.CHS = 1; // Sélection de AN1
    ADCON0bits.ADON = 1; // Activation du convertisseur
    ADCON0bits.GO = 1; // debut mesure
    while(ADCON0bits.GO == 1) // Attente de la conversion par scrutation
    {}
    irg_val = ADRESH; // résultat de la mesure

    printf("Capteur droit : %d \r\n",irg_val);

    return irg_val;
}

int mesure_ir_gauche(void)
{
    ird_val = 0; //entier valeur_IR
    
    ADCON0bits.CHS = 0; // Sélection de AN0
    ADCON0bits.ADON = 1; // Activation du convertisseur
    ADCON0bits.GO = 1; // debut mesure
    while(ADCON0bits.GO == 1) // Attente de la conversion par scrutation
    {}
    ird_val = ADRESH; // résultat de la mesure

    printf("Capteur gauche : %d \r\n",ird_val);

    return ird_val;
}

int balayage(void){
    while (test < 18) {
        if (test < 6) { //Rotation 90° à gauche
                tmp=rotation_pwm(0,1); //fonction de rotation

                led = 0b11111111; //affichage led
                Write_PCF8574(0x40, led); //affichage led

                tempo(4); //temporisation rotation
                reinit_pwm(); //arrêt moteur

                led = 0b00000000; //affichage led
                Write_PCF8574(0x40, led); //affichage led
                test++; //incrémentation balayage

                tempo(4); //attente pour séquencer les étapes
            }
        else if (test < 18) { //Rotation 180° à droite
                tmp=rotation_pwm(1,0); //fonction de rotation

                led = 0b11111111; //affichage led
                Write_PCF8574(0x40, led); //affichage led

                tempo(4); //temporisation rotation
                reinit_pwm(); //arrêt moteur

                led = 0b00000000; //affichage led
                Write_PCF8574(0x40, led); //affichage led
                test++; //incrémentation balayage

                moyenne_IR[tour] = mesure_ir(); //enregistrement mesure IR
                tour++; //incrémentation balayage

                tempo(4); //attente pour séquencer les étapes
            }
    }

    meilleur_choix = maximun_list(moyenne_IR); //choix objet
    meilleur_choix = 12 - meilleur_choix; //réglage orientation tableau
    printf("Angle de l'objet : %d \r\n", meilleur_choix);

    return meilleur_choix;
}

void rotation(int angle)
{
    // rotation vers l'objet choisis
    if (ordre_telecommande == 1) //vérification télécommande
    {
       for (i_tour=0; i_tour<angle; i_tour++)
       {
            tmp=rotation_pwm(0,1); //rotation PWM
            
            led = 0b00111111; //affichage led
            Write_PCF8574(0x40, led); //affichage led

            tempo(4); //temporisation rotation
            reinit_pwm();//arrêt moteurs

            led = 0b00000000; //affichage led
            Write_PCF8574(0x40, led); //affichage led
            test++; //incrémentation rotation
            ecrireInt(i_tour); //communication UART

            tempo(4); //attente pour séquencer les étapes
        }
    }
    
}

void telecommande(void)
{
    //Lire_i2c_Telecom(0xA2,touche); //fonction de MI2C.c
    //if(touche[1] == 0x33){ //condition touche
    if (ordre_telecommande == 0)
    {
        ordre_telecommande = 1; //interruption ordre telecommande
        printf("Demarrage telecommande \r\n"); //communication UART
    }
    else
    {
        ordre_telecommande = 0; //interruption ordre telecommande
        avant = 7; //interruption des manoeuvres
        test = 19; //interruption des manoeuvres
        reinit_pwm(); //arrêt moteur
        printf("Interruption telecommande \r\n"); //communication UART
    }
    }


void affichage_distance_led(int distance)
{
    if (distance > 100)
    {
        led = 0b11000000;
    }
    else if (distance > 80)
    {
        led = 0b11100000;
    }
    else if (distance > 60)
    {
        led = 0b11110000;
    }
    else if (distance > 40)
    {
        led = 0b11111000;
    }
    else if (distance > 20)
    {
        led = 0b11111100;
    }
    else if (distance > 0)
    {
        led = 0b11111110;
    }
        
    Write_PCF8574(0x40, led); //affichage led
    
}

void affichage_led_batterie(int uba)
{
    if (uba > 150)
    {
        led = 0b00111111;
    }
    else
    {
        led = 0b01111111;
    }

    Write_PCF8574(0x40, led);
}