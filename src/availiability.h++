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

#include <memory>
#include <string>

/* This manages the particular ways in which an node can be availiable
 * -- note that there's a whole bunch of special rules, so you
 * probably want to go about creating the sub-classes of this instead
 * as they've got those special rules. */
class availiability {
public:
    /* Obtains access to the given node on the given tile via this
     * sort of availiability, returning the first cycle on which this
     * availiability could be granted.  Note that the returned cycle
     * must always be after the passed in first cycle, which is the
     * first cycle that the scheduler can make this happen on.  This
     * will mean different things for different sorts of
     * availiability. */
    virtual ssize_t obtain(const std::shared_ptr<tile>& tile,
                           size_t first_cycle,
                           bool commit) = 0;

    /* Marks a particular sort of availiability as no longer valid.
     * This can happen during register allocation, for example. */
    virtual void deallocate(size_t cycle) = 0;

    /* Provides some string representation of this sort of
     * availiability. */
    virtual std::string to_string(void) const = 0;
};

#endif
