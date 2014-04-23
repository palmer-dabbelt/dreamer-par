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

#ifndef AVAILIABILITY_HXX
#define AVAILIABILITY_HXX

class cnode;
class tile;

#include <libdrasm/regval.h++>
#include <memory>
#include <string>

/* This manages the particular ways in which an node can be availiable
 * -- note that there's a whole bunch of special rules, so you
 * probably want to go about creating the sub-classes of this instead
 * as they've got those special rules. */
class availiability {
public:
    /* Returns an approximation of the cost involved in obtaining a
     * value as a register on the given tile at the given cycle.  Note
     * that if this cost is -1 then it's impossible to obtain this
     * value, otherwise larger values are more expensive.  This is
     * roughly the number of instructions required, but it's a quick
     * mesaure (so no congestion for the network, for example). */
    virtual ssize_t cost_to_obtain(const std::shared_ptr<tile>& tile,
                                   ssize_t cycle) = 0;

    /* Actually obtains a value as a register on the given tile.  Note
     * that if you return a non-negative cost then this can't fail! */
    virtual std::shared_ptr<libdrasm::regval> obtain(
        const std::shared_ptr<tile>& tile,
        ssize_t &cycle) = 0;

    /* Marks a particular sort of availiability as no longer valid.
     * This can happen during register allocation, for example. */
    virtual void deallocate(size_t cycle) = 0;

    /* Provides some string representation of this sort of
     * availiability. */
    virtual std::string to_string(void) const = 0;
};

#endif
