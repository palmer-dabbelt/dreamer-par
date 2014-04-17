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

tile::tile(const std::string& name, const std::shared_ptr<machine>& m)
    : libocn::node<tile>(name),
      libdrasm::tile(m)
{
}

ssize_t tile::place(std::shared_ptr<operation> op,
                    bool commit __attribute__((unused)))
{
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
        ssize_t i = find_free_instruction();
        if (i < 0) return i;

        ssize_t m = find_free_word();
        if (m < 0) return m;

        if (commit) {
            use_instruction(i);
            use_word(m);
        }

        return i;
    }

    case libflo::opcode::ADD:
    case libflo::opcode::AND:
    case libflo::opcode::ARSH:
    case libflo::opcode::CAT:
    case libflo::opcode::CATD:
    case libflo::opcode::EAT:
    case libflo::opcode::EQ:
    case libflo::opcode::GTE:
    case libflo::opcode::LD:
    case libflo::opcode::LIT:
    case libflo::opcode::LOG2:
    case libflo::opcode::LSH:
    case libflo::opcode::LT:
    case libflo::opcode::MEM:
    case libflo::opcode::MOV:
    case libflo::opcode::MSK:
    case libflo::opcode::MUL:
    case libflo::opcode::MUX:
    case libflo::opcode::NEG:
    case libflo::opcode::NEQ:
    case libflo::opcode::NOP:
    case libflo::opcode::NOT:
    case libflo::opcode::OR:
    case libflo::opcode::OUT:
    case libflo::opcode::RD:
    case libflo::opcode::REG:
    case libflo::opcode::RND:
    case libflo::opcode::RSH:
    case libflo::opcode::RSHD:
    case libflo::opcode::RST:
    case libflo::opcode::ST:
    case libflo::opcode::SUB:
    case libflo::opcode::WR:
    case libflo::opcode::XOR:
        fprintf(stderr, "Unhandled op '%s'\n", op->to_string().c_str());
        return -1;
    }

    fprintf(stderr, "Reached end of switch\n");
    abort();
    return -1;
}
