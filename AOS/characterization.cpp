#include <characterization.hpp>

using namespace dbt;

std::string DNAEncoding::encode(llvm::Module *M) {
  std::string DNA;

  for (auto &Func : *M) {
    for (auto &BB : Func) {
      for (auto &Inst : BB) {
        switch(Inst.getOpcode()) {
          case llvm::Instruction::Br:
            DNA.push_back('A');
            break;
          case llvm::Instruction::Switch:
            DNA.push_back('B');
            break;
          case llvm::Instruction::IndirectBr:
            DNA.push_back('C');
            break;
          case llvm::Instruction::Ret:
            DNA.push_back('D');
            break;
          case llvm::Instruction::Invoke:
            DNA.push_back('E');
            break;
          case llvm::Instruction::Resume:
            DNA.push_back('F');
            break;
          case llvm::Instruction::Unreachable:
            DNA.push_back('G');
            break;
          case llvm::Instruction::Add:
            DNA.push_back('H');
            break;
          case llvm::Instruction::Sub:
            DNA.push_back('I');
            break;
          case llvm::Instruction::Mul:
            DNA.push_back('J');
            break;
          case llvm::Instruction::UDiv:
            DNA.push_back('K');
            break;
          case llvm::Instruction::SDiv:
            DNA.push_back('L');
            break;
          case llvm::Instruction::URem:
            DNA.push_back('M');
            break;
          case llvm::Instruction::SRem:
            DNA.push_back('N');
            break;
          case llvm::Instruction::FAdd:
            DNA.push_back('O');
            break;
          case llvm::Instruction::FSub:
            DNA.push_back('P');
            break;
          case llvm::Instruction::FMul:
            DNA.push_back('Q');
            break;
          case llvm::Instruction::FDiv:
            DNA.push_back('R');
            break;
          case llvm::Instruction::FRem:
            DNA.push_back('S');
            break;
          case llvm::Instruction::Shl:
            DNA.push_back('T');
            break;
          case llvm::Instruction::LShr:
            DNA.push_back('U');
            break;
          case llvm::Instruction::AShr:
            DNA.push_back('V');
            break;
          case llvm::Instruction::And:
            DNA.push_back('X');
            break;
          case llvm::Instruction::Or:
            DNA.push_back('W');
            break;
          case llvm::Instruction::Xor:
            DNA.push_back('Y');
            break;
          case llvm::Instruction::ExtractElement:
            DNA.push_back('Z');
            break;
          case llvm::Instruction::InsertElement:
            DNA.push_back('a');
            break;
          case llvm::Instruction::ShuffleVector:
            DNA.push_back('b');
            break;
          case llvm::Instruction::ExtractValue:
            DNA.push_back('c');
            break;
          case llvm::Instruction::InsertValue:
            DNA.push_back('d');
            break;
          case llvm::Instruction::Load:
            DNA.push_back('e');
            break;
          case llvm::Instruction::Store:
            DNA.push_back('f');
            break;
          case llvm::Instruction::Alloca:
            DNA.push_back('g');
            break;
          case llvm::Instruction::Fence:
            DNA.push_back('h');
            break;
          case llvm::Instruction::AtomicRMW:
            DNA.push_back('i');
            break;
          case llvm::Instruction::AtomicCmpXchg:
            DNA.push_back('j');
            break;
          case llvm::Instruction::GetElementPtr:
            DNA.push_back('k');
            break;
          case llvm::Instruction::Trunc:
            DNA.push_back('l');
            break;
          case llvm::Instruction::ZExt:
            DNA.push_back('m');
            break;
          case llvm::Instruction::SExt:
            DNA.push_back('n');
            break;
          case llvm::Instruction::UIToFP:
            DNA.push_back('o');
            break;
          case llvm::Instruction::SIToFP:
            DNA.push_back('p');
            break;
          case llvm::Instruction::PtrToInt:
            DNA.push_back('q');
            break;
          case llvm::Instruction::IntToPtr:
            DNA.push_back('r');
            break;
          case llvm::Instruction::BitCast:
            DNA.push_back('s');
            break;
          case llvm::Instruction::AddrSpaceCast:
            DNA.push_back('t');
            break;
          case llvm::Instruction::FPTrunc:
            DNA.push_back('u');
            break;
          case llvm::Instruction::FPExt:
            DNA.push_back('v');
            break;
          case llvm::Instruction::FPToUI:
            DNA.push_back('x');
            break;
          case llvm::Instruction::FPToSI:
            DNA.push_back('y');
            break;
          case llvm::Instruction::ICmp:
            DNA.push_back('w');
            break;
          case llvm::Instruction::FCmp:
            DNA.push_back('z');
            break;
          case llvm::Instruction::Select:
            DNA.push_back('0');
            break;
          case llvm::Instruction::VAArg:
            DNA.push_back('1');
            break;
          case llvm::Instruction::LandingPad:
            DNA.push_back('2');
            break;
          case llvm::Instruction::PHI:
            DNA.push_back('3');
            break;
          case llvm::Instruction::Call:
            DNA.push_back('4');
            break;
          default:
            DNA.push_back('5');
            break;
        }
      }
    }
  }

  return DNA;
}

