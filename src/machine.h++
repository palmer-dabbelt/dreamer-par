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

#ifndef MACHINE_HXX
#define MACHINE_HXX

class machine;

#include "tile.h++"
#include <libdrasm/machine.h++>

class machine: public libdrasm::machine_t<tile> {
public:
    machine(size_t w, size_t h,
            std::function<std::shared_ptr<tile>(size_t, size_t)> f);
};

#endif
