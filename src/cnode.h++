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

#ifndef NODE_HXX
#define NODE_HXX

class cnode;

#include <list>
#include <libflo/node.h++>
#include "availiability.h++"
#include "tile.h++"

class cnode: public libflo::node {
    friend class libflo::node;

private:
    /* Here's how we track the availiability of a cnode.  All cnodes
     * start out as unavailiable on every cycle, and the availiability
     * is filled in as the schedule is created. */
    std::list<std::shared_ptr<availiability>> _avail_list;

    /* If anyone owns this node then this will point to that tile. */
    std::shared_ptr<tile> _owner;

private:
    cnode(const std::string name,
          const libflo::unknown<size_t>& width,
          const libflo::unknown<size_t>& depth,
          bool is_mem,
          bool is_const,
          libflo::unknown<size_t> cycle);

public:
    /* Updates the availiability listing with a new sort of
     * availiability. */
    void make_availiable(const std::shared_ptr<availiability>& a);

    /* Returns the first cycle during with this node can be made
     * availiable on the target tile.  This cycle must be after
     * "first_cycle", which is the first cycle at which the scheduler
     * can actually use this node. */
    ssize_t obtain(const std::shared_ptr<tile>& tile,
                   size_t first_cycle,
                   bool commit);

    /* Provides access to the availiability list of this node. */
    std::list<std::shared_ptr<availiability>> avail_list(void) const
        { return _avail_list; }

    /* Allows us to manage the owner of this node, which is the node
     * upon which memory has been allocated to store this node between
     * cycles.  This only makes sense for persistant nodes, otherwise
     * it'll probably never get allocated at all! */
    std::shared_ptr<tile> owner(void) const { return _owner; }
    void set_owner(const std::shared_ptr<tile>& owner) { _owner = owner; }
    
};

#endif

