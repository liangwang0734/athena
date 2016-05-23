//======================================================================================
// Athena++ astrophysical MHD code
// Copyright (C) 2014 James M. Stone  <jmstone@princeton.edu>
//
// This program is free software: you can redistribute and/or modify it under the terms
// of the GNU General Public License (GPL) as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of GNU GPL in the file LICENSE included in the code
// distribution.  If not see <http://www.gnu.org/licenses/>.
//======================================================================================

//======================================================================================
//! \file cgk_utils.cpp 
//  \brief namespace containing cgk utilities.
//======================================================================================
#include "cgk_utils.hpp"
#include <math.h>
#include <vector>     // vector container
#include <sstream>    // stringstream
#include <string>     // c_str()
#include <iostream>   // endl
#include <stdexcept>  // std::runtime_error()

static int get_Tidx(const Real temp);
static const int NTBL=801;
//tables
static Real T1_tbl[NTBL] = {
         1.00000000e+01,   1.02329299e+01,   1.04712855e+01,
         1.07151931e+01,   1.09647820e+01,   1.12201845e+01,
         1.14815362e+01,   1.17489755e+01,   1.20226443e+01,
         1.23026877e+01,   1.25892541e+01,   1.28824955e+01,
         1.31825674e+01,   1.34896288e+01,   1.38038426e+01,
         1.41253754e+01,   1.44543977e+01,   1.47910839e+01,
         1.51356125e+01,   1.54881662e+01,   1.58489319e+01,
         1.62181010e+01,   1.65958691e+01,   1.69824365e+01,
         1.73780083e+01,   1.77827941e+01,   1.81970086e+01,
         1.86208714e+01,   1.90546072e+01,   1.94984460e+01,
         1.99526231e+01,   2.04173794e+01,   2.08929613e+01,
         2.13796209e+01,   2.18776162e+01,   2.23872114e+01,
         2.29086765e+01,   2.34422882e+01,   2.39883292e+01,
         2.45470892e+01,   2.51188643e+01,   2.57039578e+01,
         2.63026799e+01,   2.69153480e+01,   2.75422870e+01,
         2.81838293e+01,   2.88403150e+01,   2.95120923e+01,
         3.01995172e+01,   3.09029543e+01,   3.16227766e+01,
         3.23593657e+01,   3.31131121e+01,   3.38844156e+01,
         3.46736850e+01,   3.54813389e+01,   3.63078055e+01,
         3.71535229e+01,   3.80189396e+01,   3.89045145e+01,
         3.98107171e+01,   4.07380278e+01,   4.16869383e+01,
         4.26579519e+01,   4.36515832e+01,   4.46683592e+01,
         4.57088190e+01,   4.67735141e+01,   4.78630092e+01,
         4.89778819e+01,   5.01187234e+01,   5.12861384e+01,
         5.24807460e+01,   5.37031796e+01,   5.49540874e+01,
         5.62341325e+01,   5.75439937e+01,   5.88843655e+01,
         6.02559586e+01,   6.16595002e+01,   6.30957344e+01,
         6.45654229e+01,   6.60693448e+01,   6.76082975e+01,
         6.91830971e+01,   7.07945784e+01,   7.24435960e+01,
         7.41310241e+01,   7.58577575e+01,   7.76247117e+01,
         7.94328235e+01,   8.12830516e+01,   8.31763771e+01,
         8.51138038e+01,   8.70963590e+01,   8.91250938e+01,
         9.12010839e+01,   9.33254301e+01,   9.54992586e+01,
         9.77237221e+01,   1.00000000e+02,   1.02329299e+02,
         1.04712855e+02,   1.07151931e+02,   1.09647820e+02,
         1.12201845e+02,   1.14815362e+02,   1.17489755e+02,
         1.20226443e+02,   1.23026877e+02,   1.25892541e+02,
         1.28824955e+02,   1.31825674e+02,   1.34896288e+02,
         1.38038426e+02,   1.41253754e+02,   1.44543977e+02,
         1.47910839e+02,   1.51356125e+02,   1.54881662e+02,
         1.58489319e+02,   1.62181010e+02,   1.65958691e+02,
         1.69824365e+02,   1.73780083e+02,   1.77827941e+02,
         1.81970086e+02,   1.86208714e+02,   1.90546072e+02,
         1.94984460e+02,   1.99526231e+02,   2.04173794e+02,
         2.08929613e+02,   2.13796209e+02,   2.18776162e+02,
         2.23872114e+02,   2.29086765e+02,   2.34422882e+02,
         2.39883292e+02,   2.45470892e+02,   2.51188643e+02,
         2.57039578e+02,   2.63026799e+02,   2.69153480e+02,
         2.75422870e+02,   2.81838293e+02,   2.88403150e+02,
         2.95120923e+02,   3.01995172e+02,   3.09029543e+02,
         3.16227766e+02,   3.23593657e+02,   3.31131121e+02,
         3.38844156e+02,   3.46736850e+02,   3.54813389e+02,
         3.63078055e+02,   3.71535229e+02,   3.80189396e+02,
         3.89045145e+02,   3.98107171e+02,   4.07380278e+02,
         4.16869383e+02,   4.26579519e+02,   4.36515832e+02,
         4.46683592e+02,   4.57088190e+02,   4.67735141e+02,
         4.78630092e+02,   4.89778819e+02,   5.01187234e+02,
         5.12861384e+02,   5.24807460e+02,   5.37031796e+02,
         5.49540874e+02,   5.62341325e+02,   5.75439937e+02,
         5.88843655e+02,   6.02559586e+02,   6.16595002e+02,
         6.30957344e+02,   6.45654229e+02,   6.60693448e+02,
         6.76082975e+02,   6.91830971e+02,   7.07945784e+02,
         7.24435960e+02,   7.41310241e+02,   7.58577575e+02,
         7.76247117e+02,   7.94328235e+02,   8.12830516e+02,
         8.31763771e+02,   8.51138038e+02,   8.70963590e+02,
         8.91250938e+02,   9.12010839e+02,   9.33254301e+02,
         9.54992586e+02,   9.77237221e+02,   1.00000000e+03,
         1.02329299e+03,   1.04712855e+03,   1.07151931e+03,
         1.09647820e+03,   1.12201845e+03,   1.14815362e+03,
         1.17489755e+03,   1.20226443e+03,   1.23026877e+03,
         1.25892541e+03,   1.28824955e+03,   1.31825674e+03,
         1.34896288e+03,   1.38038426e+03,   1.41253754e+03,
         1.44543977e+03,   1.47910839e+03,   1.51356125e+03,
         1.54881662e+03,   1.58489319e+03,   1.62181010e+03,
         1.65958691e+03,   1.69824365e+03,   1.73780083e+03,
         1.77827941e+03,   1.81970086e+03,   1.86208714e+03,
         1.90546072e+03,   1.94984460e+03,   1.99526231e+03,
         2.04173794e+03,   2.08929613e+03,   2.13796209e+03,
         2.18776162e+03,   2.23872114e+03,   2.29086765e+03,
         2.34422882e+03,   2.39883292e+03,   2.45470892e+03,
         2.51188643e+03,   2.57039578e+03,   2.63026799e+03,
         2.69153480e+03,   2.75422870e+03,   2.81838293e+03,
         2.88403150e+03,   2.95120923e+03,   3.01995172e+03,
         3.09029543e+03,   3.16227766e+03,   3.23593657e+03,
         3.31131121e+03,   3.38844156e+03,   3.46736850e+03,
         3.54813389e+03,   3.63078055e+03,   3.71535229e+03,
         3.80189396e+03,   3.89045145e+03,   3.98107171e+03,
         4.07380278e+03,   4.16869383e+03,   4.26579519e+03,
         4.36515832e+03,   4.46683592e+03,   4.57088190e+03,
         4.67735141e+03,   4.78630092e+03,   4.89778819e+03,
         5.01187234e+03,   5.12861384e+03,   5.24807460e+03,
         5.37031796e+03,   5.49540874e+03,   5.62341325e+03,
         5.75439937e+03,   5.88843655e+03,   6.02559586e+03,
         6.16595002e+03,   6.30957344e+03,   6.45654229e+03,
         6.60693448e+03,   6.76082975e+03,   6.91830971e+03,
         7.07945784e+03,   7.24435960e+03,   7.41310241e+03,
         7.58577575e+03,   7.76247117e+03,   7.94328235e+03,
         8.12830516e+03,   8.31763771e+03,   8.51138038e+03,
         8.70963590e+03,   8.91250938e+03,   9.12010839e+03,
         9.33254301e+03,   9.54992586e+03,   9.77237221e+03,
         1.00000000e+04,   1.02329299e+04,   1.04712855e+04,
         1.07151931e+04,   1.09647820e+04,   1.12201845e+04,
         1.14815362e+04,   1.17489755e+04,   1.20226443e+04,
         1.23026877e+04,   1.25892541e+04,   1.28824955e+04,
         1.31825674e+04,   1.34896288e+04,   1.38038426e+04,
         1.41253754e+04,   1.44543977e+04,   1.47910839e+04,
         1.51356125e+04,   1.54881662e+04,   1.58489319e+04,
         1.62181010e+04,   1.65958691e+04,   1.69824365e+04,
         1.73780083e+04,   1.77827941e+04,   1.81970086e+04,
         1.86208714e+04,   1.90546072e+04,   1.94984460e+04,
         1.99526231e+04,   2.04173794e+04,   2.08929613e+04,
         2.13796209e+04,   2.18776162e+04,   2.23872114e+04,
         2.29086765e+04,   2.34422882e+04,   2.39883292e+04,
         2.45470892e+04,   2.51188643e+04,   2.57039578e+04,
         2.63026799e+04,   2.69153480e+04,   2.75422870e+04,
         2.81838293e+04,   2.88403150e+04,   2.95120923e+04,
         3.01995172e+04,   3.09029543e+04,   3.16227766e+04,
         3.23593657e+04,   3.31131121e+04,   3.38844156e+04,
         3.46736850e+04,   3.54813389e+04,   3.63078055e+04,
         3.71535229e+04,   3.80189396e+04,   3.89045145e+04,
         3.98107171e+04,   4.07380278e+04,   4.16869383e+04,
         4.26579519e+04,   4.36515832e+04,   4.46683592e+04,
         4.57088190e+04,   4.67735141e+04,   4.78630092e+04,
         4.89778819e+04,   5.01187234e+04,   5.12861384e+04,
         5.24807460e+04,   5.37031796e+04,   5.49540874e+04,
         5.62341325e+04,   5.75439937e+04,   5.88843655e+04,
         6.02559586e+04,   6.16595002e+04,   6.30957344e+04,
         6.45654229e+04,   6.60693448e+04,   6.76082975e+04,
         6.91830971e+04,   7.07945784e+04,   7.24435960e+04,
         7.41310241e+04,   7.58577575e+04,   7.76247117e+04,
         7.94328235e+04,   8.12830516e+04,   8.31763771e+04,
         8.51138038e+04,   8.70963590e+04,   8.91250938e+04,
         9.12010839e+04,   9.33254301e+04,   9.54992586e+04,
         9.77237221e+04,   1.00000000e+05,   1.02329299e+05,
         1.04712855e+05,   1.07151931e+05,   1.09647820e+05,
         1.12201845e+05,   1.14815362e+05,   1.17489755e+05,
         1.20226443e+05,   1.23026877e+05,   1.25892541e+05,
         1.28824955e+05,   1.31825674e+05,   1.34896288e+05,
         1.38038426e+05,   1.41253754e+05,   1.44543977e+05,
         1.47910839e+05,   1.51356125e+05,   1.54881662e+05,
         1.58489319e+05,   1.62181010e+05,   1.65958691e+05,
         1.69824365e+05,   1.73780083e+05,   1.77827941e+05,
         1.81970086e+05,   1.86208714e+05,   1.90546072e+05,
         1.94984460e+05,   1.99526231e+05,   2.04173794e+05,
         2.08929613e+05,   2.13796209e+05,   2.18776162e+05,
         2.23872114e+05,   2.29086765e+05,   2.34422882e+05,
         2.39883292e+05,   2.45470892e+05,   2.51188643e+05,
         2.57039578e+05,   2.63026799e+05,   2.69153480e+05,
         2.75422870e+05,   2.81838293e+05,   2.88403150e+05,
         2.95120923e+05,   3.01995172e+05,   3.09029543e+05,
         3.16227766e+05,   3.23593657e+05,   3.31131121e+05,
         3.38844156e+05,   3.46736850e+05,   3.54813389e+05,
         3.63078055e+05,   3.71535229e+05,   3.80189396e+05,
         3.89045145e+05,   3.98107171e+05,   4.07380278e+05,
         4.16869383e+05,   4.26579519e+05,   4.36515832e+05,
         4.46683592e+05,   4.57088190e+05,   4.67735141e+05,
         4.78630092e+05,   4.89778819e+05,   5.01187234e+05,
         5.12861384e+05,   5.24807460e+05,   5.37031796e+05,
         5.49540874e+05,   5.62341325e+05,   5.75439937e+05,
         5.88843655e+05,   6.02559586e+05,   6.16595002e+05,
         6.30957344e+05,   6.45654229e+05,   6.60693448e+05,
         6.76082975e+05,   6.91830971e+05,   7.07945784e+05,
         7.24435960e+05,   7.41310241e+05,   7.58577575e+05,
         7.76247117e+05,   7.94328235e+05,   8.12830516e+05,
         8.31763771e+05,   8.51138038e+05,   8.70963590e+05,
         8.91250938e+05,   9.12010839e+05,   9.33254301e+05,
         9.54992586e+05,   9.77237221e+05,   1.00000000e+06,
         1.02329299e+06,   1.04712855e+06,   1.07151931e+06,
         1.09647820e+06,   1.12201845e+06,   1.14815362e+06,
         1.17489755e+06,   1.20226443e+06,   1.23026877e+06,
         1.25892541e+06,   1.28824955e+06,   1.31825674e+06,
         1.34896288e+06,   1.38038426e+06,   1.41253754e+06,
         1.44543977e+06,   1.47910839e+06,   1.51356125e+06,
         1.54881662e+06,   1.58489319e+06,   1.62181010e+06,
         1.65958691e+06,   1.69824365e+06,   1.73780083e+06,
         1.77827941e+06,   1.81970086e+06,   1.86208714e+06,
         1.90546072e+06,   1.94984460e+06,   1.99526231e+06,
         2.04173794e+06,   2.08929613e+06,   2.13796209e+06,
         2.18776162e+06,   2.23872114e+06,   2.29086765e+06,
         2.34422882e+06,   2.39883292e+06,   2.45470892e+06,
         2.51188643e+06,   2.57039578e+06,   2.63026799e+06,
         2.69153480e+06,   2.75422870e+06,   2.81838293e+06,
         2.88403150e+06,   2.95120923e+06,   3.01995172e+06,
         3.09029543e+06,   3.16227766e+06,   3.23593657e+06,
         3.31131121e+06,   3.38844156e+06,   3.46736850e+06,
         3.54813389e+06,   3.63078055e+06,   3.71535229e+06,
         3.80189396e+06,   3.89045145e+06,   3.98107171e+06,
         4.07380278e+06,   4.16869383e+06,   4.26579519e+06,
         4.36515832e+06,   4.46683592e+06,   4.57088190e+06,
         4.67735141e+06,   4.78630092e+06,   4.89778819e+06,
         5.01187234e+06,   5.12861384e+06,   5.24807460e+06,
         5.37031796e+06,   5.49540874e+06,   5.62341325e+06,
         5.75439937e+06,   5.88843655e+06,   6.02559586e+06,
         6.16595002e+06,   6.30957344e+06,   6.45654229e+06,
         6.60693448e+06,   6.76082975e+06,   6.91830971e+06,
         7.07945784e+06,   7.24435960e+06,   7.41310241e+06,
         7.58577575e+06,   7.76247117e+06,   7.94328235e+06,
         8.12830516e+06,   8.31763771e+06,   8.51138038e+06,
         8.70963590e+06,   8.91250938e+06,   9.12010839e+06,
         9.33254301e+06,   9.54992586e+06,   9.77237221e+06,
         1.00000000e+07,   1.02329299e+07,   1.04712855e+07,
         1.07151931e+07,   1.09647820e+07,   1.12201845e+07,
         1.14815362e+07,   1.17489755e+07,   1.20226443e+07,
         1.23026877e+07,   1.25892541e+07,   1.28824955e+07,
         1.31825674e+07,   1.34896288e+07,   1.38038426e+07,
         1.41253754e+07,   1.44543977e+07,   1.47910839e+07,
         1.51356125e+07,   1.54881662e+07,   1.58489319e+07,
         1.62181010e+07,   1.65958691e+07,   1.69824365e+07,
         1.73780083e+07,   1.77827941e+07,   1.81970086e+07,
         1.86208714e+07,   1.90546072e+07,   1.94984460e+07,
         1.99526231e+07,   2.04173794e+07,   2.08929613e+07,
         2.13796209e+07,   2.18776162e+07,   2.23872114e+07,
         2.29086765e+07,   2.34422882e+07,   2.39883292e+07,
         2.45470892e+07,   2.51188643e+07,   2.57039578e+07,
         2.63026799e+07,   2.69153480e+07,   2.75422870e+07,
         2.81838293e+07,   2.88403150e+07,   2.95120923e+07,
         3.01995172e+07,   3.09029543e+07,   3.16227766e+07,
         3.23593657e+07,   3.31131121e+07,   3.38844156e+07,
         3.46736850e+07,   3.54813389e+07,   3.63078055e+07,
         3.71535229e+07,   3.80189396e+07,   3.89045145e+07,
         3.98107171e+07,   4.07380278e+07,   4.16869383e+07,
         4.26579519e+07,   4.36515832e+07,   4.46683592e+07,
         4.57088190e+07,   4.67735141e+07,   4.78630092e+07,
         4.89778819e+07,   5.01187234e+07,   5.12861384e+07,
         5.24807460e+07,   5.37031796e+07,   5.49540874e+07,
         5.62341325e+07,   5.75439937e+07,   5.88843655e+07,
         6.02559586e+07,   6.16595002e+07,   6.30957344e+07,
         6.45654229e+07,   6.60693448e+07,   6.76082975e+07,
         6.91830971e+07,   7.07945784e+07,   7.24435960e+07,
         7.41310241e+07,   7.58577575e+07,   7.76247117e+07,
         7.94328235e+07,   8.12830516e+07,   8.31763771e+07,
         8.51138038e+07,   8.70963590e+07,   8.91250938e+07,
         9.12010839e+07,   9.33254301e+07,   9.54992586e+07,
         9.77237221e+07,   1.00000000e+08,   1.02329299e+08,
         1.04712855e+08,   1.07151931e+08,   1.09647820e+08,
         1.12201845e+08,   1.14815362e+08,   1.17489755e+08,
         1.20226443e+08,   1.23026877e+08,   1.25892541e+08,
         1.28824955e+08,   1.31825674e+08,   1.34896288e+08,
         1.38038426e+08,   1.41253754e+08,   1.44543977e+08,
         1.47910839e+08,   1.51356125e+08,   1.54881662e+08,
         1.58489319e+08,   1.62181010e+08,   1.65958691e+08,
         1.69824365e+08,   1.73780083e+08,   1.77827941e+08,
         1.81970086e+08,   1.86208714e+08,   1.90546072e+08,
         1.94984460e+08,   1.99526231e+08,   2.04173794e+08,
         2.08929613e+08,   2.13796209e+08,   2.18776162e+08,
         2.23872114e+08,   2.29086765e+08,   2.34422882e+08,
         2.39883292e+08,   2.45470892e+08,   2.51188643e+08,
         2.57039578e+08,   2.63026799e+08,   2.69153480e+08,
         2.75422870e+08,   2.81838293e+08,   2.88403150e+08,
         2.95120923e+08,   3.01995172e+08,   3.09029543e+08,
         3.16227766e+08,   3.23593657e+08,   3.31131121e+08,
         3.38844156e+08,   3.46736850e+08,   3.54813389e+08,
         3.63078055e+08,   3.71535229e+08,   3.80189396e+08,
         3.89045145e+08,   3.98107171e+08,   4.07380278e+08,
         4.16869383e+08,   4.26579519e+08,   4.36515832e+08,
         4.46683592e+08,   4.57088190e+08,   4.67735141e+08,
         4.78630092e+08,   4.89778819e+08,   5.01187234e+08,
         5.12861384e+08,   5.24807460e+08,   5.37031796e+08,
         5.49540874e+08,   5.62341325e+08,   5.75439937e+08,
         5.88843655e+08,   6.02559586e+08,   6.16595002e+08,
         6.30957344e+08,   6.45654229e+08,   6.60693448e+08,
         6.76082975e+08,   6.91830971e+08,   7.07945784e+08,
         7.24435960e+08,   7.41310241e+08,   7.58577575e+08,
         7.76247117e+08,   7.94328235e+08,   8.12830516e+08,
         8.31763771e+08,   8.51138038e+08,   8.70963590e+08,
         8.91250938e+08,   9.12010839e+08,   9.33254301e+08,
         9.54992586e+08,   9.77237221e+08,   1.00000000e+09,
};

