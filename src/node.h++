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

#include <list>
#include <libflo/node.h++>
#include "availiability.h++"

class node: public libflo::node {
    friend class libflo::node;

private:
    /* Here's how we track the availiability of a node.  All nodes
     * start out as unavailiable on every cycle, and the availiability
     * is filled in as the schedule is created. */
    std::list<std::shared_ptr<availiability>> _avail_list;

private:
    node(const std::string name,
         const libflo::unknown<size_t>& width,
         const libflo::unknown<size_t>& depth,
         bool is_mem,
         bool is_const,
         libflo::unknown<size_t> cycle);
};

#endif
