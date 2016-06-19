/***
 * IIR Filter Library - Definitions and constants
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

#ifndef FILTERS_DEFS_H
#define FILTERS_DEFS_H

#define FCUTOFF_HZ     10.0;
#define TS_US          15000;
#define FILTER_ORDER   1;

typedef float float_t;
typedef int   int_t;


namespace IIR {
  const uint8_t MAX_ORDER = 5;
  enum class ORDER  : uint8_t {OD1 = 0, OD2, OD3, OD4, OD5};

  const float SQRT2 = sqrt(2.0);
  const float SQRT3 = sqrt(3.0);
  const float SQRT5 = sqrt(5.0);

  const float EPS   = 0.00001;    // Tolerancia para constantes numericasa
  const float WEPS  = 0.00010;    // Limite para emissão de aviso de degeneração da constante
  const float KM    = 100.0;      // Pré-multiplicador para reduzir problemas de representação numérica
}



#endif //FILTERS_DEFS_H