static Real temp_tbl[NTBL] = {
         1.29500000e+01,   1.32516443e+01,   1.35603147e+01,
         1.38761750e+01,   1.41993926e+01,   1.45301390e+01,
         1.48685894e+01,   1.52149233e+01,   1.55693244e+01,
         1.59319806e+01,   1.63030841e+01,   1.66828317e+01,
         1.70714248e+01,   1.74690693e+01,   1.78759762e+01,
         1.82923612e+01,   1.87184450e+01,   1.91544536e+01,
         1.96006182e+01,   2.00571752e+01,   2.05243668e+01,
         2.10024408e+01,   2.14916505e+01,   2.19922553e+01,
         2.25045207e+01,   2.30287184e+01,   2.35651261e+01,
         2.41140284e+01,   2.46757163e+01,   2.52504876e+01,
         2.58386470e+01,   2.64405064e+01,   2.70563849e+01,
         2.76866091e+01,   2.83315130e+01,   2.89914387e+01,
         2.96667361e+01,   3.03577632e+01,   3.10648863e+01,
         3.17884805e+01,   3.25289293e+01,   3.32866254e+01,
         3.40619705e+01,   3.48553757e+01,   3.56672617e+01,
         3.64980590e+01,   3.73482080e+01,   3.82181595e+01,
         3.91083748e+01,   4.00193259e+01,   4.09514957e+01,
         4.19053786e+01,   4.28814802e+01,   4.38803182e+01,
         4.49024221e+01,   4.59483339e+01,   4.70186081e+01,
         4.81138122e+01,   4.92345268e+01,   5.03813463e+01,
         5.15548786e+01,   5.27557460e+01,   5.39845852e+01,
         5.52420477e+01,   5.65288003e+01,   5.78455252e+01,
         5.91929206e+01,   6.05717008e+01,   6.19825970e+01,
         6.34263571e+01,   6.49037468e+01,   6.64155492e+01,
         6.79625661e+01,   6.95456176e+01,   7.11655432e+01,
         7.28232016e+01,   7.45194719e+01,   7.62552534e+01,
         7.80314664e+01,   7.98490527e+01,   8.17089761e+01,
         8.36122227e+01,   8.55598015e+01,   8.75527453e+01,
         8.95921107e+01,   9.16789791e+01,   9.38144568e+01,
         9.59996762e+01,   9.82357960e+01,   1.00524002e+02,
         1.02865506e+02,   1.05261552e+02,   1.07713408e+02,
         1.10222376e+02,   1.12789785e+02,   1.15416996e+02,
         1.18105404e+02,   1.20856432e+02,   1.23671540e+02,
         1.26552220e+02,   1.29500000e+02,   1.32516443e+02,
         1.35603147e+02,   1.38761750e+02,   1.41993926e+02,
         1.45301390e+02,   1.48685894e+02,   1.52149233e+02,
         1.55693244e+02,   1.59319806e+02,   1.63030841e+02,
         1.66828317e+02,   1.70714248e+02,   1.74690693e+02,
         1.78759762e+02,   1.82923612e+02,   1.87184450e+02,
         1.91544536e+02,   1.96006182e+02,   2.00571752e+02,
         2.05243668e+02,   2.10024408e+02,   2.14916505e+02,
         2.19922553e+02,   2.25045207e+02,   2.30287184e+02,
         2.35651261e+02,   2.41140284e+02,   2.46757163e+02,
         2.52504876e+02,   2.58386470e+02,   2.64405064e+02,
         2.70563849e+02,   2.76866091e+02,   2.83315130e+02,
         2.89914387e+02,   2.96667361e+02,   3.03577632e+02,
         3.10648863e+02,   3.17884805e+02,   3.25289293e+02,
         3.32866254e+02,   3.40619705e+02,   3.48553757e+02,
         3.56672617e+02,   3.64980590e+02,   3.73482080e+02,
         3.82181595e+02,   3.91083748e+02,   4.00193259e+02,
         4.09514957e+02,   4.19053786e+02,   4.28814802e+02,
         4.38803182e+02,   4.49024221e+02,   4.59483339e+02,
         4.70186081e+02,   4.81138122e+02,   4.92345268e+02,
         5.03813463e+02,   5.15548786e+02,   5.27557460e+02,
         5.39845852e+02,   5.52420477e+02,   5.65288003e+02,
         5.78455252e+02,   5.91929206e+02,   6.05717008e+02,
         6.19825970e+02,   6.34263571e+02,   6.49037468e+02,
         6.64155492e+02,   6.79625661e+02,   6.95456176e+02,
         7.11655432e+02,   7.28232016e+02,   7.45194719e+02,
         7.62552534e+02,   7.80314664e+02,   7.98490527e+02,
         8.17089761e+02,   8.36122227e+02,   8.55598015e+02,
         8.75527453e+02,   8.95921107e+02,   9.16789791e+02,
         9.38144568e+02,   9.59996762e+02,   9.82357960e+02,
         1.00524002e+03,   1.02865506e+03,   1.05261552e+03,
         1.07713408e+03,   1.10222376e+03,   1.12789785e+03,
         1.15416996e+03,   1.18105404e+03,   1.20856432e+03,
         1.23671540e+03,   1.26552220e+03,   1.29500000e+03,
         1.32516443e+03,   1.35603147e+03,   1.38761750e+03,
         1.41993926e+03,   1.45301390e+03,   1.48685894e+03,
         1.52149233e+03,   1.55693244e+03,   1.59319806e+03,
         1.63030841e+03,   1.66828317e+03,   1.70714248e+03,
         1.74690693e+03,   1.78759762e+03,   1.82923612e+03,
         1.87184450e+03,   1.91544536e+03,   1.96006182e+03,
         2.00571752e+03,   2.05243668e+03,   2.10024408e+03,
         2.14916505e+03,   2.19922553e+03,   2.25045207e+03,
         2.30287184e+03,   2.35651261e+03,   2.41140284e+03,
         2.46757163e+03,   2.52504876e+03,   2.58386470e+03,
         2.64405064e+03,   2.70563849e+03,   2.76866091e+03,
         2.83315130e+03,   2.89914387e+03,   2.96667361e+03,
         3.03577632e+03,   3.10648863e+03,   3.17884805e+03,
         3.25289293e+03,   3.32866254e+03,   3.40619705e+03,
         3.48553757e+03,   3.56672617e+03,   3.64980590e+03,
         3.73482080e+03,   3.82181595e+03,   3.91083748e+03,
         4.00193259e+03,   4.09514957e+03,   4.19053786e+03,
         4.28814802e+03,   4.38803182e+03,   4.49024221e+03,
         4.59483339e+03,   4.70186081e+03,   4.81138122e+03,
         4.92345268e+03,   5.03802949e+03,   5.15538027e+03,
         5.27546451e+03,   5.39834586e+03,   5.52408949e+03,
         5.65276206e+03,   5.78443181e+03,   5.91916853e+03,
         6.05704368e+03,   6.19813035e+03,   6.34250335e+03,
         6.49023923e+03,   6.64141633e+03,   6.79611479e+03,
         6.95441663e+03,   7.11640581e+03,   7.28216819e+03,
         7.45179168e+03,   7.62536621e+03,   7.80298380e+03,
         7.98473864e+03,   8.17072710e+03,   8.36104778e+03,
         8.55580160e+03,   8.75509183e+03,   8.95902411e+03,
         9.16770659e+03,   9.38124991e+03,   9.59976729e+03,
         9.82337460e+03,   1.00478087e+04,   1.02618409e+04,
         1.04803924e+04,   1.07035803e+04,   1.09314980e+04,
         1.11642261e+04,   1.13601284e+04,   1.15456386e+04,
         1.17337325e+04,   1.19247839e+04,   1.21183694e+04,
         1.23145033e+04,   1.25137280e+04,   1.26727841e+04,
         1.28066003e+04,   1.29408632e+04,   1.30757920e+04,
         1.32107539e+04,   1.33467471e+04,   1.34823771e+04,
         1.36192902e+04,   1.37558961e+04,   1.38932363e+04,
         1.40307659e+04,   1.41638239e+04,   1.42873646e+04,
         1.44101932e+04,   1.45334282e+04,   1.46565626e+04,
         1.47789268e+04,   1.49021309e+04,   1.50247134e+04,
         1.51466333e+04,   1.52693777e+04,   1.53912653e+04,
         1.55126423e+04,   1.56346045e+04,   1.57556576e+04,
         1.58875903e+04,   1.60598806e+04,   1.62320696e+04,
         1.64059312e+04,   1.65794716e+04,   1.67546391e+04,
         1.69296428e+04,   1.71057962e+04,   1.72822014e+04,
         1.74591821e+04,   1.76369245e+04,   1.78317516e+04,
         1.81093903e+04,   1.83906751e+04,   1.86752383e+04,
         1.89637816e+04,   1.92560707e+04,   1.95516873e+04,
         1.98514215e+04,   2.02103170e+04,   2.06041638e+04,
         2.10053930e+04,   2.14141348e+04,   2.18306982e+04,
         2.22551847e+04,   2.27123418e+04,   2.31901074e+04,
         2.36778073e+04,   2.41756450e+04,   2.46838283e+04,
         2.52008803e+04,   2.57194368e+04,   2.62486217e+04,
         2.67885478e+04,   2.73393854e+04,   2.79013497e+04,
         2.84766702e+04,   2.90656984e+04,   2.96667167e+04,
         3.02799641e+04,   3.09056841e+04,   3.15443271e+04,
         3.22212905e+04,   3.29163062e+04,   3.36262179e+04,
         3.43513422e+04,   3.50920029e+04,   3.58655069e+04,
         3.66748546e+04,   3.75024473e+04,   3.83486957e+04,
         3.92140200e+04,   4.01056681e+04,   4.10323641e+04,
         4.19804712e+04,   4.29504842e+04,   4.39429092e+04,
         4.49582639e+04,   4.59970780e+04,   4.70598937e+04,
         4.81472653e+04,   4.92597602e+04,   5.03957734e+04,
         5.15509431e+04,   5.27325846e+04,   5.39413044e+04,
         5.51777228e+04,   5.64392636e+04,   5.77125891e+04,
         5.90146118e+04,   6.03459778e+04,   6.17073477e+04,
         6.30992872e+04,   6.44792408e+04,   6.58892507e+04,
         6.73299633e+04,   6.88020437e+04,   7.03061717e+04,
         7.18044700e+04,   7.33156078e+04,   7.48581850e+04,
         7.64328463e+04,   7.80402494e+04,   7.96866685e+04,
         8.14003595e+04,   8.31506944e+04,   8.49386205e+04,
         8.67647243e+04,   8.86298144e+04,   9.05794256e+04,
         9.25882235e+04,   9.46414580e+04,   9.67401096e+04,
         9.88851798e+04,   1.01112994e+05,   1.03429180e+05,
         1.05798407e+05,   1.08221891e+05,   1.10700872e+05,
         1.13236622e+05,   1.15830440e+05,   1.18483656e+05,
         1.21197629e+05,   1.23973750e+05,   1.26828776e+05,
         1.29782998e+05,   1.32806032e+05,   1.35899482e+05,
         1.39064987e+05,   1.42295554e+05,   1.45582244e+05,
         1.48944843e+05,   1.52385105e+05,   1.55904823e+05,
         1.59514294e+05,   1.63229859e+05,   1.67031971e+05,
         1.70922646e+05,   1.74903946e+05,   1.78977982e+05,
         1.83146915e+05,   1.87412954e+05,   1.91778363e+05,
         1.96245455e+05,   2.00816599e+05,   2.05494218e+05,
         2.10280793e+05,   2.15178862e+05,   2.20191022e+05,
         2.25307975e+05,   2.30512014e+05,   2.35836243e+05,
         2.41283440e+05,   2.46856443e+05,   2.52569561e+05,
         2.58452662e+05,   2.64472798e+05,   2.70633161e+05,
         2.76937017e+05,   2.83387709e+05,   2.89988656e+05,
         2.96743360e+05,   3.03655401e+05,   3.10728443e+05,
         3.17966239e+05,   3.25372624e+05,   3.32951526e+05,
         3.40706963e+05,   3.48643048e+05,   3.56747882e+05,
         3.64987780e+05,   3.73417982e+05,   3.82042885e+05,
         3.90866985e+05,   3.99909765e+05,   4.09224860e+05,
         4.18756931e+05,   4.28511033e+05,   4.38492337e+05,
         4.48706136e+05,   4.59157845e+05,   4.69853005e+05,
         4.80797287e+05,   4.91996495e+05,   5.02491825e+05,
         5.14196363e+05,   5.26173535e+05,   5.38429691e+05,
         5.50971330e+05,   5.63805101e+05,   5.76937809e+05,
         5.90376417e+05,   6.04128050e+05,   6.18200000e+05,
         6.32599728e+05,   6.47334868e+05,   6.62413234e+05,
         6.77842821e+05,   6.93631808e+05,   7.09788569e+05,
         7.26321668e+05,   7.43239873e+05,   7.60552154e+05,
         7.78267690e+05,   7.96395873e+05,   8.14946316e+05,
         8.33928854e+05,   8.53353552e+05,   8.73230710e+05,
         8.93570866e+05,   9.14384806e+05,   9.35683564e+05,
         9.57478434e+05,   9.79780972e+05,   1.00260300e+06,
         1.02595663e+06,   1.04985423e+06,   1.07430847e+06,
         1.09933233e+06,   1.12493907e+06,   1.15114227e+06,
         1.17795582e+06,   1.20539393e+06,   1.23347116e+06,
         1.26220240e+06,   1.29160287e+06,   1.32168816e+06,
         1.35247424e+06,   1.38397741e+06,   1.41621438e+06,
         1.44920225e+06,   1.48295851e+06,   1.51750105e+06,
         1.55284819e+06,   1.58901867e+06,   1.62603167e+06,
         1.66390682e+06,   1.70266418e+06,   1.74232433e+06,
         1.78290828e+06,   1.82443754e+06,   1.86693415e+06,
         1.91042064e+06,   1.95492005e+06,   2.00045599e+06,
         2.04705259e+06,   2.09473457e+06,   2.14352721e+06,
         2.19345637e+06,   2.24454853e+06,   2.29683079e+06,
         2.35033085e+06,   2.40507709e+06,   2.46109853e+06,
         2.51842488e+06,   2.57708653e+06,   2.63711459e+06,
         2.69854087e+06,   2.76139797e+06,   2.82571919e+06,
         2.89153864e+06,   2.95889123e+06,   3.02781266e+06,
         3.09833948e+06,   3.17050908e+06,   3.24435972e+06,
         3.31993057e+06,   3.39726168e+06,   3.47639407e+06,
         3.55736969e+06,   3.64023148e+06,   3.72502336e+06,
         3.81179030e+06,   3.90057830e+06,   3.99143444e+06,
         4.08440690e+06,   4.17954495e+06,   4.27689906e+06,
         4.37652084e+06,   4.47846311e+06,   4.58277991e+06,
         4.68952657e+06,   4.79875967e+06,   4.91053715e+06,
         5.02491825e+06,   5.14196363e+06,   5.26173535e+06,
         5.38429691e+06,   5.50971330e+06,   5.63805101e+06,
         5.76937809e+06,   5.90376417e+06,   6.04128050e+06,
         6.18200000e+06,   6.32599728e+06,   6.47334868e+06,
         6.62413234e+06,   6.77842821e+06,   6.93631808e+06,
         7.09788569e+06,   7.26321668e+06,   7.43239873e+06,
         7.60552154e+06,   7.78267690e+06,   7.96395873e+06,
         8.14946316e+06,   8.33928854e+06,   8.53353552e+06,
         8.73230710e+06,   8.93570866e+06,   9.14384806e+06,
         9.35683564e+06,   9.57478434e+06,   9.79780972e+06,
         1.00260300e+07,   1.02595663e+07,   1.04985423e+07,
         1.07430847e+07,   1.09933233e+07,   1.12493907e+07,
         1.15114227e+07,   1.17795582e+07,   1.20539393e+07,
         1.23347116e+07,   1.26220240e+07,   1.29160287e+07,
         1.32168816e+07,   1.35247424e+07,   1.38397741e+07,
         1.41621438e+07,   1.44920225e+07,   1.48295851e+07,
         1.51750105e+07,   1.55284819e+07,   1.58901867e+07,
         1.62603167e+07,   1.66390682e+07,   1.70266418e+07,
         1.74232433e+07,   1.78290828e+07,   1.82443754e+07,
         1.86693415e+07,   1.91042064e+07,   1.95492005e+07,
         2.00045599e+07,   2.04705259e+07,   2.09473457e+07,
         2.14352721e+07,   2.19345637e+07,   2.24454853e+07,
         2.29683079e+07,   2.35033085e+07,   2.40507709e+07,
         2.46109853e+07,   2.51842488e+07,   2.57708653e+07,
         2.63711459e+07,   2.69854087e+07,   2.76139797e+07,
         2.82571919e+07,   2.89153864e+07,   2.95889123e+07,
         3.02781266e+07,   3.09833948e+07,   3.17050908e+07,
         3.24435972e+07,   3.31993057e+07,   3.39726168e+07,
         3.47639407e+07,   3.55736969e+07,   3.64023148e+07,
         3.72502336e+07,   3.81179030e+07,   3.90057830e+07,
         3.99143444e+07,   4.08440690e+07,   4.17954495e+07,
         4.27689906e+07,   4.37652084e+07,   4.47846311e+07,
         4.58277991e+07,   4.68952657e+07,   4.79875967e+07,
         4.91053715e+07,   5.02491825e+07,   5.14196363e+07,
         5.26173535e+07,   5.38429691e+07,   5.50971330e+07,
         5.63805101e+07,   5.76937809e+07,   5.90376417e+07,
         6.04128050e+07,   6.18200000e+07,   6.32599728e+07,
         6.47334868e+07,   6.62413234e+07,   6.77842821e+07,
         6.93631808e+07,   7.09788569e+07,   7.26321668e+07,
         7.43239873e+07,   7.60552154e+07,   7.78267690e+07,
         7.96395873e+07,   8.14946316e+07,   8.33928854e+07,
         8.53353552e+07,   8.73230710e+07,   8.93570866e+07,
         9.14384806e+07,   9.35683564e+07,   9.57478434e+07,
         9.79780972e+07,   1.00260300e+08,   1.02595663e+08,
         1.04985423e+08,   1.07430847e+08,   1.09933233e+08,
         1.12493907e+08,   1.15114227e+08,   1.17795582e+08,
         1.20539393e+08,   1.23347116e+08,   1.26220240e+08,
         1.29160287e+08,   1.32168816e+08,   1.35247424e+08,
         1.38397741e+08,   1.41621438e+08,   1.44920225e+08,
         1.48295851e+08,   1.51750105e+08,   1.55284819e+08,
         1.58901867e+08,   1.62603167e+08,   1.66390682e+08,
         1.70266418e+08,   1.74232433e+08,   1.78290828e+08,
         1.82443754e+08,   1.86693415e+08,   1.91042064e+08,
         1.95492005e+08,   2.00045599e+08,   2.04705259e+08,
         2.09473457e+08,   2.14352721e+08,   2.19345637e+08,
         2.24454853e+08,   2.29683079e+08,   2.35033085e+08,
         2.40507709e+08,   2.46109853e+08,   2.51842488e+08,
         2.57708653e+08,   2.63711459e+08,   2.69854087e+08,
         2.76139797e+08,   2.82571919e+08,   2.89153864e+08,
         2.95889123e+08,   3.02781266e+08,   3.09833948e+08,
         3.17050908e+08,   3.24435972e+08,   3.31993057e+08,
         3.39726168e+08,   3.47639407e+08,   3.55736969e+08,
         3.64023148e+08,   3.72502336e+08,   3.81179030e+08,
         3.90057830e+08,   3.99143444e+08,   4.08440690e+08,
         4.17954495e+08,   4.27689906e+08,   4.37652084e+08,
         4.47846311e+08,   4.58277991e+08,   4.68952657e+08,
         4.79875967e+08,   4.91053715e+08,   5.02491825e+08,
         5.14196363e+08,   5.26173535e+08,   5.38429691e+08,
         5.50971330e+08,   5.63805101e+08,   5.76937809e+08,
         5.90376417e+08,   6.04128050e+08,   6.18200000e+08,
};

