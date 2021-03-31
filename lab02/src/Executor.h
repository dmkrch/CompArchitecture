
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        Word firstOp;   // first operand from instruction
        Word secondOp;  // second operand from instruction

        bool areValidOperands = true;

        switch(instr->_type)
        {
            case IType::Ld:
            case IType::St:
                // here we set first operand of ALU
                if (instr->_src1)
                    firstOp = instr->_src1Val;
                else
                    areValidOperands = false;
                    
                // here we set second operand of ALU
                if (areValidOperands)
                {
                    if (instr->_imm)
                        secondOp = instr->_imm;
                    else if(instr->_src2Val)
                        secondOp = instr->_src2Val;
                    else
                        areValidOperands = false;
                }

                Word result = AluAction(firstOp, secondOp, instr->_aluFunc);
                instr->_addr = result;

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
                instr->_data = instr->_addr + 4;
                break;
            case IType::Auipc:
                //instr->_data = instr->_addr + instr->_imm;
                break;
        }


        // now branching
        bool result = BrAction(instr->_src1Val, instr->_src2Val, instr->_brFunc);


        instr->_nextIp = 
    }

    Word AluAction(Word& op1, Word& op2, AluFunc func)
    {
        switch(func)
        {
            case AluFunc::Add:
                return op1 + op2;
                break;
            case AluFunc::Sub:
                return op1 - op2;
                break;
            case AluFunc::And:
                return op1 & op2;
                break;
            case AluFunc::Or:
                return op1 | op2;
                break;
            case AluFunc::Xor:
                return op1 ^ op2;
                break;
            case AluFunc::Slt:
                return op1 < op2;
                break;
            case AluFunc::Sltu:
                return op1 < op2;
                break;
            case AluFunc::Sll:
                return op1 << (op2 % 32);
                break;
            case AluFunc::Srl:
                return op1 >> (op2 % 32);
                break;
            case AluFunc::Sra:
                return op1 >> (op2 % 32);
                break;

            default:
                return -1;
            }
    }

    bool BrAction(Word op1, Word op2, BrFunc f)
    {
        switch(f)
        {
            case BrFunc::Eq:
                return op1 == op2;
                break;
            case BrFunc::Neq:
                return op1 != op2;
                break;
            case BrFunc::Lt:
                return op1 < op2;
                break;
            case BrFunc::Ltu:
                return op1 < op2;
                break;
            case BrFunc::Ge:
                return op1 >= op2;
                break;
            case BrFunc::Geu:
                return op1 >= op2;
                break;
            case BrFunc::AT:
                return true;
                break;
            case BrFunc::NT:
                return false;
                break;
            default:
                return false;
        }
    }

    void SetNextIp(bool brResult, InstructionPtr& instr)
    {
        switch(instr->_type)
        {
            case IType::Br:
            case IType::J:
                instr->_nextIp = instr->_addr + instr->_imm;
                break;
            
            case IType::Jr:
                instr->_nextIp = instr->_src1Val + instr->_imm;
        }

        if(brResult)
            instr->_nextIp = instr->_addr;
        else
            instr->_nextIp = instr->_addr + 4;
    }

private:
    /* YOUR CODE HERE */
    // ALU
    // BLOCK OF BRANCHING
    // BLOCK OF LOGIC - WHICH WHICH DATA WE NEED TO ADD TO _data

};

#endif // RISCV_SIM_EXECUTOR_H
