/***
 * HighPass_RemoveTrend Example
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

// 12 bit sine wave; spans 256 samples, centered at 512 (DC)
const uint16_t sine12bits_256samples[] = {
  512,525,537,550,562,575,587,600,612,624,637,649,661,673,685,697,708,720,731,743,754,765,776,786,797,
  807,817,827,837,847,856,865,874,883,892,900,908,916,924,931,938,945,951,958,964,969,975,980,985,990,
  994,998,1002,1005,1008,1011,1014,1016,1018,1020,1021,1022,1023,1023,1023,1023,1022,1021,1020,1019,
  1017,1015,1012,1010,1007,1003,1000,996,992,987,983,978,972,967,961,955,948,941,934,927,920,912,904,
  896,888,879,870,861,852,842,832,822,812,802,792,781,770,759,748,737,726,714,702,691,679,667,655,643,
  631,618,606,593,581,569,556,543,531,518,506,493,481,468,455,443,431,418,406,393,381,369,357,345,333,
  322,310,298,287,276,265,254,243,232,222,212,202,192,182,172,163,154,145,136,128,120,112,104,97,90,
  83,76,69,63,57,52,46,41,37,32,28,24,21,17,14,12,9,7,5,4,3,2,1,1,1,1,2,3,4,6,8,10,13,16,19,22,26,30,
  34,39,44,49,55,60,66,73,79,86,93,100,108,116,124,132,141,150,159,168,177,187,197,207,217,227,238,
  248,259,270,281,293,304,316,327,339,351,363,375,387,400,412,424,437,449,462,474,487,499,512
};

// 12 bit sine wave; spans 100 samples, centered at 512 (DC) 
const uint16_t sine12bits_100samples[] = {
  512,544,577,609,640,671,702,732,760,788,815,840,865,887,909,928,946,962,977,989,1000,1009,1015,1020,
  1022,1023,1021,1018,1012,1005,995,983,970,955,937,919,898,876,853,828,802,774,746,717,687,656,625,
  593,561,528,496,463,431,399,368,337,307,278,250,222,196,171,148,126,105,87,69,54,41,29,19,12,6,3,
  1,2,4,9,15,24,35,47,62,78,96,115,137,159,184,209,236,264,292,322,353,384,415,447,480,512
};

// Creating high-pass filter; maximum order is 2 
const float cutoff_freq   = 3.0;   //Cutoff frequency in Hz
const float sampling_time = 0.001; //Sampling time in seconds.
Filter fhp(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);

int s1i = 0, s2i = 0;
float sign_raw, sign_filt;

// View with serial plotter
void setup() {
  Serial.begin(115200);
}

void loop() {
  // Composing a fast (10hz) signal over a slow one (~0.4hz, trend), both with DC components
  for(int r=0; r<10; r++) {
    sign_raw  = 3.0*(float)sine12bits_256samples[s1i] + sine12bits_100samples[s2i];
    s2i = ++s2i % 99;
    
    // High-pass filter removes low frequency trend and DC component
    sign_filt = fhp.filterIn(sign_raw);

    // Loop time has to be approximately equal to selected sampling period
    delay(1); 

    // View with serial plotter 
    Serial.print(sign_raw); // raw signal (signal of interest + trend + DC)
    Serial.print(",");
    Serial.print(sign_filt); // filtered signal (signal of interest, no trend, no DC)
    Serial.print(",");
    Serial.print(sine12bits_100samples[s2i]); // original signal of interest (has DC component)
    Serial.println(",0.0,5000.0"); // just setting a "grid" to make visualization easier
  }
  s1i = ++s1i % 255;
}

