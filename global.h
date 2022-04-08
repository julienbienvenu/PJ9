/*
 * File:   global.h
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

//variables globales
#ifndef GLOBAL_H
#define	GLOBAL_H

extern unsigned int dutyCycleMoteurD; //rotation_pwm
extern unsigned int dutyCycleMoteurG;
extern int tmp; //rotation_pwm (utile pour la suite)
extern volatile unsigned int led; //affichage leds

/* Initialisation variables moteurs*/
extern volatile int PWM_value = 0;
extern volatile int PWM_d_time = 0;
extern volatile int PWM_g_time = 0;

extern volatile int num_rot = 0;
extern volatile int IRG[18] ;
extern volatile int IRD[18] ;

/* Initialisation variables IR*/
extern unsigned int ird_val = 0;
extern unsigned int irg_val = 0;
extern int avancer_objet;
/* Initialisation incrémentation*/
extern int inc = 0; // initialisation varibla d'incrémentation

#endif	/* GLOBAL_H */

