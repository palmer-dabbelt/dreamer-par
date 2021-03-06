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

#ifndef AVAIL_NET_HXX
#define AVAIL_NET_HXX

class avail_net;

#include "availiability.h++"

/* This tracks the availiability of a value that's sitting at the
 * network output port.  */
class avail_net: public availiability {
protected:
    ssize_t _cycle;
    std::weak_ptr<tile> _owner;
    std::weak_ptr<cnode> _node;

public:
    /* Network operations are only valid for a single cycle, as
     * they'll be gone directly after that. */
    avail_net(ssize_t cycle, const std::weak_ptr<tile>& owner);

    /* Overrides from "class availiability". */
    ssize_t cost_to_obtain(const std::shared_ptr<tile>& tile,
                           ssize_t cycle) override;
    std::shared_ptr<libdrasm::regval> obtain(
        const std::shared_ptr<tile>& tile,
        ssize_t& cicle) override;
    void deallocate(size_t cycle) override;
    std::string to_string(void) const override;
};

#endif
