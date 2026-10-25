#include <avr/io.h>
#include <avr/interrupt.h>

ISR(INT0_vect){
    PINB = (1<<PB5);//toggle led
}

int main(){

    DDRB |= (1<<PB5);//data direction
    DDRD &= ~(1<<PD2);//makes it an input
    PORTD |= (1<<PD2);//enable internal pullup

    //interrupt on the falling edge
    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC00);

    //External Interrupt Mask Register
    EIMSK |= (1<<INT0);

    sei();

    while(1);


}