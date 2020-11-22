/*
 * main.c
 *
 *  Created on: ? ’›— ???? Â‹
 *      Author: user
 */

#define  Trigger_pin	PA0	/* Trigger pin */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void  go_straight (void) ;
void go_back (void) ;
void turn_right (void) ;
void turn_left (void) ;
void stop (void) ;
void timer0_init_PWM (void);
void timer2_init_PWM (void);



int main (void)
{



 DDRC |= (1<<3);      //   configure the MOTOR pin to be output pin.
 DDRC |= (1<<4);
 DDRC |= (1<<5);
 DDRC |= (1<<6);

 	 timer0_init_PWM ();
 	 timer2_init_PWM ();

   stop () ;  //Initialization MOTORE is off at the beginning


   	  	long count;
   	  	double distance;

   	    TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
   	 	TCCR1A = 0;		/* Set all bit to zero Normal operation */

   	  	DDRA |= (1<<Trigger_pin) ;
   	  	PORTD &=(1<<6);


 	 while (1)
 	 {

 				PORTA |= (1 << Trigger_pin);
 				_delay_us(10);                 // Give 10us trigger
 				PORTA &= (~(1 << Trigger_pin));



 			    TCNT1 = 0;   	// Clear Timer counter
 				TCCR1B = 0x41;	// Capture on rising edge, No prescaler
 				TIFR = 1<<ICF1;	// Clear ICP flag (Input Capture flag)
 				TIFR = 1<<TOV1;	// Clear Timer Overflow flag

 				/*Calculate width of Echo by Input Capture (ICP) */

 				while ((TIFR & (1 << ICF1)) == 0);// Wait for rising edge
 				TCNT1 = 0;				// Clear Timer counter
 				TCCR1B = 0x01;			// Capture on falling edge, No prescaler
 				TIFR = 1<<ICF1;			// Clear ICP flag (Input Capture flag)
 				TIFR = 1<<TOV1;      	// Clear Timer Overflow flag

 				while ((TIFR & (1 << ICF1)) == 0);   // Wait for falling edge
 				count = ICR1 ;	// Take count
 				distance = (double)count /466.47;

 		 if ( distance<30)
 		 {
 			turn_right();

 		 }
 		 else
 		 {
 			 go_straight();
 		 }
 	 }
}


void  go_straight (void)
{
	PORTC |=  (1<<3);
    PORTC &= (~(1<<4));
 	PORTC |=  (1<<5);
    PORTC &= (~(1<<6));
}

void go_back (void)
{
	PORTC &= (~(1<<3));
    PORTC|= (1<<4);
	PORTC &= (~(1<<5));
	PORTC|= (1<<6);
}

void stop (void)
{
	PORTC &= (~(1<<3));
	PORTC &= (~(1<<4));
	PORTC &= (~(1<<5));
	PORTC &= (~(1<<6));
}

void turn_right (void)
{

	PORTC &= (~(1<<3));
    PORTC &= (~(1<<4));
 	PORTC |=  (1<<5);
    PORTC &= (~(1<<6));
}

void turn_left (void)
{
	PORTC |=  (1<<3);
	PORTC &= (~(1<<4));
	PORTC &= (~(1<<5));
	PORTC &= (~(1<<6));
}

void timer0_init_PWM (void)
{

	TCCR0 |= (1<<3) |(1<<6) |(1<<5) | (1<<1)  ;
	 OCR0 = 200 ;
	 DDRB |=(1<<3);


}

void timer2_init_PWM (void)
{
	TCCR2 |= (1<<3) |(1<<6) |(1<<5) | (1<<1)  ;
	OCR2 = 200 ;
	DDRD |=(1<<7);
}


