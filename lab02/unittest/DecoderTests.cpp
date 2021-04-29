#define  DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Instructions.h"
#include "../src/Decoder.h"


void testBranch(InstructionPtr &instruction);
void testI(InstructionPtr &instruction);
void testR(InstructionPtr &instruction);
void testU(InstructionPtr &instruction);
void testUJ(InstructionPtr &instruction);
void testAlu(InstructionPtr &instruction);


constexpr Word SRCVAL1   = 1;
constexpr Word SRCVAL2   = 2;


unsigned get_bitt(unsigned x, unsigned n) 
{
    return (x >> n) & 1;
}

void DecoderSetWord(Word& w)
{
    Word word = 0;

    // first we get imm[12 | 10:5]
    Word bit = get_bitt(IMM_SB, 12);

    word |= bit;
    word <<= 1;

    for (int i = 10; i > 4; --i)
    {   
        // getting exact bit from imm
        bit = get_bitt(IMM_SB, i);

        // moving bits to left by 1 bit, so that we can insert next bit
        word <<= 1;

        // setting next bit
        word |= bit;
    }

    // getting rs2 to next 5 positions
    Word rs2 = 0b01111;
    word <<= 5;
    word |= rs2;


    // getting rs1
    Word rs1 = 0b01111;
    word <<= 5;
    word |= rs1;

    // getting 111 (14-12 bits)
    word <<= 3;
    word |= 111;



    // getting imm [4:1 | 11]
    for (int i = 4; i > 0; --i)
    {
        // getting exact bit from imm
        bit = get_bitt(IMM_SB, i);

        // moving bits to left by 1 bit, so that we can insert next bit
        word <<= 1;

        // setting next bit
        word |= bit;
    }

    bit = get_bitt(IMM_SB, 11);
    word <<= 1;
    word |= bit;


    // setting last bits: 1100011 (6-0)
    word <<= 7;
    word |= 0b1100011;

    w = word;
}


