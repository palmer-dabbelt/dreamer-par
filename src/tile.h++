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
#include <libdrasm/regval.h++>
#include <libocn/node.h++>
#include <string>
#include "avail_net.h++"
#include "operation.h++"
#include "machine.h++"
#include "cnode.h++"

class tile: public libocn::node<tile>, public libdrasm::tile {
private:
    std::weak_ptr<tile> _self;

    /* This is used by the simulated annealing code to tune the
     * congestion cost for the cost function. */
    size_t _nodes;

public:
    /* Creates a new tile by name.  Note that this actually consists
     * of a pair of operations: one to create the shared pointer, and
     * one to add the self-reference. */
    tile(const std::string& name, const std::shared_ptr<machine>& m);
    void set_self_pointer(const std::shared_ptr<tile>& self);

    /* Attempts to place the given operation on this tile, returning
     * TRUE if the operation could actually be placed and FALSE
     * otherwise. */
    bool place(const std::shared_ptr<operation>& op);

    /* Attempts to obtain access to a node as a register on this tile.
     * This access must exist at the given cycle, but we can modify */
    std::shared_ptr<libdrasm::regval> obtain(const std::shared_ptr<cnode>& node,
                                             ssize_t &cycle);

    /* Allocates a register on this tile, potentially spilling some if
     * we need to in order to obtain a free register.  The register
     * will be allocated at the given cycle, which may need to be
     * modified in order to make space for instructions */
    std::shared_ptr<libdrasm::regval> allocate_register(ssize_t& cycle);

    /* Add or remove a node from this tile. */
    void add_node(void);
    void remove_node(void);
    size_t node_count(void) const { return _nodes; }
};

#endif
