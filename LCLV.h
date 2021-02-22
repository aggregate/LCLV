/* LCLV.h

   Arduino driver library for LCLV like Adafruit 3627 and 3330
  
   Copyright Henry (Hank) Dietz 2021.
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE or copy at
   https://www.boost.org/LICENSE_1_0.txt)

   Revision history:
   20200907	Original version, not structured as a library
   20210221	First packaging as an Arduino library on git
*/

#ifndef	_LCLV_H_
#define	_LCLV_H_

#include <Arduino.h>

// library interface description
class LCLV {
  public:
    // constructors
    LCLV(int pina, int pinb, unsigned char v, int hz, int an);

    // actions
    void shutter(unsigned long us); // open shutter for us
    void set(uint8_t v);            // set PWM level, 0=light, 255=dark
    void poll();                    // flip a,b if time for it
    void force();                   // force pins to match internal state
    
  private:
    int pina, pinb;       // Pin numbers
    uint8_t pwma, pwmb;   // PWM settings for pina, pinb
    int an;               // Analog PWM centered on 127 or digital on 0?
    int swap;             // swap a,b?
    unsigned long flip;   // next time to flip
    unsigned long wait;   // how long to wait between flips
};

#endif
