/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-par.
 *
 * dreamer-par is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * dreamer-par is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with dreamer-par.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef TEMPSCHED_HXX
#define TEMPSCHED_HXX

class tempsched {
private:
    std::vector<double> temps;
    size_t i;

public:
    tempsched(void)
        : temps(),
          i(0)
        {
            for (double t = 1.0; t >= 0.001; t *= 0.999)
                temps.push_back(t);
        }

    auto begin(void) -> decltype(temps.begin()) const { return temps.begin(); }
    auto end(void) -> decltype(temps.end()) const { return temps.end(); }
    double operator*(void) const { return temps[i]; }
    void operator++(void) { ++i; }
};

#endif
