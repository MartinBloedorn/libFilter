/***
 * IIR Filter Library - Header
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

#ifndef FILTERS_H
#define FILTERS_H

/// Has to be executed on Arduino IDE > 1.6.7
#include <Arduino.h>

#include "filters_defs.h"

using namespace IIR;

class Filter {
public:

  /** \brief Constroi um filtro.
   *
   */
  Filter(float_t hz_, float_t ts_, ORDER od_);
  ~Filter();

  float filterIn(float input);

  void flush();

#ifndef CONST_COMPILE_TIME
  Filter(); ///< Construtor vazio

  void init(bool doFlush=true);

  void setSamplingTime(float ts_, bool doFlush=true) { ts = ts_; init(doFlush); }
  void setCutoffFreqHZ(float hz_, bool doFlush=true) { hz = hz_; init(doFlush); }
  void setOrder(ORDER od_, bool doFlush=true)        { od = od_; init(doFlush); }
#endif

  bool isInErrorState() { return f_err;  }
  bool isInWarnState()  { return f_warn; }
  void dumpParams();

private:
  float_t ts;
  float_t hz;
  ORDER od;

  float_t a, b, c, d, e;
  float_t k0, k1, k2, k3, k4, k5;
  float b1, b2, b3, b4, b5;
  float y[MAX_ORDER], u[MAX_ORDER];

  bool f_err, f_warn;

  float ap(float p); ///< Assert Parameter
};


#endif //FILTERS_H
