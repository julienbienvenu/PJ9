/* 
* File:   coreFunctions.h
 * Author: julie
 *
 * Created on 29 avril 2021, 18:31
 */

#ifndef COREFUNCTIONS_H
#define	COREFUNCTIONS_H

int rotation_pwm(int dirg, int dird);
void ecrireChar(char c[30]);
void ecrireInt( int k);
void tempo(unsigned int T);
void reinit_pwm(void);
void ecrireFloat( float k);
int mesure_ir(void);
int balayage(void);
int maximun_list(int tab[18]);
void rotation(int angle);
void telecommande(void);
void reinit_pwm(void);
void affichage_distance_led(int distance);
int mesure_ir_gauche(void);
int mesure_ir_droit(void);
void initialisation_total(void);


#endif	/* COREFUNCTIONS_H */

