
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        Word firstOp;
        Word secondOp;

        // checking whether two operands are valid. If so, doing alu actions
        if (instr->_src1 && (instr->_imm || instr->_src2Val))
        {
            // setting first operand of ALU
            firstOp = instr->_src1Val;
                    
            // setting second operand of ALU
            if (instr->_imm)
                secondOp = instr->_imm.value();
            else
                secondOp = instr->_src2Val;

            // setting _addr or _data (doing alu action)
            switch(instr->_type)
            {
                case IType::Ld:
                case IType::St:
                    instr->_addr = AluAction(firstOp, secondOp, instr->_aluFunc);                    
                    break;

                case IType::Alu:
                case IType::Br:
                case IType::Unsupported:
                    instr->_data = AluAction(firstOp, secondOp, instr->_aluFunc);
                    break;
            }
        }

        // some actions with non-alu actions
        switch(instr->_type)
        {
            case IType::Csrr:
                instr->_data = instr->_csrVal;
                break;

            case IType::Csrw:
                instr->_data = instr->_src1Val;
                break;

            case IType::St:
                instr->_data = instr->_src2Val;
                break;

            case IType::J:
            case IType::Jr:
                instr->_data = ip + 4;
                break;

            case IType::Auipc:
                instr->_data = ip + instr->_imm.value();
                break;
        }

        // now branching

        bool brResult = BrAction(instr->_src1Val, instr->_src2Val, instr->_brFunc);
        SetNextIp(brResult, instr, ip);
    }

private:
    void SetNextIp(bool brResult, InstructionPtr& instr, Word ip)
    {
        switch(instr->_type)
        {
            case IType::Br:
            case IType::J:
                if (brResult)
                    instr->_nextIp = ip + instr->_imm.value();
                else
                    instr->_nextIp = ip + 4;
                return;
            
            case IType::Jr:
                if (brResult)
                    instr->_nextIp = instr->_src1Val + instr->_imm.value();
                else
                    instr->_nextIp = ip + 4;
                return;

            default:
                instr->_nextIp = ip + 4;
        }
    }

    
    Word AluAction(Word op1, Word op2, AluFunc func)
    {
        switch(func)
        {
            case AluFunc::Add:
                return op1 + op2;
            case AluFunc::Sub:
                return op1 - op2;
            case AluFunc::And:
                return op1 & op2;
            case AluFunc::Or:
                return op1 | op2;
            case AluFunc::Xor:
                return op1 ^ op2;
            case AluFunc::Slt:
                return (SignedWord)(op1) < (SignedWord)(op2);
            case AluFunc::Sltu:
                return op1 < op2;
            case AluFunc::Sll:
                return op1 << (op2 % 32);
            case AluFunc::Srl:
                return op1 >> (op2 % 32);
            case AluFunc::Sra:
                return (SignedWord)(op1) >> ((SignedWord)(op2) % 32);
            default:
                return 0;
        }
    }


    bool BrAction(Word op1, Word op2, BrFunc func)
    {
        switch(func)
        {
            case BrFunc::Eq:
                return op1 == op2;
            case BrFunc::Neq:
                return op1 != op2;
            case BrFunc::Lt:
                return (SignedWord)(op1) < (SignedWord)(op2);
            case BrFunc::Ltu:
                return op1 < op2;
            case BrFunc::Ge:
                return (SignedWord)(op1) >= (SignedWord)(op2);
            case BrFunc::Geu:
                return op1 >= op2;
            case BrFunc::AT:
                return true;
            case BrFunc::NT:
                return false;   
            default:
                return false;
        }
    }
};

#endif // RISCV_SIM_EXECUTOR_H