namespace CGKUtility
{
  Real get_temp_from_t1(const Real t1) {
    int iter=1,T1idx;
    Real Tmax,Tmin,Tnew;
    Real Ti,Tip1,T1i,T1ip1;

    Tmax=mumax*t1;
    Tmin=mumin*t1;

    if(Tmax < 5.e3) return Tmax;
    if(Tmin > 1.e7) return Tmin;

    T1idx=get_Tidx(t1);
    T1i   = T1_tbl[T1idx  ];
    T1ip1 = T1_tbl[T1idx+1];
    Ti   = temp_tbl[T1idx  ];
    Tip1 = temp_tbl[T1idx+1];
    Tnew=Ti+(Tip1-Ti)*(t1-T1i)/(T1ip1-T1i);

    return Tnew;
  }

  Real get_temp(Real pressure, Real dens){
    Real T1, temp;

    T1 = pressure / dens * unitT;
    temp = get_temp_from_t1(T1);
    return temp;
  }

	int FindStrIndex(const std::string *str_arr, const int len,
			const std::string name) {
		std::vector<int> ifind;
		std::stringstream msg; //error message
		for (int i=0; i<len; i++) {
			if (str_arr[i] == name) {
				ifind.push_back(i);
			}
		}
		if (ifind.size() == 1) {
			return ifind[0];
		} else if (ifind.size() == 0) {
			msg <<  "### FATAL ERROR in ChemNetwork [FindStrIndex]" << std::endl
				<< name << " not found." << std::endl; 
			throw std::runtime_error(msg.str().c_str());
		} else {
			msg <<  "### FATAL ERROR in ChemNetwork [FindStrIndex]" << std::endl
				<< name << " found more than once (" << ifind.size() << ")."  << std::endl; 
			throw std::runtime_error(msg.str().c_str());
		}
	}

}

static int get_Tidx(const Real temp){
  Real Tidx;
  Real Tmin=10.0,Tmax=1.e9,dT=0.01;
  Real x1, x2;

  if(temp < Tmin) return 0;
  if(temp >= Tmax) return NTBL-2;

  x1 = log10(temp/Tmin)/dT;
  x2 = NTBL-2;
  if (x1 < x2) {
    return (int)x1;
  } else {
    return (int)x2;
  }
}