TEST_SUITE("Decoder"){
    Decoder _decoder;
    TEST_CASE("R-Format"){
        SUBCASE("AND"){
            auto instruction = _decoder.Decode(AND);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::And);
        }

        SUBCASE("ADD"){
            auto instruction = _decoder.Decode(ADD);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Add);
        }

        SUBCASE("OR"){
            auto instruction = _decoder.Decode(OR);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Or);
        }

        SUBCASE("SUB"){
            auto instruction = _decoder.Decode(SUB);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sub);
        }

        SUBCASE("SLL"){
            auto instruction = _decoder.Decode(SLL);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sll);
        }

        SUBCASE("XOR"){
            auto instruction = _decoder.Decode(XOR);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Xor);
        }

        SUBCASE("SRL"){
            auto instruction = _decoder.Decode(SRL);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Srl);
        }

        SUBCASE("SRA"){
            auto instruction = _decoder.Decode(SRA);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sra);
        }

        SUBCASE("SLT"){
            auto instruction = _decoder.Decode(SLT);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Slt);
        }

        SUBCASE("SLTU"){
            auto instruction = _decoder.Decode(SLTU);
            testR(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sltu);
        }
    }

    TEST_CASE("I-Format"){
        SUBCASE("ANDI"){
            auto instruction = _decoder.Decode(ANDI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::And);
        }

        SUBCASE("ADDI"){
            auto instruction = _decoder.Decode(ADDI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Add);
        }

        SUBCASE("ORI"){
            auto instruction = _decoder.Decode(ORI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Or);
        }

        SUBCASE("SLLI"){
            auto instruction = _decoder.Decode(SLLI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sll);
        }

        SUBCASE("XORI"){
            auto instruction = _decoder.Decode(XORI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Xor);
        }

        SUBCASE("SRLI"){
            auto instruction = _decoder.Decode(SRLI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Srl);
        }

        SUBCASE("SRAI"){
            auto instruction = _decoder.Decode(SRAI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sra);
        }

        SUBCASE("SLTIU"){
            auto instruction = _decoder.Decode(SLTIU);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Sltu);
        }

        SUBCASE("SLTI"){
            auto instruction = _decoder.Decode(SLTI);
            testI(instruction);
            CHECK(instruction->_aluFunc == AluFunc::Slt);
        }


        // RV32 Load Instructions are also I-Type
        SUBCASE("LW"){
            auto instruction = _decoder.Decode(LW);
            CHECK(instruction->_imm.value() == IMM);
            CHECK(instruction->_src1.value() == 1);
            CHECK(instruction->_dst.value() == 15);
            CHECK(instruction->_type == IType::Ld);
            CHECK(instruction->_aluFunc == AluFunc::Add);
        }
    }

    TEST_CASE("U-Format"){
        SUBCASE("AUIPC"){
            auto instruction = _decoder.Decode(AUIPC);
            testU(instruction);
            CHECK(instruction->_type == IType::Auipc);
        }

        SUBCASE("LUI"){
            auto instruction = _decoder.Decode(LUI);
            testU(instruction);
            CHECK(instruction->_type == IType::Alu);
            CHECK(instruction->_aluFunc == AluFunc::Add);
        }

    }

    TEST_CASE("S-Format"){
        SUBCASE("SW"){
            auto instruction = _decoder.Decode(SW);
            CHECK(instruction->_imm.value() == IMM_S);
            CHECK(instruction->_src2.value() == 15);
            CHECK(instruction->_src1.value() == 15);
            CHECK(instruction->_type == IType::St);
        }
    }

    TEST_CASE("SB-Format"){
        SUBCASE("BEQ"){
            auto instruction = _decoder.Decode(BEQ);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Eq);
        }

        SUBCASE("BGE"){
            auto instruction = _decoder.Decode(BGE);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Ge);
        }

        SUBCASE("BGEU"){
            auto instruction = _decoder.Decode(BGEU);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Geu);
        }

        SUBCASE("BNE"){
            auto instruction = _decoder.Decode(BNE);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Neq);
        }

        SUBCASE("BLT"){
            auto instruction = _decoder.Decode(BLT);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Lt);
        }

        SUBCASE("BLTU"){
            auto instruction = _decoder.Decode(BLTU);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Ltu);
        }

    }

    TEST_CASE("UJ-Format"){
        SUBCASE("JAL"){
            auto instruction = _decoder.Decode(JAL);
            testUJ(instruction);
            CHECK(instruction->_type == IType::J);
        }

        SUBCASE("JALR"){
            auto instruction = _decoder.Decode(JALR);
            testUJ(instruction);
            CHECK(instruction->_src1.value() == 1);
            CHECK(instruction->_type == IType::Jr);
        }
    }

    // my test for decoder
    TEST_CASE("SB-Format")
    {
        SUBCASE("BGEU")
        {
            Word w;
            DecoderSetWord(w);
            auto instruction = _decoder.Decode(w);
            testBranch(instruction);
            CHECK(instruction->_brFunc == BrFunc::Geu);
        }
    }
}

void testBranch(InstructionPtr &instruction){
    CHECK(instruction->_imm.value() == IMM_SB);
    CHECK(instruction->_src1.value() == 15);
    CHECK(instruction->_src2.value() == 15);
    CHECK(instruction->_type == IType::Br);
}

void testR(InstructionPtr &instruction){
    testAlu(instruction);
    CHECK(instruction->_src2.value() == 3);

}

void testI(InstructionPtr &instruction){
    testAlu(instruction);
    CHECK(instruction->_imm.value() == IMM);
}

void testU(InstructionPtr &instruction){
    CHECK(instruction->_imm.value() == IMM_U << 12u);
    CHECK(instruction->_dst.value() == 15);
}

void testUJ(InstructionPtr &instruction){
    CHECK(instruction->_imm.value() == IMM_UJ);
    CHECK(instruction->_dst.value() == 15);
}

void testAlu(InstructionPtr &instruction){
    CHECK(instruction->_src1.value() == 1);
    CHECK(instruction->_dst.value() == 15);
    CHECK(instruction->_type == IType::Alu);
}
