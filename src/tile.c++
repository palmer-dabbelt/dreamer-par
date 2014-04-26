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
#include "avail_net.h++"
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

bool tile::place(const std::shared_ptr<operation>& op)
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
        return true;

        /* These nodes don't have any other constraints at all, they
         * can simply be placed anywhere on the graph. */
    case libflo::opcode::IN:
    {
        /* In order to make this work we need two things: an
         * instruction to actually load the input, and a register in
         * which to store that value. */
        auto c = find_free_instruction();
        if (c < 0) return false;
        auto r = allocate_register(c);
        if (r == NULL) return false;

        /* We know this instruction can be computed on this tile so go
         * ahead and do so right now. */
        use_instruction(c);
        op->d()->computed_at(_self.lock(), c);
        op->set_cycle(c);

        return true;
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
    case libflo::opcode::WR:
    case libflo::opcode::XOR:
    {
        /* Memories need to be placed before we use them.  Note that
         * many sorts of operations can refer to a memory, so in this
         * case we just wait for a memory node to appear and schedule
         * it on this tile -- essentially it's a greedy schedule. */
        for (const auto& source: op->sources()) {
            if (!source->is_mem())
                continue;

            if (source->owner() != NULL)
                continue;

            ssize_t array = find_free_array(source->depth());
            if (array < 0)
                return false;

            auto tile = _self.lock();
            use_array(array, source->depth());
            source->set_owner(tile);
            source->make_availiable(std::make_shared<avail_mem>(0, tile));
        }

        /* We need to use an instruction in order to compute this
         * operation, so find one right here that we could use -- if
         * there's none left on this tile then die right away. */
        auto cycle = find_free_instruction();
        if (cycle < 0)
            return false;

#ifdef PRINT_PLACEMENT
        fprintf(stderr, "Attempting to place '%s' on '%s' at %ld\n",
                op->to_string().c_str(),
                name().c_str(),
                cycle);
#endif

        /* Walk through the list of sources and attempt to obtain each
         * one as a register value that lives on this tile.  Note that
         * if this fails we don't actually perform cleanup but instead
         * just leave the extra values around -- this will only fail
         * when we're close to full of instructions on a tile, so it
         * shouldn't be a problem (as probably nothing else can fit
         * anyway). */
        for (const auto& source: op->sources()) {
#ifdef PRINT_PLACEMENT
            fprintf(stderr, "Obtaining '%s'\n", source->name().c_str());
#endif
            auto source_reg = obtain(source, cycle);
            if (source_reg == NULL)
                return false;

#ifdef PRINT_PLACEMENT
            fprintf(stderr, "'%s' obtained\n", source->name().c_str());
#endif
        }

        /* All operations output some value, and we can only store
         * those values in a register.  Here we find the register that
         * we'll compute into. */
        auto reg = allocate_register(cycle);
        if (reg == NULL)
            return false;

        /* At this point we've got every value on this tile and a free
         * instruction with wich to perform the computation.  Here we
         * actually do that computation. */
        use_instruction(cycle);
        op->set_cycle(cycle);
        op->d()->computed_at(_self.lock(), cycle);
        return true;
    }

        /* These nodes don't actually need to be scheduled, so just
         * drop them right here. */
    case libflo::opcode::CATD:
    case libflo::opcode::MEM:
    case libflo::opcode::NOP:
        op->set_cycle(0);
        return true;

    case libflo::opcode::EAT:
    case libflo::opcode::LD:
    case libflo::opcode::REG:
    case libflo::opcode::ST:
        fprintf(stderr, "Unhandled op '%s'\n", op->to_string().c_str());
        return false;
    }

    fprintf(stderr, "Reached end of switch: %d\n", op->op());
    abort();
    return -1;
}

std::shared_ptr<libdrasm::regval> tile::obtain(
    const std::shared_ptr<cnode>& node,
    ssize_t &cycle)
{
    /* Constants are always availiable, all the time. */
    if (node->is_const())
        return std::make_shared<libdrasm::regval>();

    return node->obtain(_self.lock(), cycle);
}

/* FIXME: This does no register allocation.  This is actually OK
 * because it'll never fail if you've got the same number of registers
 * as instructions (which we do!). */
std::shared_ptr<libdrasm::regval>
tile::allocate_register(ssize_t& cycle __attribute__((unused)))
{
    auto r = find_free_register();
    if (r < 0) {
        fprintf(stderr, "Register spilling unimplemented\n");
        abort();
    }
    use_register(r);

    return std::make_shared<libdrasm::regval>();
}
