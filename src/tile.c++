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

#include "tile.h++"
#include "avail_reg.h++"
#include "avail_mem.h++"
#include <assert.h>

tile::tile(const std::string& name, const std::shared_ptr<machine>& m)
    : libocn::node<tile>(name),
      libdrasm::tile(m),
      _self()
{
}

void tile::set_self_pointer(const std::shared_ptr<tile>& self)
{
    _self = self;
}

ssize_t tile::place(const std::shared_ptr<operation>& op,
                    bool commit __attribute__((unused)))
{
    /* We need a self-pointer here. */
    if (_self.expired() == true) {
        fprintf(stderr, "Missing self pointer\n");
        abort();
    }

    switch (op->op()) {
        /* These nodes don't actually need to be placed as they don't
         * do anything at all!  This isn't actually a problem as
         * nothing can ever refer to them. */
    case libflo::opcode::INIT:
        return 0;

        /* These nodes don't have any other constraints at all, they
         * can simply be placed anywhere on the graph. */
    case libflo::opcode::IN:
    {
        /* In order to make this work we need two things: an
         * instruction to actually load the input, and a register in
         * which to store that value. */
        ssize_t c = find_free_instruction();
        if (c < 0) return -1;
        ssize_t r = allocate_register(c, false);
        if (r < 0) return -1;

        if (commit) {
            use_instruction(c);
            allocate_register(c, true);
            op->d()->make_availiable(std::make_shared<avail_reg>(c, _self));
        }

        return c;
    }

        /* These are "regular" operations, which essentially just
         * means that they've got to be scheduled after all their
         * inputs. */
    case libflo::opcode::ADD:
    case libflo::opcode::AND:
    case libflo::opcode::ARSH:
    case libflo::opcode::CAT:
    case libflo::opcode::OR:
    case libflo::opcode::EQ:
    case libflo::opcode::GTE:
    case libflo::opcode::LIT:
    case libflo::opcode::LOG2:
    case libflo::opcode::LSH:
    case libflo::opcode::LT:
    case libflo::opcode::MOV:
    case libflo::opcode::MSK:
    case libflo::opcode::MUL:
    case libflo::opcode::MUX:
    case libflo::opcode::NEG:
    case libflo::opcode::NEQ:
    case libflo::opcode::NOT:
    case libflo::opcode::OUT:
    case libflo::opcode::RD:
    case libflo::opcode::RND:
    case libflo::opcode::RSH:
    case libflo::opcode::RSHD:
    case libflo::opcode::RST:
    case libflo::opcode::SUB:
    case libflo::opcode::XOR:
    case libflo::opcode::WR:
    {
        /* We need to use an instruction in order to compute this
         * operation, so find one right here that we could use. */
        ssize_t first_cycle = find_free_instruction();
        if (first_cycle < 0)
            return -1;

        /* Finds a register that we could potentially use to
         * allocate. */
        ssize_t reg = allocate_register(first_cycle, false);
        if (reg < 0)
            return -1;

        /* Walk through the entire list of sources and figure out when
         * (if ever) we can obtain all these values on this tile. */
        ssize_t target_cycle = first_cycle;
        for (const auto& source: op->sources()) {
            ssize_t cycle = obtain(source, first_cycle, false);
            if (cycle < 0)
                return -1;

            if (cycle > target_cycle)
                target_cycle = cycle;
        }

        /* Here we go and find a free instruction slot that's some
         * time later than the earliest cycle that we've been able to
         * find so far. */
        target_cycle = find_free_instruction(target_cycle);

        /* If they're all availiable, then actually commit to moving
         * them to this tile. */
        for (const auto& source: op->sources()) {
            ssize_t cycle = obtain(source, target_cycle, commit);

            /* FIXME: I actually expect this assert to fail every once
             * in a while.  Specifically, just because a value was
             * availiable at an earlier cycle doesn't mean it can be
             * made availiable now! */
            if (cycle != target_cycle) {
                fprintf(stderr, "Mis-matched target cycle\n");
                fprintf(stderr, "  target_cycle: %lu\n", target_cycle);
                fprintf(stderr, "  '%s': %lu\n", source->name().c_str(), cycle);
                abort();
            }
        }

        /* Now that we've got everything on this tile, use up an
         * instruction in order to compute the values. */
        if (commit) {
            use_instruction(target_cycle);
            allocate_register(target_cycle, commit);
            op->d()->make_availiable(std::make_shared<avail_reg>(target_cycle,
                                                                 _self));
        }

        return target_cycle;
    }

        /* These nodes don't actually need to be scheduled, so just
         * drop them right here. */
    case libflo::opcode::CATD:
    case libflo::opcode::MEM:
    case libflo::opcode::NOP:
        return 0;

    case libflo::opcode::EAT:
    case libflo::opcode::LD:
    case libflo::opcode::REG:
    case libflo::opcode::ST:
        fprintf(stderr, "Unhandled op '%s'\n", op->to_string().c_str());
        return -1;
    }

    fprintf(stderr, "Reached end of switch: %d\n", op->op());
    abort();
    return -1;
}

ssize_t tile::obtain(const std::shared_ptr<cnode>& n,
                     size_t target_cycle,
                     bool commit)
{
    /* Constants are always availiable, all the time. */
    if (n->is_const())
        return target_cycle;

    return n->obtain(_self.lock(), target_cycle, commit);
}

/* FIXME: This does no register allocation.  This is actually OK
 * because it'll never fail if you've got the same number of registers
 * as instructions (which we do!). */
ssize_t tile::allocate_register(size_t target_cycle __attribute__((unused)),
                                bool commit)
{
    ssize_t r = find_free_register();
    if (r < 0)
        return -1;

    if (commit)
        use_register(r);

    return r;
}
