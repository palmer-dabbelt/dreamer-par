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

#ifndef OPERATION_HXX
#define OPERATION_HXX

class operation;

#include <libflo/opcode.h++>
#include <libflo/node.h++>
#include <libflo/operation.h++>
#include <libocn/node.h++>
#include "tile.h++"
#include "cnode.h++"

class operation: public libflo::operation<cnode> {
    friend class libflo::operation<cnode>;

private:
    ssize_t _cycle;

private:
    operation(std::shared_ptr<cnode>& dest,
              const libflo::unknown<size_t>& width,
              const libflo::opcode& op,
              const std::vector<std::shared_ptr<cnode>>& s);

public:
    /* Allows us to update the cycle this is computed at, and query
     * that value. */
    ssize_t cycle(void) const { return _cycle; }
    void set_cycle(ssize_t cycle) { _cycle = cycle; }
};

#endif
