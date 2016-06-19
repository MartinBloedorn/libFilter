/***
 * IIR Filter Library - Implementation
 *
 * Copyright (C) 2016  Martin Vincent Bloedorn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filters.h"

using namespace IIR;

// CONSTRUCTOR AND DESTRUCTOR * * * * * * * * * * * * * * *

Filter::Filter(float_t hz_, float_t ts_, ORDER od_) :
  ts( ts_ ),
  hz( hz_ ),
  od( od_ )
{
  init();
}

Filter::Filter() { }
Filter::~Filter() { }

// PUBLIC METHODS * * * * * * * * * * * * * * * * * * * * *

void Filter::init() {
  flush();
  f_err  = false;
  f_warn = false;

  switch((uint8_t)od) {

    case (uint8_t)ORDER::OD1:
        a  = 2.0*PI*hz;
        k1 = exp(-a*ts);
        k0 = 1.0 - k1;
      break;
    case (uint8_t)ORDER::OD2:
        a  = -PI*hz*SQRT2;
        b  =  PI*hz*SQRT2;
        k2 = ap(exp(2.0*ts*a));
        k1 = ap(2.0*exp(a*ts)*cos(b*ts));
        k0 = ap(1.0*KM - k1*KM + k2*KM);
      break;
    // B3Z = k0/(1 - (b1+b3)*z^-1 + (b2+b1*b3)*z^-2 - b2*b3*z^-3) = k0/(1 - k1*z^-1 + k2*z^-2 - k3*z^-3)
    case (uint8_t)ORDER::OD3:
        a  = -PI*hz;
        b  =  PI*hz*SQRT3;
        c  =  2*PI*hz;
        b3 = exp(-c*ts);
        b2 = exp(2.0*ts*a);
        b1 = 2.0*exp(a*ts)*cos(b*ts);
        k3 = ap(b2*b3);
        k2 = ap(b2 + b1*b3);
        k1 = ap(b1 + b3);
        k0 = ap(1.0*KM - b1*KM + b2*KM -b3*KM + b1*KM*b3 - b2*KM*b3);
      break;
    case (uint8_t)ORDER::OD4:
        a  = -0.3827*2.0*PI*hz;
        b  =  0.9238*2.0*PI*hz;
        c  = -0.9238*2.0*PI*hz;
        d  =  0.3827*2.0*PI*hz;
        b4 = exp(2.0*ts*c);
        b3 = 2.0*exp(c*ts)*cos(d*ts);
        b2 = exp(2.0*ts*a);
        b1 = 2.0*exp(a*ts)*cos(b*ts);
        k4 = ap(b2*b4);
        k3 = ap(b1*b4 + b2*b3);
        k2 = ap(b4 + b1*b3 + b2);
        k1 = ap(b1 + b3);
        k0 = ap(1.0*KM - k1*KM + k2*KM - k3*KM + k4*KM);
      break;
  }
}

float Filter::filterIn(float input) {
  if(f_err) return 0.0;

  for(uint8_t i=MAX_ORDER-1; i>0; i--)
    y[i] = y[i-1];

  switch((uint8_t)od) {
    case (uint8_t)ORDER::OD1:
        y[0] = k1*y[1] + k0*input;
      break;
    case (uint8_t)ORDER::OD2:
        y[0] = k1*y[1] - k2*y[2] + (k0*input)/KM;
      break;
    case (uint8_t)ORDER::OD3:
        y[0] = k1*y[1] - k2*y[2] + k3*y[3] + (k0*input)/KM;
      break;
    case (uint8_t)ORDER::OD4:
        y[0] = k1*y[1] - k2*y[2] + k3*y[3] - k4*y[4] + (k0*input)/KM;
      break;
    default:
        y[0] = input;
      break;
  }
  return y[0];
}

void Filter::flush() {
  for(uint8_t i=0; i<MAX_ORDER; i++) {
    u[i] = 0.0;
    y[i] = 0.0;
  }
}

void Filter::dumpParams() {
  uint8_t p = 6;
  Serial.println("Filter parameters:");
  Serial.print("ts\t= "); Serial.println(ts, p);
  Serial.print("hz\t= "); Serial.println(hz, p);
  Serial.print("od\t= "); Serial.println((uint8_t)od);
  Serial.print("a\t= ");  Serial.println(a, p);
  Serial.print("b\t= ");  Serial.println(b, p);
  Serial.print("c\t= ");  Serial.println(c, p);
  Serial.print("d\t= ");  Serial.println(d, p);
  Serial.print("e\t= ");  Serial.println(e, p);

  Serial.print("b1\t= ");  Serial.println(b1, p);
  Serial.print("b2\t= ");  Serial.println(b2, p);
  Serial.print("b3\t= ");  Serial.println(b3, p);
  Serial.print("b4\t= ");  Serial.println(b4, p);
  Serial.print("b5\t= ");  Serial.println(b5, p);

  Serial.print("k0\t= ");  Serial.println(k0, p);
  Serial.print("k1\t= ");  Serial.println(k1, p);
  Serial.print("k2\t= ");  Serial.println(k2, p);
  Serial.print("k3\t= ");  Serial.println(k3, p);
  Serial.print("k4\t= ");  Serial.println(k4, p);
  Serial.print("k5\t= ");  Serial.println(k5, p);
}

// PRIVATE METHODS  * * * * * * * * * * * * * * * * * * * *

float Filter::ap(float p) {
  f_err  = f_err  | (abs(p) <= EPS );
  f_warn = f_warn | (abs(p) <= WEPS);
  if(f_err) return 0.0; else return p;
}
