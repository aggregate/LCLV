# LCLV
Arduino driver library for LCLV like Adafruit 3627 and 3330

A single-element Liquid Crystal Light Valve (LCLV) display, such
as the Adafruit 3627 or 3330, is an inexpensive device that uses
LC material sandwiched between two polarizers to allow
electrical control of opacity by applying a voltage across two
leads. Measurements show that these LCLV always reduce
transmissivity by at least 2EV (2 stops), but changing the
voltage difference across the LCLV from 0V to 5V causes a
further drop of approximately 9EV. Intermediate voltages allow
smoothly varying transmissivity from -2EV to -11EV.

The light passing through an LCLV leaves linearly polarized, but
with minimal diffusion.  Thus, the LCLV can be placed in the
optical path of a lens, acting as either a programmable neutral
denisty (ND) filter or a global electronic shutter. Although
somewhat longer shutter open times are recommended, shutter
speeds faster than 1ms (1/1000s) were experimentally proven to
be feasible using digital control.

This library for Arduino-compatible systems provides an LCLV
class providing for AC control of LCLV displays, each using a
pair of output pins.  Although it is possible to drive an LC
material by simply connecting one contact to ground and the
other to a controllable output pin, always untwisting in the
same direction will slowly degrade the molecular structure of
the liquid crystal. It is awkward to drive a signal to a
negative volatage, thus, in this library, the AC drive is
simulated using two output pins such that the the polarity of
the drive signal is alternated at approximately the frequency
specified.

Arduinos typically support analog output using analogWrite(),
which often is implemented using PWM (pulse width modulation) at
between 490Hz and 980Hz.  It was experimentally determined that
the PWM output does indeed result in fairly consistent
intermediate transmissivity for the LCLV. Using a 5V Pro Micro,
going from PWM values of 0 to approximately 127 brought a
roughly linear decrease in EV transmission, dropping from -2EV
to approximately -10EV. PWM values from 128 to about 208 reduced
transmission from -10EV to -11EV, and no further change was seen
going to a PWM value of 255. However, requesting a PWM value of
255 might reach maximum opacity more quickly, so we recommend
use of 255 (continuous digital logic 1 output) for making the
LCLV behave as a closed shutter.

The LCLV class takes five parameters:

* pina: Output pin connected to one lead of the LCLV
* pinb: Output pin connected to the other LCLV lead
* v: Analog/PWM value from 0..255 or digital value 0 or non-0
* hz: Desired AC frequency, in Hertz (cycles/second)
* an: Analog control or digital?

The an parameter requires a little explanation. Analog outputs
are commonly more linear in the middle of their range than near
the minimum and maximum values, thus, if the an parameter is
non-0, the PWM average value will be 127 instead of 0. For
example, requesting a PWM value of 21 with an non-0 will
alternate between 117,138 and 138,117 using analogWrite() to the
pins. If an is 0, then v==0 will use digitalWrite() to output
LOW, LOW and v>0 would alternate between LOW, HIGH and HIGH, LOW.

This driver was created by Professor Hank Dietz,
hankd@engr.uky.edu, at the University of Kentucky in support of
his Electronic Imaging 2021 research paper: "Programmable liquid
crystal apertures and filters for photographic lenses." There is
a web page, http://aggregate.org/DIT/LC, containing a preprint
of the paper, presentation slides, and other relevant materials.

This software is distributed under:

          Copyright Henry (Hank) Dietz, 2021.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE or copy at
          https://www.boost.org/LICENSE_1_0.txt)
