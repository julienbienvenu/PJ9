/*
 * File:   initialisation.c
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

#include <p18cxxx.h>
#include "global.h"
#include "coreFunctions.h"
#include "MI2C.h"

void Initialisation(){
    //Horloge à 8Mhz
    OSCCONbits.IRCF=7;

    INTCONbits.INT0IE = 1;    // Validation interruptions
    INTCON2bits.INTEDG0 = 0;  // Front descendant interruption 0

    //ADC
    TRISBbits.RB5 = 0;
    ADCON0bits.CHS3 = 0;     // ADC converti l'entree AN0 pour IR
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;

    ADCON1bits.VCFG=0;       // Vref- = Vss & Vref+ =VDD
    ADCON1bits.PCFG=12;      // Port AN2 opened
    ADCON2bits.ADCS=6;       // Fosc/8 1
    ADCON2bits.ACQT=7;       // 4 TAD 3
    ADCON2bits.ADFM=0;       // Justification gauche
    ADCON0bits.ADON=1;       // Activation du convertisseur
    PIR1bits.ADIF=0;         // Reset flag
    PIE1bits.ADIE=1;         // Autorisation interruption
    INTCON2bits.INTEDG0 = 0; // Front descendant
    INTCON2bits.INTEDG1 = 0; // Front descendant
    ADCON0bits.GO=1;         // Démarrer convertisseur

    //config entrée analogique
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 0;
    ADCON1bits.PCFG0 = 0;

    //Timer 0
    T0CONbits.T08BIT=0;      // 16 bits timer
    T0CONbits.T0CS=0;        // Horloge interne
    T0CONbits.PSA=0;         // Prescaler ON
    T0CONbits.T0PS=7;        // Prescalar sur 255 bits
    T0CONbits.TMR0ON=1;      // Mise en marche du Timer
    INTCONbits.TMR0IE=1;     // Interruption
    TMR0H = 0xFD;
    TMR0L = 0xAF;            // Rechargement 15535


    //Initialisation I2C
    MI2CInit();

    //UART
    TXSTAbits.SYNC=0;        // Mode asynchrone
    TXSTAbits.BRGH=1;        // High speed
    BAUDCONbits.BRG16=0;     // Baud rate 8 bits
    SPBRG=51;
    RCSTAbits.SPEN=1;
    TXSTAbits.TXEN=1;        // Autoriser transmission
    PIR1bits.TXIF=0;
    PIE1bits.TXIE=0;
    RCSTAbits.CREN=1;        // Autoriser réception

    //IR
    TRISAbits.RA0 = 1;       // Activation entrée IRD
    TRISAbits.RA1 = 1;       // Activation entrée IRG
    TRISBbits.RB1 = 0;       // Activation décenchement IR

    TRISBbits.RB0 = 1;       // Activation entrée télécommande

    //Interruption coeur et peripherique
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}


void InitialiserMoteurs(void){
    TRISAbits.RA6=0; //DIRD sortie
    TRISAbits.RA7=0; // DIRG sortie
    TRISAbits.RA4=1; //acquisition moteur droit
    TRISCbits.RC0=1; //acquisition moteur gauche

    /*INIT Timer2 Configuration E/S*/
    TRISCbits.RC1 = 0;          // RC1 en sortie PWM droite
    TRISCbits.RC2 = 0;          // RC2 en sortie PWM gauche

    /* Configuration TIMER2 */
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;      // CLK /4
    PR2 = 124;                  // PR2 = 499
    T2CONbits.T2OUTPS = 9;      // postscaler = 9

    /* Reglage rapport cyclique */
    CCP1CONbits.DC1B0 = 1;
    CCP1CONbits.DC1B1 = 1;
    CCP2CONbits.DC2B0 = 1;
    CCP2CONbits.DC2B1 = 1;

    /* Selection Mode PWM */
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;

    /* Configuration interruption TMR2*/
    PIE1bits.TMR2IE=0;       // Validation TMR2IF (TMR2IP =1 par défault)
    INTCONbits.PEIE=1;       //Validation des interruptions des périphériques
    T2CONbits.TMR2ON = 1;    //Lance le moteur
    CCPR1L = dutyCycleMoteurD * 2.5;
    CCPR2L = dutyCycleMoteurG * 2.5;

    INTCONbits.GIE=1;  // Validation globale des INT
}

void test_led(void)
{
    char info_led[30]="fin test led\r\n"; //communication UART

    led = 0b11111110;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11111101;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11111011;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11110111;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11101111;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11011111;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b10111111;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b01111111;//allumage led
    Write_PCF8574(0x40, led);
    tempo(1);

    led = 0b11111111;//allumage led
    Write_PCF8574(0x40, led);

    ecrireChar(info_led);
}