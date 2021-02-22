/* LCLV.cpp

   Arduino driver library for LCLV like Adafruit 3627 and 3330
  
   Copyright Henry (Hank) Dietz 2021.
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE or copy at
   https://www.boost.org/LICENSE_1_0.txt)

   Revision history:
   20200907	Original version, not structured as a library
   20210221	First packaging as an Arduino library on git
*/

#include "LCLV.h"

LCLV::LCLV(int pina, int pinb, uint8_t v, int hz, int an)
{
  // Set times from hz
  this->wait = 1000000L / hz;
  this->flip = micros();
  this->swap = 0;

  // Analog/PWM centered on 127, or digital
  this->an = an;
  
  // Set which pins to use and set level
  pinMode((this->pina = pina), OUTPUT);
  pinMode((this->pinb = pinb), OUTPUT);
  LCLV::set(v);
}

void
LCLV::shutter(unsigned long us)
{
  // designed to be as fast as possible
  unsigned long n = micros() + us;
  
  if (this->an) {
    // analog version
    analogWrite(this->pina, 127);
    analogWrite(this->pinb, 127);
  } else {
    // digital version
    digitalWrite(this->pina, LOW);
    digitalWrite(this->pinb, LOW);
  }
  
  // wait for us, spinning
  // no need to do AC waveform; it's PWM 0
  while (micros() < n) ;

  // force shutter closed
  LCLV::force();
}

void
LCLV::set(uint8_t v)
{
  // Analog of digital values?
  if (this->an) {
    // analog
    uint8_t a, b;
    b = (v - (a = v >> 1));
    this->pwma = 127 - a;
    this->pwmb = 127 + b;
  } else {
    // digital
    this->pwma = LOW;
    this->pwmb = ((v != 0) ? HIGH : LOW);
  }

  // make it so (going in current direction)
  LCLV::force();
}

void
LCLV::poll()
{
  unsigned long n = micros();
  if (n >= this->flip) {
    LCLV::force();
    this->flip = micros() + this->wait;
    this->swap = !(this->swap);   
  }
}

void
LCLV::force()
{
  // force AC signal, analog or digital
  if (this->an) {
    if (this->swap) {
      analogWrite(this->pina, this->pwmb);
      analogWrite(this->pinb, this->pwma);
    } else {
      analogWrite(this->pina, this->pwma);
      analogWrite(this->pinb, this->pwmb);
    }
  } else {
    if (this->swap) {
      digitalWrite(this->pina, this->pwmb);
      digitalWrite(this->pinb, this->pwma);
    } else {
      digitalWrite(this->pina, this->pwma);
      digitalWrite(this->pinb, this->pwmb);
    }
  }

  this->flip = micros() + this->wait;
  this->swap = !(this->swap);
}
