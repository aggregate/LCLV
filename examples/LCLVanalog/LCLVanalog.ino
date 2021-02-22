/* LCLVanalog.ino

   Analog LCLV control example using the
   Arduino driver library for LCLV like Adafruit 3627 and 3330
  
   Copyright Henry (Hank) Dietz 2021.
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE or copy at
   https://www.boost.org/LICENSE_1_0.txt)
*/

#include <LCLV.h>

#define	STEPT	100000L	  // time per opacity step, in us
#define	WAITT	2000000L  // time between phases, in us

#define	PINA	9	  // one lead of the LCLV
#define	PINB	10	// the other lead
#define	V     0	  // initial value (minimum opacity)
#define	HZ    100 // AC target frequency for LCLV control
#define	AN    0   // use analog control

// create an LCLV controller
LCLV lclv = LCLV(PINA, PINB, V, HZ, AN);

void setup() {
  //Initialize serial
  Serial.begin(115200);
  while (!Serial) ; 	// wait for connection

  // prints title with ending line break
  Serial.println("Liquid Crystal Light Valve analog control demo...");
}

void loop() {
  unsigned long t;
  int i;

  // do a simple ramp
  Serial.println("Interlaced ramp:");
  for (i=0; i<255; ++i) {
    lclv.set(i);
    t = micros() + STEPT;
    Serial.print("lclv.set(");
    Serial.print(i);
    Serial.println(")");
    while (micros() < t) lclv.poll(); // polling makes AC waveform
  }

  // wait between phases
  lclv.set(255);
  t = micros() + WAITT;
  Serial.println("Shutter test:");
  while (micros() < t) lclv.poll(); // polling makes AC waveform

  // show shutter speeds from 1s to 1/256s
  for (i=1; i<257; i+=i) {
    unsigned long s = 1000000L / i;
    Serial.print("lclv.shutter(");
    Serial.print(s);
    Serial.print(") // 1/");
    Serial.print(i);
    Serial.println("s");
    lclv.shutter(s);

    // wait between phases
    t = micros() + WAITT;
    while (micros() < t) lclv.poll(); // polling makes AC waveform
  }

  // wait between phases
  lclv.set(255);
  t = micros() + WAITT;
  Serial.println("Shutter test:");
  while (micros() < t) lclv.poll(); // polling makes AC waveform
}
