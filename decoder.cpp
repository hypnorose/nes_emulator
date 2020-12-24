#pragma once
#define DEBUG 0
#include "instructions.cpp"
#include <fstream>

#include <string>
#include "ppu.cpp"

using namespace std;
namespace Decoder{

	

	void loadRom(string filename){

		
		ifstream FIN;
		FIN.open(filename,ios::binary|ios::ate);
		 char b;
		FIN.seekg(0,FIN.end);
		int length = FIN.tellg();
		FIN.seekg(0,FIN.beg);
		int mem_addr = 0;
		int i = 0;
		for(i = 0 ; i < 0x4010;i++){
			FIN.seekg(i);
			FIN.read(&b,1);	
			unsigned char ub = b;
			if(i < 16)continue;	
			if(DEBUG == 3){
					if(i%16==0)cout << endl;
				if(ub < 16)cout << "0";
				cout << hex << (int)ub << " ";
			}		
			//Memory::write(0x8000+mem_addr,ub);
			Memory::write(0xC000+mem_addr,ub);
			mem_addr ++ ;
		
		}
		int start = i;

		mem_addr = 0;
		for (int i = start ; i < start + 0x2000;i++){
			FIN.seekg(i);
			FIN.read(&b,1);	
			unsigned char ub = b;
				if(DEBUG){
					if(i%16==0)cout << endl;
				if(ub < 16)cout << "0";
				cout << hex << (int)ub << " ";
			}
			PPU::VRAM[mem_addr] = ub;
			
			mem_addr++;
		}
		CPU::PC = 0xC000;
		CPU::PC = CPU::getLittleEndianDW(0xFFFC);
		PPU::init();
		
	}
	void next(){
		unsigned char instr = Memory::read(CPU::PC);
		switch (instr){
			case 0x01:
				Instructions::ORA_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x05:
				Instructions::ORA_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x06:
				Instructions::ASL_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x08:
				Instructions::PHP();
			break;
			case 0x09:
				Instructions::ORA_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x0a:
				Instructions::ASL_Accumulator();
			break;
			case 0x0d:
				Instructions::ORA_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x0e:
				Instructions::ASL_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x10:
				Instructions::BPL(Memory::read(CPU::PC+1));
			break;
			case 0x11:
				Instructions::ORA_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x15:
				Instructions::ORA_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x16:
				Instructions::ASL_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x18:
				Instructions::CLC();
			break;
			case 0x19:
				Instructions::ORA_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x1d:
				Instructions::ORA_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x1e:
				Instructions::ASL_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x20:
			//printf("JSR %d\n",CPU::getLittleEndianDW(CPU::PC+1));
				Instructions::JSR_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				
			break;

			case 0x21:
		
				Instructions::AND_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
	
			break;

			case 0x24:
				Instructions::BIT_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x25:
				Instructions::AND_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x26:
				Instructions::ROL_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x28:
				Instructions::PLP();
			break;
			case 0x29:
				Instructions::AND_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x2a:
				Instructions::ROL_Accumulator();
			break;
			
			case 0x2c:
				Instructions::BIT_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x2e:
				Instructions::ROL_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x30:
				Instructions::BMI(Memory::read(CPU::PC+1));
			break;
			
			case 0x31:
		
				Instructions::AND_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
	
			break;
			case 0x2D:
				Instructions::AND_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x35:
				Instructions::AND_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x36:
				Instructions::ROL_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x38:
				Instructions::SEC();
			break;
			case 0x39:
				Instructions::AND_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;	
			case 0x3D:
				Instructions::AND_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x3e:
				Instructions::ROL_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x40:
				Instructions::RTI();
		//		cout << "RTI" << "\n";
			break;
			case 0x41:
				Instructions::EOR_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x45:
				Instructions::EOR_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x46:
				Instructions::LSR_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x48:
				Instructions::PHA();
			break;
			case 0x49:
				Instructions::EOR_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x4a:
				Instructions::LSR_Accumulator();
			break;
			case 0x4c:
		//		printf("JMP %X\n",CPU::getLittleEndianDW(CPU::PC+1));
				Instructions::JMP_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
		//		printf("%X\n",CPU::PC);
			break;
			case 0x4d:
				Instructions::EOR_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x4e:
				Instructions::LSR_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x50:
				Instructions::BVC(Memory::read(CPU::PC+1));
			break;
			case 0x51:
				Instructions::EOR_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x55:
				Instructions::EOR_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x56:
				Instructions::LSR_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x58:
				Instructions::CLI();
			break;
			case 0x59:
				Instructions::EOR_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x5d:
				Instructions::EOR_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x5e:
				Instructions::LSR_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x60:
				Instructions::RTS();
		//		printf("RTS\n");
			break;
			case 0x61:
				Instructions::ADC_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x65:
				Instructions::ADC_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x66:
				Instructions::ROR_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x68:
				Instructions::PLA();
			break;
			case 0x69:
				Instructions::ADC_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x6a:
				Instructions::ROR_Accumulator();
			break;
			case 0x6c:
				Instructions::JMP_Indirect(CPU::getLittleEndianDW(CPU::PC+1));
			break;
			case 0x6d:
				Instructions::ADC_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x6e:
				Instructions::ROR_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x70:
				Instructions::BVS(Memory::read(CPU::PC+1));
			break;
			case 0x71:
				Instructions::ADC_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x75:
				Instructions::ADC_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x76:
				Instructions::ROR_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x78:
				Instructions::SEI();
			break;
			case 0x7d:
				Instructions::ADC_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x7e:
				Instructions::ROR_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x79:
				Instructions::ADC_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x81:
				Instructions::STA_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x84:
				Instructions::STY_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x85:
				Instructions::STA_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x86:
				Instructions::STX_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;

			case 0x88:
				Instructions::DEY();
			break;
			case 0x8a:
				Instructions::TXA();
			break;
			case 0x8c:
				Instructions::STY_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x8d:
				Instructions::STA_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x8e:
				Instructions::STX_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0x90:
				Instructions::BCC(Memory::read(CPU::PC+1));
			break;
			case 0x91:
				Instructions::STA_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x94:
				Instructions::STY_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x95:
				Instructions::STA_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x96:
				Instructions::STX_Zero_Page_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0x98:
				Instructions::TYA();
			break;
			case 0x9a:
				Instructions::TXS();
			break;
			
			case 0x9d:
				Instructions::STA_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
	
			case 0x99:
				Instructions::STA_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xa1:
				Instructions::LDA_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa2:
				Instructions::LDX_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa5:
				Instructions::LDA_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa6:
				Instructions::LDX_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa9:
				Instructions::LDA_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa0:
				Instructions::LDY_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
		
			case 0xa4:
				Instructions::LDY_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xa8:
				Instructions::TAY();
			break;
			case 0xaa:
				Instructions::TAX();
			break;
			case 0xac:
				Instructions::LDY_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xad:
				Instructions::LDA_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xae:
				Instructions::LDX_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xb0:
				Instructions::BCS(Memory::read(CPU::PC+1));
			break;
			case 0xb1:
				Instructions::LDA_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xb4:
				Instructions::LDY_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			
			case 0xb5:
				Instructions::LDA_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xb6:
				Instructions::LDX_Zero_Page_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xb8:
				Instructions::CLV();
			break;
			case 0xb9:
				Instructions::LDA_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xba:
				Instructions::TSX();
			break;
			case 0xbc:
				Instructions::LDY_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xbd:
				Instructions::LDA_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xbe:
				Instructions::LDX_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xc0:
				Instructions::CPY_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xc1:
				Instructions::CMP_Indirect_X(Memory::read(CPU::PC+1));
					CPU::PC+=1;
			break;
			case 0xc4:
				Instructions::CPY_Zero_Page(Memory::read(CPU::PC+1));
					CPU::PC+=1;
			break;
			case 0xc5:
				Instructions::CMP_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xc6:
				Instructions::DEC_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
		
			case 0xc8:
				Instructions::INY();
			break;
			case 0xc9:
				Instructions::CMP_Immediate(Memory::read(CPU::PC+1));
					CPU::PC+=1;
			break;
			case 0xca:
				Instructions::DEX();
			break;
			case 0xcc:
				Instructions::CPY_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xcd:
				Instructions::CMP_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xce:
				Instructions::DEC_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xd0:
				Instructions::BNE(Memory::read(CPU::PC+1));
			
			break;
			case 0xd1:
				Instructions::CMP_Indirect_Y(Memory::read(CPU::PC+1));
					CPU::PC+=1;
			break;
			case 0xd5:
				Instructions::CMP_Zero_Page_X(Memory::read(CPU::PC+1));
					CPU::PC+=1;
			break;
			case 0xd6:
				Instructions::DEC_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xd8:
				Instructions::CLD();
			break;
			case 0xd9:
				Instructions::CMP_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xdd:
				Instructions::CMP_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xde:
				Instructions::DEC_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xe0:
				Instructions::CPX_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xe1:
				Instructions::SBC_Indirect_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xe4:
				Instructions::CPX_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xe5:
				Instructions::SBC_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xe6:
				Instructions::INC_Zero_Page(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xe8:
				Instructions::INX();
			break;
			case 0xe9:
				Instructions::SBC_Immediate(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xea:
			
			break;
			case 0xec:
				Instructions::CPX_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xed:
				Instructions::SBC_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xee:
				Instructions::INC_Absolute(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xf0:
				Instructions::BEQ(Memory::read(CPU::PC+1));
			break;
			case 0xf1:
				Instructions::SBC_Indirect_Y(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xf5:
				Instructions::SBC_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xf6:
				Instructions::INC_Zero_Page_X(Memory::read(CPU::PC+1));
				CPU::PC+=1;
			break;
			case 0xf8:
				Instructions::SED();
			break;
			case 0xf9:
				Instructions::SBC_Absolute_Y(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xfd:
				Instructions::SBC_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;
			case 0xfe:
				Instructions::INC_Absolute_X(CPU::getLittleEndianDW(CPU::PC+1));
				CPU::PC+=2;
			break;

			default:
				printf("unknown instruction: %X \n",instr);
			break;

		}

		CPU::PC++;
	}

	void decode(){

	}
	void panel(){

		string cmd;
			CPU::print();
			cout << endl;
			cout << ">";
		while(cin >> cmd){
		
			if(cmd == "cpu"){
				CPU::print();
			}
			else if( cmd == "mem"){
				int addr;
				cin >> hex >> addr;
				cout << endl;
				for(int i = 0 ; i < 16;i++){
						if((int)Memory::read(addr+i) < 16)cout << "0";
						cout << hex<< (int)Memory::read(addr+i) << " ";
				}
			
			}
			else if( cmd == "next"){
				next();
				PPU::tick();
			}
			else if( cmd == "drawchr"){
				PPU::drawChr();
			}
			else if( cmd == "draw"){
				PPU::draw();
			}
			cout << endl;
			cout << ">";
		}

	}
	void automatic(){
		printf("\n");
		
		CPU::P = 0x24;
		CPU::print();
	
		CPU::file_out = new ofstream("mine.log");
		CPU::log();
		PPU::init();
	
	}
	void tick(){
		//printf("next\n");
		next();
		PPU::tick();
		CPU::log();
	}
}