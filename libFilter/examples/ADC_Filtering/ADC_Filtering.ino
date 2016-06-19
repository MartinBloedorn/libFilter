/***
 * ADC_Filtering Example
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

#include <filters.h>

const float cutoff_freq   = 20.0;  //Cutoff frequency in Hz
const float sampling_time = 0.005; //Sampling time in seconds.
IIR::ORDER  order  = IIR::ORDER::OD3; // Order (OD1 to OD4)

// Low-pass filter
Filter f(cutoff_freq, sampling_time, order);

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  // Enable pull-ups if necessary
  digitalWrite(A0, HIGH);
}

void loop() {
  int value = analogRead(0);
  float filteredval = f.filterIn(value);
  //View with Serial Plotter
  Serial.print(value, DEC);
  Serial.print(",");
  Serial.println(filteredval, 4);
  delay(5); // Loop time will approx. match the sampling time.
}
