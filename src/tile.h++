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

#ifndef TILE_HXX
#define TILE_HXX

class tile;

#include <libdrasm/tile.h++>
#include <libocn/node.h++>
#include <string>
#include "operation.h++"
#include "machine.h++"
#include "cnode.h++"

class tile: public libocn::node<tile>, public libdrasm::tile {
private:
    std::weak_ptr<tile> _self;

public:
    /* Creates a new tile by name.  Note that this actually consists
     * of a pair of operations: one to create the shared pointer, and
     * one to add the self-reference. */
    tile(const std::string& name, const std::shared_ptr<machine>& m);
    void set_self_pointer(const std::shared_ptr<tile>& self);

    /* Places an operation on this tile, returning the cycle at which
     * this operation will be scheduled.  If "commit" is set to TRUE
     * then the result of this placement will actually be stored to
     * the machine, if FALSE then this just returns the cycle at which
     * this would be placed. */
    ssize_t place(const std::shared_ptr<operation>& op, bool commit);

    /* Attempts to obtain access to a node. */
    ssize_t obtain(const std::shared_ptr<cnode>& n, bool commit);
};

#endif