std::string DNAEncoding::encode(OIInstList OIRegion) {
  std::string DNA;

  for (auto Pair : OIRegion) {
    switch(OIDecoder::decode(Pair[1]).Type) {
      case OIInstType::Add:
        DNA.push_back('a');
        DNA.push_back('a');
        break;
      case OIInstType::And:
        DNA.push_back('a');
        DNA.push_back('b');
        break;
      case OIInstType::Andi:
        DNA.push_back('a');
        DNA.push_back('c');
        break;
      case OIInstType::Call:
        DNA.push_back('a');
        DNA.push_back('d');
        break;
      case OIInstType::Jumpr:
        DNA.push_back('a');
        DNA.push_back('e');
        break;
      case OIInstType::Stw:
        DNA.push_back('a');
        DNA.push_back('f');
        break;
      case OIInstType::Sltiu:
        DNA.push_back('a');
        DNA.push_back('g');
        break;
      case OIInstType::Slti:
        DNA.push_back('a');
        DNA.push_back('h');
        break;
      case OIInstType::Jeq:
        DNA.push_back('a');
        DNA.push_back('i');
        break;
      case OIInstType::Jne:
        DNA.push_back('a');
        DNA.push_back('j');
        break;
      case OIInstType::Jump:
        DNA.push_back('a');
        DNA.push_back('k');
        break;
      case OIInstType::Mul:
        DNA.push_back('a');
        DNA.push_back('l');
        break;
      case OIInstType::Shr:
        DNA.push_back('a');
        DNA.push_back('m');
        break;
      case OIInstType::Shl:
        DNA.push_back('a');
        DNA.push_back('n');
        break;
      case OIInstType::Jeqz:
        DNA.push_back('a');
        DNA.push_back('o');
        break;
      case OIInstType::Sub:
        DNA.push_back('a');
        DNA.push_back('p');
        break;
      case OIInstType::Slt:
        DNA.push_back('a');
        DNA.push_back('q');
        break;
      case OIInstType::Div:
        DNA.push_back('a');
        DNA.push_back('r');
        break;
      case OIInstType::Mod:
        DNA.push_back('a');
        DNA.push_back('s');
        break;
      case OIInstType::Ori:
        DNA.push_back('a');
        DNA.push_back('t');
        break;
      case OIInstType::Jgtz:
        DNA.push_back('a');
        DNA.push_back('u');
        break;
      case OIInstType::Jlez:
        DNA.push_back('a');
        DNA.push_back('v');
        break;
      case OIInstType::Jnez:
        DNA.push_back('a');
        DNA.push_back('w');
        break;
      case OIInstType::Ldbu:
        DNA.push_back('a');
        DNA.push_back('x');
        break;
      case OIInstType::Stb:
        DNA.push_back('a');
        DNA.push_back('y');
        break;
      case OIInstType::Sltu:
        DNA.push_back('a');
        DNA.push_back('z');
        break;
      case OIInstType::Asr:
        DNA.push_back('b');
        DNA.push_back('a');
        break;
      case OIInstType::Jltz:
        DNA.push_back('b');
        DNA.push_back('b');
        break;
      case OIInstType::Movn:
        DNA.push_back('b');
        DNA.push_back('c');
        break;
      case OIInstType::Nor:
        DNA.push_back('b');
        DNA.push_back('d');
        break;
      case OIInstType::Ldh:
        DNA.push_back('b');
        DNA.push_back('e');
        break;
      case OIInstType::Ldb:
        DNA.push_back('b');
        DNA.push_back('f');
        break;
      case OIInstType::Sth:
        DNA.push_back('b');
        DNA.push_back('g');
        break;
      case OIInstType::Ldhu:
        DNA.push_back('b');
        DNA.push_back('h');
        break;
      case OIInstType::Jgez:
        DNA.push_back('b');
        DNA.push_back('i');
        break;
      case OIInstType::Nop:
        DNA.push_back('b');
        DNA.push_back('j');
        break;
      case OIInstType::Seh:
        DNA.push_back('b');
        DNA.push_back('k');
        break;
      case OIInstType::Callr:
        DNA.push_back('b');
        DNA.push_back('l');
        break;
      case OIInstType::Shlr:
        DNA.push_back('b');
        DNA.push_back('m');
        break;
      case OIInstType::Xor:
        DNA.push_back('b');
        DNA.push_back('n');
        break;
      case OIInstType::Seb:
        DNA.push_back('b');
        DNA.push_back('o');
        break;
      case OIInstType::Ijmp:
        DNA.push_back('b');
        DNA.push_back('p');
        break;
      case OIInstType::Divu:
        DNA.push_back('b');
        DNA.push_back('q');
        break;
      case OIInstType::Modu:
        DNA.push_back('b');
        DNA.push_back('r');
        break;
      case OIInstType::Ldc1:
        DNA.push_back('b');
        DNA.push_back('s');
        break;
      case OIInstType::Mthc1:
        DNA.push_back('b');
        DNA.push_back('t');
        break;
      case OIInstType::Ceqd:
        DNA.push_back('b');
        DNA.push_back('u');
        break;
      case OIInstType::Ceqs:
        DNA.push_back('b');
        DNA.push_back('v');
        break;
      case OIInstType::Bc1f:
        DNA.push_back('b');
        DNA.push_back('w');
        break;
      case OIInstType::Bc1t:
        DNA.push_back('b');
        DNA.push_back('x');
        break;
      case OIInstType::Movd:
        DNA.push_back('b');
        DNA.push_back('y');
        break;
      case OIInstType::Lwc1:
        DNA.push_back('b');
        DNA.push_back('z');
        break;
      case OIInstType::Adds:
        DNA.push_back('c');
        DNA.push_back('a');
        break;
      case OIInstType::Addd:
        DNA.push_back('c');
        DNA.push_back('b');
        break;
      case OIInstType::Mtc1:
        DNA.push_back('c');
        DNA.push_back('c');
        break;
      case OIInstType::Truncws:
        DNA.push_back('c');
        DNA.push_back('d');
        break;
      case OIInstType::Truncwd:
        DNA.push_back('c');
        DNA.push_back('e');
        break;
      case OIInstType::Cvtsw:
        DNA.push_back('c');
        DNA.push_back('f');
        break;
      case OIInstType::Cvtdw:
        DNA.push_back('c');
        DNA.push_back('g');
        break;
      case OIInstType::Cvtds:
        DNA.push_back('c');
        DNA.push_back('h');
        break;
      case OIInstType::Cvtsd:
        DNA.push_back('c');
        DNA.push_back('i');
        break;
      case OIInstType::Mulu:
        DNA.push_back('c');
        DNA.push_back('j');
        break;
      case OIInstType::Movz:
        DNA.push_back('c');
        DNA.push_back('k');
        break;
      case OIInstType::Xori:
        DNA.push_back('c');
        DNA.push_back('l');
        break;
      case OIInstType::Sdc1:
        DNA.push_back('c');
        DNA.push_back('m');
        break;
      case OIInstType::Swc1:
        DNA.push_back('c');
        DNA.push_back('n');
        break;
      case OIInstType::Maddd:
        DNA.push_back('c');
        DNA.push_back('o');
        break;
      case OIInstType::Movs:
        DNA.push_back('c');
        DNA.push_back('p');
        break;
      case OIInstType::Muls:
        DNA.push_back('c');
        DNA.push_back('q');
        break;
      case OIInstType::Coltd:
        DNA.push_back('c');
        DNA.push_back('r');
        break;
      case OIInstType::Swxc1:
        DNA.push_back('c');
        DNA.push_back('s');
        break;
      case OIInstType::Negd:
        DNA.push_back('c');
        DNA.push_back('t');
        break;
      case OIInstType::Lwxc1:
        DNA.push_back('c');
        DNA.push_back('u');
        break;
      case OIInstType::Syscall:
        DNA.push_back('c');
        DNA.push_back('v');
        break;
      case OIInstType::Mtlc1:
        DNA.push_back('c');
        DNA.push_back('w');
        break;
      case OIInstType::Divs:
        DNA.push_back('c');
        DNA.push_back('x');
        break;
      case OIInstType::Subs:
        DNA.push_back('c');
        DNA.push_back('y');
        break;
      case OIInstType::Mflc1:
        DNA.push_back('c');
        DNA.push_back('z');
        break;
      case OIInstType::Mfhc1:
        DNA.push_back('d');
        DNA.push_back('a');
        break;
      case OIInstType::Divd:
        DNA.push_back('d');
        DNA.push_back('b');
        break;
      case OIInstType::Subd:
        DNA.push_back('d');
        DNA.push_back('c');
        break;
      case OIInstType::Negs:
        DNA.push_back('d');
        DNA.push_back('d');
        break;
      case OIInstType::Ext:
        DNA.push_back('d');
        DNA.push_back('e');
        break;
      case OIInstType::Madds:
        DNA.push_back('d');
        DNA.push_back('f');
        break;
      case OIInstType::Shrr:
        DNA.push_back('d');
        DNA.push_back('g');
        break;
      case OIInstType::Movf:
        DNA.push_back('d');
        DNA.push_back('h');
        break;
      case OIInstType::Movt:
        DNA.push_back('d');
        DNA.push_back('i');
        break;
      case OIInstType::Ldxc1:
        DNA.push_back('d');
        DNA.push_back('j');
        break;
      case OIInstType::Msubs:
        DNA.push_back('d');
        DNA.push_back('k');
        break;
      case OIInstType::Coled:
        DNA.push_back('d');
        DNA.push_back('l');
        break;
      case OIInstType::Culed:
        DNA.push_back('d');
        DNA.push_back('m');
        break;
      case OIInstType::Msubd:
        DNA.push_back('d');
        DNA.push_back('n');
        break;
      case OIInstType::Movzd:
        DNA.push_back('d');
        DNA.push_back('o');
        break;
      case OIInstType::Movfd:
        DNA.push_back('d');
        DNA.push_back('p');
        break;
      case OIInstType::Asrr:
        DNA.push_back('d');
        DNA.push_back('q');
        break;
      case OIInstType::Absd:
        DNA.push_back('d');
        DNA.push_back('r');
        break;
      case OIInstType::Abss:
        DNA.push_back('d');
        DNA.push_back('s');
        break;
      case OIInstType::Cund:
        DNA.push_back('d');
        DNA.push_back('t');
        break;
      case OIInstType::Movnd:
        DNA.push_back('d');
        DNA.push_back('u');
        break;
      case OIInstType::Ror:
        DNA.push_back('d');
        DNA.push_back('v');
        break;
      case OIInstType::Movzs:
        DNA.push_back('d');
        DNA.push_back('w');
        break;
      case OIInstType::Movfs:
        DNA.push_back('d');
        DNA.push_back('x');
        break;
      case OIInstType::Colts:
        DNA.push_back('d');
        DNA.push_back('y');
        break;
      case OIInstType::Movns:
        DNA.push_back('d');
        DNA.push_back('z');
        break;
      case OIInstType::Coles:
        DNA.push_back('e');
        DNA.push_back('a');
        break;
      case OIInstType::Sqrtd:
        DNA.push_back('e');
        DNA.push_back('b');
        break;
      case OIInstType::Sqrts:
        DNA.push_back('e');
        DNA.push_back('c');
        break;
      case OIInstType::Cults:
        DNA.push_back('e');
        DNA.push_back('d');
        break;
      case OIInstType::Cules:
        DNA.push_back('e');
        DNA.push_back('e');
        break;
      case OIInstType::Cultd:
        DNA.push_back('e');
        DNA.push_back('f');
        break;
      case OIInstType::Movtd:
        DNA.push_back('e');
        DNA.push_back('g');
        break;
      case OIInstType::Movts:
        DNA.push_back('e');
        DNA.push_back('h');
        break;
      case OIInstType::Cuns:
        DNA.push_back('e');
        DNA.push_back('i');
        break;
      case OIInstType::Cueqs:
        DNA.push_back('e');
        DNA.push_back('j');
        break;
      case OIInstType::Cueqd:
        DNA.push_back('e');
        DNA.push_back('k');
        break;
      case OIInstType::Null:
        DNA.push_back('e');
        DNA.push_back('l');
        break;
      default:
        DNA.push_back('e');
        DNA.push_back('m');
        break;
    }
  }

  return DNA;
}
