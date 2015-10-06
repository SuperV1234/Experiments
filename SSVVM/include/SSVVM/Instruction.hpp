// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVVM_INSTRUCTION
#define SSVVM_INSTRUCTION

namespace ssvvm
{
    struct Instruction
    {
        using Idx = int;

        OpCode opCode;
        Params params;
    };
}

#endif
