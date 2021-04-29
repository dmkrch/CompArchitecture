
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"

class Cpu
{
public:
    Cpu(Memory& mem)
        : _mem(mem)
    {

    }
    
    void ProcessInstruction()
    {
        Word instr = _mem.Request(_ip);
        InstructionPtr decodedInstr = _decoder.Decode(instr);

        // reading some needed by instruction registers from rf, csrf
        _rf.Read(decodedInstr);  
        _csrf.Read(decodedInstr);
        _exe.Execute(decodedInstr, _ip);

        // putting data of instruction to memory
        _mem.Request(decodedInstr);

        // putting result to register files rf, csrf
        _rf.Write(decodedInstr);
        _csrf.Write(decodedInstr);

        // refreshing registers in csrfile
        _csrf.InstructionExecuted();
        _ip = decodedInstr->_nextIp;
    }

    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }

    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }

private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    Memory& _mem;
};


#endif //RISCV_SIM_CPU_H
