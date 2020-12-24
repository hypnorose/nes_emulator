#include <iostream>
#include "cpu.cpp"

namespace Instructions{
	// general functions


	unsigned int last_adr;

	unsigned char Immediate(unsigned char arg1){
		return arg1;
	}

	unsigned char Zero_Page(unsigned char arg1){
		last_adr = arg1;
		return Memory::read(arg1);
	}

	unsigned char Zero_Page_X(unsigned char arg1){
		last_adr = (arg1 + CPU::X)%0x100;
		return Memory::read((arg1 + CPU::X)%0x100);
	}
	unsigned char Zero_Page_Y(unsigned char arg1){
		last_adr = (arg1 + CPU::Y)%0x100;
		return Memory::read((arg1 + CPU::Y)%0x100);
	}


	unsigned char Absolute(unsigned int arg1){
		last_adr = arg1;
		return Memory::read(last_adr);
	}
	unsigned char Absolute_X(unsigned int arg1){
		last_adr = arg1 + CPU::X;
		last_adr = last_adr&0xFFFF;
		return Memory::read(last_adr);
	}
	unsigned char Absolute_Y(unsigned int arg1){
		last_adr = arg1 + CPU::Y;
		last_adr = last_adr&0xFFFF;
		return Memory::read(last_adr);

	}
	unsigned char Indirect(unsigned char arg1){
		last_adr = CPU::getLittleEndianDW(arg1);
		return Memory::read(CPU::getLittleEndianDW(arg1));
	}
	unsigned char Indirect_X(unsigned char arg1){
		

		
		last_adr = (Memory::read((arg1+CPU::X+1)&255)<<8) + Memory::read((arg1 + CPU::X)&255);
	
		return Memory::read(last_adr);
	}

	unsigned char Indirect_Y(unsigned char arg1){

	
		last_adr = (Memory::read((arg1+1)&255)<<8) + Memory::read((arg1)&255);
		last_adr = (last_adr + CPU::Y)&0xFFFF;

		return Memory::read(last_adr );
	}


	void ORA(unsigned char value){
		CPU::A = CPU::A | value;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	void ADC(unsigned char value){
		unsigned char AL,AH;
		AL = (CPU::A & 15) + (value & 15) + CPU::get_flag(CARRY_FLAG);
			AH = (CPU::A >> 4) + (value >> 4) + (AL > 15);
		if(0){
			
			if(AL > 9 )AL+=6;
			CPU::set_flag(ZERO_FLAG,(CPU::A+value+CPU::get_flag(CARRY_FLAG))& 255 != 0);
			CPU::set_flag(NEGATIVE_FLAG, AH & 8 != 0);
			CPU::set_flag(OVERFLOW_FLAG, ((AH << 4) ^ CPU::A) & 128 && !(( CPU::A ^ value) & 128));
			if(AH > 9)AH+=6;
			CPU::set_flag(CARRY_FLAG,AH>15);
			CPU::A = ((AH << 4) | (AL & 15)) & 255;
			CPU::set_flag(ZERO_FLAG,CPU::A == 0);
			CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
		}
		else{
			CPU::set_flag(ZERO_FLAG,((CPU::A+value+CPU::get_flag(CARRY_FLAG))& 255 )== 0);
			
			CPU::set_flag(NEGATIVE_FLAG, (CPU::A+value+CPU::get_flag(CARRY_FLAG)) & 128);
			CPU::set_flag(OVERFLOW_FLAG, ((AH << 4) ^ CPU::A) & 128 && !(( CPU::A ^ value) & 128));
			int cf =( CPU::A +value +CPU::get_flag(CARRY_FLAG))&256;
			CPU::A = CPU::A +value +CPU::get_flag(CARRY_FLAG);
			CPU::set_flag(CARRY_FLAG,cf);
		}
	
	}
	void AND(unsigned char value){
		CPU::A = CPU::A & value;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	
	void STA(unsigned int value){
	
		Memory::write(last_adr,CPU::A);
	}
	void STY(unsigned int value){
	
		Memory::write(last_adr,CPU::Y);
	}
	void EOR(unsigned char value){
		CPU::A = CPU::A ^ value;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	void SBC(unsigned char value){
			unsigned char AH; 
			unsigned char AL; 
		if(0){
			AL = (CPU::A&15) - (value & 15) - !CPU::get_flag(CARRY_FLAG);
			if(AL&16)AL -= 6;
			AH = (CPU::A>>4) - (value >> 4) - (AL & 16);
			if(AH&16)AH-=6;
		}
		
		int cf = (CPU::A - value - !CPU::get_flag(CARRY_FLAG))&256;
		CPU::set_flag(ZERO_FLAG,((CPU::A - value - !CPU::get_flag(CARRY_FLAG))&255) == 0);
		CPU::set_flag(OVERFLOW_FLAG,((CPU::A - value - !CPU::get_flag(CARRY_FLAG))^value) && (CPU::A ^ value) & 128);
		CPU::set_flag(NEGATIVE_FLAG,(CPU::A - value - !CPU::get_flag(CARRY_FLAG))&128);
		if(0){
			CPU::A = ((AH << 4) | (AL&15) )&255;
		}
		else{
			CPU::A = CPU::A - value - !CPU::get_flag(CARRY_FLAG) ;
		}
		CPU::set_flag(CARRY_FLAG,!cf);
	}

	void CMP(unsigned char value){
			unsigned char AH; 
			unsigned char AL; 
		if(0){
			AL = (CPU::A&15) - (value & 15) - !CPU::get_flag(CARRY_FLAG);
			if(AL&16)AL -= 6;
			AH = (CPU::A>>4) - (value >> 4) - (AL & 16);
			if(AH&16)AH-=6;
		}
		int out = CPU::A - value;
		CPU::set_flag(CARRY_FLAG,!((CPU::A - value)&256));
		CPU::set_flag(ZERO_FLAG,(out)==0);
		//CPU::set_flag(OVERFLOW_FLAG,((CPU::A - value - !CPU::get_flag(CARRY_FLAG))^value) && (CPU::A ^ value) & 128);
		CPU::set_flag(NEGATIVE_FLAG,out & 128);
	}

	// CPY
	void CPY(unsigned char arg1){
		int out = CPU::Y - arg1;
		CPU::set_flag(CARRY_FLAG,!((CPU::Y - arg1)&256));
		CPU::set_flag(ZERO_FLAG,(out)==0);
		//CPU::set_flag(OVERFLOW_FLAG,((CPU::A - value - !CPU::get_flag(CARRY_FLAG))^value) && (CPU::A ^ value) & 128);
		CPU::set_flag(NEGATIVE_FLAG,out & 128);
	}

	//CPX
	void CPX(unsigned char value){
		int out = CPU::X - value;
		CPU::set_flag(CARRY_FLAG,!((CPU::X - value)&256));
		CPU::set_flag(ZERO_FLAG,(out)==0);
		//CPU::set_flag(OVERFLOW_FLAG,((CPU::A - value - !CPU::get_flag(CARRY_FLAG))^value) && (CPU::A ^ value) & 128);
		CPU::set_flag(NEGATIVE_FLAG,out & 128);
	}


	// $20
	// len: 3
	void JSR_Absolute(unsigned  int arg1){

		CPU::push_stack(((CPU::PC+2)>>8)&0x00FF);
		CPU::push_stack((CPU::PC+2) & 0x00FF);
		CPU::PC = arg1-1;
	
	}

	void JMP(unsigned int arg1){
		CPU::PC = arg1-1;
	}
	// $4c
	// len: 3
	void JMP_Absolute(unsigned int arg1){
		JMP(arg1);
	
	}

	// $6c

	void JMP_Indirect(unsigned  int arg1){


		unsigned int page = arg1&0xFF00;
		last_adr = (Memory::read(page + ((arg1+1)&255))<<8) + Memory::read((arg1));
	
		JMP(last_adr);
	}
	/// STX
	void STX(unsigned int adr ){
		Memory::write(last_adr,CPU::X);
	}
		
	// $86
	void STX_Zero_Page(unsigned char arg1){

		STX(Zero_Page(arg1));
	}

	// $96
	void STX_Zero_Page_Y(unsigned char arg1){
		STX(Zero_Page_Y(arg1));
	}

	// $8e
	void STX_Absolute(unsigned int arg1){
		STX(Absolute(arg1));
	}

	void BIT(unsigned char arg1){

		CPU::set_flag(ZERO_FLAG,(CPU::A & arg1) == 0);
		CPU::set_flag(NEGATIVE_FLAG,((arg1) & (1<<7)));
		CPU::set_flag(OVERFLOW_FLAG,(( arg1) & (1<<6)));
	}

	// $24
	void BIT_Zero_Page(unsigned char arg1){

		BIT(Zero_Page(arg1));
	}

	// $2C

	void BIT_Absolute(unsigned int arg1){
		BIT(Absolute(arg1));
	}
	// $60

	void RTS(){
		CPU::PC = (CPU::pop_stack()) + (CPU::pop_stack()<<8);
		
	}

	// $40

	void RTI(){
		CPU::P = (CPU::pop_stack());
		CPU::PC = (CPU::pop_stack()) + (CPU::pop_stack()<<8);
		CPU::PC -=1;
		CPU::set_flag(5,1);
		CPU::set_flag(4,0);
	}
	// $29
	void AND_Immediate(unsigned char arg1){
		AND(arg1);
	}

	// $25
	void AND_Zero_Page(unsigned char arg1){

		AND(Zero_Page(arg1));
	}

	// $35
	void AND_Zero_Page_X(unsigned char arg1){
		AND(Zero_Page_X(arg1));
	}

	// $2D

	void AND_Absolute(unsigned int arg1){
		AND(Absolute(arg1));
	}

	// $3D 

	void AND_Absolute_X(unsigned int arg1){
		AND(Absolute_X(arg1));
	}

	// $39

	void AND_Absolute_Y(unsigned int arg1){
		AND(Absolute_Y(arg1));
	}

	// $21

	void AND_Indirect_X(unsigned char arg1){
		AND(Indirect_X(arg1));
	}

	// $31
	void AND_Indirect_Y(unsigned char arg1){
		AND(Indirect_Y(arg1));
	}




	// $69
	void ADC_Immediate(unsigned char arg1){
		ADC(arg1);
	}	
	// $65
	void ADC_Zero_Page(unsigned char arg1){
		ADC(Zero_Page(arg1));
	}

	// $75
	void ADC_Zero_Page_X(unsigned char arg1){
		ADC(Zero_Page_X(arg1));
	}

	// $6D
	void ADC_Absolute(unsigned int arg1){
		ADC(Absolute(arg1));
	}

	// $7D
	void ADC_Absolute_X(unsigned int arg1){
		ADC(Absolute_X(arg1));
	}

	// $79
	void ADC_Absolute_Y(unsigned int arg1){
		ADC(Absolute_Y(arg1));
	}

	// $61
	void ADC_Indirect_X(unsigned char arg1){
		ADC(Indirect_X(arg1));
	}

	// $71
	void ADC_Indirect_Y(unsigned char arg1){
		ADC(Indirect_Y(arg1));
	}

	// $58
	void CLI(){
		CPU::clear_flag(INTERRUPT_DISABLE_FLAG);
	}
	// $78
	void SEI(){
		CPU::set_flag(INTERRUPT_DISABLE_FLAG,1);
	}

	// $f8
	void SED(){
		CPU::set_flag(DECIMAL_FLAG,1);
	}

	// $d8
	void CLD(){
		CPU::clear_flag(DECIMAL_FLAG);
	}
	// $18
	void CLC(){
		CPU::clear_flag(CARRY_FLAG);
	}
	// $38
	void SEC(){
		CPU::set_flag(CARRY_FLAG,1);
	}
	// $b8
	void CLV(){
		CPU::clear_flag(OVERFLOW_FLAG);
	}

	void LDA(unsigned char value){
	
		CPU::A = value;

		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
		
	}

	// $a9
	void LDA_Immediate(unsigned char arg1){
		LDA(arg1);
	}

	// $a5
	void LDA_Zero_Page(unsigned char arg1){

		LDA(Zero_Page(arg1));
	}

	// $b5
	void LDA_Zero_Page_X(unsigned char arg1){
		LDA(Zero_Page_X(arg1));
	}

	// $ad

	void LDA_Absolute(unsigned int arg1){
		LDA(Absolute(arg1));
	}

	// $bd

	void LDA_Absolute_X(unsigned int arg1){
		LDA(Absolute_X(arg1));
	}

	// $b9

	void LDA_Absolute_Y(unsigned int arg1){
		LDA(Absolute_Y(arg1));
	}

	// $a1

	void LDA_Indirect_X(unsigned char arg1){
		LDA(Indirect_X(arg1));
	}

	// $b1
	void LDA_Indirect_Y(unsigned char arg1){
		LDA(Indirect_Y(arg1));
	}

	void LDX(unsigned char value){
		CPU::X = value;
		CPU::set_flag(ZERO_FLAG,CPU::X == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::X & 128);
	}

	// $a2
	void LDX_Immediate(unsigned char arg1){
		LDX(arg1);
	}

	// $a6
	void LDX_Zero_Page(unsigned char arg1){

		LDX(Zero_Page(arg1));
	}
	// $b6
	void LDX_Zero_Page_Y(unsigned char arg1){

		LDX(Zero_Page_Y(arg1));
	}

	// $ae

	void LDX_Absolute(unsigned int arg1){
		LDX(Absolute(arg1));
	}


	// $be

	void LDX_Absolute_Y(unsigned int arg1){
		LDX(Absolute_Y(arg1));
	}

	
	/// LDY


	void LDY(unsigned char value){
		CPU::Y = value;
		CPU::set_flag(ZERO_FLAG,CPU::Y == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::Y & 128);

	}

	// $a0
	void LDY_Immediate(unsigned char arg1){
		LDY(arg1);
	}

	// $a4
	void LDY_Zero_Page(unsigned char arg1){

		LDY(Zero_Page(arg1));
	}
	// $b4
	void LDY_Zero_Page_X(unsigned char arg1){

		LDY(Zero_Page_X(arg1));
	}

	// $ac

	void LDY_Absolute(unsigned int arg1){
		LDY(Absolute(arg1));
	}


	// $bc

	void LDY_Absolute_X(unsigned int arg1){
		LDY(Absolute_X(arg1));
	}





	// $85

	void STA_Zero_Page(unsigned char arg1){
		STA(Zero_Page(arg1));
	}

	// $95

	void STA_Zero_Page_X(unsigned char arg1){
		STA(Zero_Page_X(arg1));
	}

	
	// $8d
	void STA_Absolute(unsigned int value){
		STA(Absolute(value));
	}
	// $9d
	void STA_Absolute_X(unsigned int value){
		STA(Absolute_X(value));
	}

	// $99
	void STA_Absolute_Y(unsigned int value){
		STA(Absolute_Y(value));
	}

	void STA_Indirect_X(unsigned char value){
		STA(Indirect_X(value));
	}
	void STA_Indirect_Y(unsigned char value){
		STA(Indirect_Y(value));
	}	

	//STY

	// $84

	void STY_Zero_Page(unsigned char arg1){
		STY(Zero_Page(arg1));
	}

	// $94

	void STY_Zero_Page_X(unsigned char arg1){
		STY(Zero_Page_X(arg1));
	}

	
	// $8c
	void STY_Absolute(unsigned int value){
		STY(Absolute(value));
	}



	// $9a
	void TXS(){
		CPU::S = CPU::X;
		
	}
	// $ba
	void TSX(){
		CPU::X = CPU::S;
		CPU::set_flag(ZERO_FLAG,CPU::X == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::X & 128);
	}

	// $48
	void PHA(){
		CPU::push_stack(CPU::A);
	}

	// $68
	void PLA(){
		CPU::A = CPU::pop_stack();
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	// $08
	void PHP(){
		CPU::set_flag(4,1);
		CPU::push_stack(CPU::P);
		CPU::set_flag(4,0);
	}

	// $28
	void PLP(){
		CPU::P = CPU::pop_stack();
		CPU::set_flag(4,0);
		CPU::set_flag(5,1);
	}


	// branchs

	// $10

	void BPL(unsigned char arg1){
		if(CPU::get_flag(NEGATIVE_FLAG) == 0){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}
	// $30
	void BMI(unsigned char arg1){
		if(CPU::get_flag(NEGATIVE_FLAG) == 1){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}
	// $50
	void BVC(unsigned char arg1){
		if(CPU::get_flag(OVERFLOW_FLAG) == 0){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// $70
	void BVS(unsigned char arg1){
		if(CPU::get_flag(OVERFLOW_FLAG) == 1){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// $90
	void BCC(unsigned char arg1){
		if(CPU::get_flag(CARRY_FLAG) == 0){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// $b0
	void BCS(unsigned char arg1){
		if(CPU::get_flag(CARRY_FLAG) == 1){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// $d0
	void BNE(unsigned char arg1){
		if(CPU::get_flag(ZERO_FLAG) == 0){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// $f0
	void BEQ(unsigned char arg1){
		if(CPU::get_flag(ZERO_FLAG) == 1){
			CPU::PC+=(char)arg1+1;
		}
		else{
			CPU::PC+=1;
		}
		
	}

	// Reg instructions
	// all implied
	// $aa
	void TAX(){
		CPU::X = CPU::A;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}
	// $8a
	void TXA(){
		CPU::A = CPU::X;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	// $ca
	void DEX(){
		CPU::X--;
		CPU::set_flag(ZERO_FLAG,CPU::X == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::X & 128);
	}

	// $e8
	void INX(){
		CPU::X++;
		CPU::set_flag(ZERO_FLAG,CPU::X == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::X & 128);
	}
	
	// $a8
	void TAY(){
		CPU::Y = CPU::A;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	// $98
	void TYA(){
		CPU::A = CPU::Y;
		CPU::set_flag(ZERO_FLAG,CPU::A == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::A & 128);
	}

	// $88
	void DEY(){
		CPU::Y--;
		CPU::set_flag(ZERO_FLAG,CPU::Y == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::Y& 128);
	}

	// $c8
	void INY(){
		CPU::Y++;
		CPU::set_flag(ZERO_FLAG,CPU::Y == 0);
		CPU::set_flag(NEGATIVE_FLAG,CPU::Y & 128);
	}



	// EOR


	// $49
	void EOR_Immediate(unsigned char arg1){
		EOR(arg1);
	}

	// $45
	void EOR_Zero_Page(unsigned char arg1){

		EOR(Zero_Page(arg1));
	}

	// $55
	void EOR_Zero_Page_X(unsigned char arg1){
		EOR(Zero_Page_X(arg1));
	}

	// $4d

	void EOR_Absolute(unsigned int arg1){
		EOR(Absolute(arg1));
	}

	// $5d

	void EOR_Absolute_X(unsigned int arg1){
		EOR(Absolute_X(arg1));
	}

	// $59

	void EOR_Absolute_Y(unsigned int arg1){
		EOR(Absolute_Y(arg1));
	}

	// $41

	void EOR_Indirect_X(unsigned char arg1){
		EOR(Indirect_X(arg1));
	}

	// $51
	void EOR_Indirect_Y(unsigned char arg1){
		EOR(Indirect_Y(arg1));
	}

	

	


	// CMP


	// $c9
	void CMP_Immediate(unsigned char arg1){
		CMP(arg1);
	}

	// $c5
	void CMP_Zero_Page(unsigned char arg1){

		CMP(Zero_Page(arg1));
	}

	// $d5
	void CMP_Zero_Page_X(unsigned char arg1){
		CMP(Zero_Page_X(arg1));
	}

	// $cd

	void CMP_Absolute(unsigned int arg1){
		CMP(Absolute(arg1));
	}

	// $dd

	void CMP_Absolute_X(unsigned int arg1){
		CMP(Absolute_X(arg1));
	}

	// $d9

	void CMP_Absolute_Y(unsigned int arg1){
		CMP(Absolute_Y(arg1));	}

	// $c1

	void CMP_Indirect_X(unsigned char arg1){
		CMP(Indirect_X(arg1));
	}

	// $d1
	void CMP_Indirect_Y(unsigned char arg1){
		CMP(Indirect_Y(arg1));
	}




	
	// ORA


	// $09
	void ORA_Immediate(unsigned char arg1){
		ORA(arg1);
	}

	// $05
	void ORA_Zero_Page(unsigned char arg1){

		ORA(Zero_Page(arg1));
	}

	// $15
	void ORA_Zero_Page_X(unsigned char arg1){
		ORA(Zero_Page_X(arg1));
	}

	// $0d

	void ORA_Absolute(unsigned int arg1){
		ORA(Absolute(arg1));
	}

	// $1d

	void ORA_Absolute_X(unsigned int arg1){
		ORA(Absolute_X(arg1));
	}

	// $19

	void ORA_Absolute_Y(unsigned int arg1){
		ORA(Absolute_Y(arg1));	}

	// $01

	void ORA_Indirect_X(unsigned char arg1){
		ORA(Indirect_X(arg1));
	}

	// $11
	void ORA_Indirect_Y(unsigned char arg1){
		ORA(Indirect_Y(arg1));
	}





	// $c0
	void CPY_Immediate(unsigned char arg1){
		CPY(arg1);
	}

	// $c4
	void CPY_Zero_Page(unsigned char arg1){

		CPY(Zero_Page(arg1));
	}


	// $cc

	void CPY_Absolute(unsigned int arg1){
		CPY(Absolute(arg1));
	}

	



	// $e0
	void CPX_Immediate(unsigned char arg1){
		CPX(arg1);
	}

	// $e4
	void CPX_Zero_Page(unsigned char arg1){

		CPX(Zero_Page(arg1));
	}


	// $ec

	void CPX_Absolute(unsigned int arg1){
		CPX(Absolute(arg1));
	}



		//SBC

	// $e9
	void SBC_Immediate(unsigned char arg1){
		SBC(arg1);
	}	
	// $e5
	void SBC_Zero_Page(unsigned char arg1){
		SBC(Zero_Page(arg1));
	}

	// $f5
	void SBC_Zero_Page_X(unsigned char arg1){
		SBC(Zero_Page_X(arg1));
	}

	// $eD
	void SBC_Absolute(unsigned int arg1){
		SBC(Absolute(arg1));
	}

	// $fD
	void SBC_Absolute_X(unsigned int arg1){
		SBC(Absolute_X(arg1));
	}

	// $f9
	void SBC_Absolute_Y(unsigned int arg1){
		SBC(Absolute_Y(arg1));
	}

	// $e1
	void SBC_Indirect_X(unsigned char arg1){
		SBC(Indirect_X(arg1));
	}

	// $f1
	void SBC_Indirect_Y(unsigned char arg1){
		SBC(Indirect_Y(arg1));
	}

	void LSR(){
		unsigned char orig;
		if(last_adr==-1){
			orig = CPU::A;
		}
		else{
			orig = Memory::read(last_adr);
		}
		
		unsigned char out = (orig >> 1)&255;
		CPU::set_flag(CARRY_FLAG,orig&1);
		CPU::set_flag(ZERO_FLAG,out==0);
		CPU::set_flag(NEGATIVE_FLAG,out&128);
		if(last_adr==-1){
			CPU::A = out;
		}
		else{
			Memory::write(last_adr,out);
		}
	}
	void ROR(){
		int orig;
		if(last_adr==-1){
			orig = CPU::A;
		}
		else{
			orig = Memory::read(last_adr);
		}
		unsigned char out = (orig >> 1)&255;
		out |= (1<<7)*CPU::get_flag(CARRY_FLAG);
		CPU::set_flag(CARRY_FLAG,orig&1);
		CPU::set_flag(ZERO_FLAG,out==0);
		CPU::set_flag(NEGATIVE_FLAG,out&128);
		if(last_adr==-1){
			CPU::A = out;
		}
		else{
			Memory::write(last_adr,out);
		}
	}

	void ROL(){
		int orig;
		if(last_adr==-1){
			orig = CPU::A;
		}
		else{
			orig = Memory::read(last_adr);
		}
	
		unsigned char out = (orig << 1)&255;
		out |= CPU::get_flag(CARRY_FLAG);
		CPU::set_flag(CARRY_FLAG,orig&(1<<7));
		CPU::set_flag(ZERO_FLAG,out==0);
		CPU::set_flag(NEGATIVE_FLAG,out&128);
		if(last_adr==-1){
			CPU::A = out;
		}
		else{
			Memory::write(last_adr,out);
		}
	}

	void ASL(){
		int orig;
		if(last_adr==-1){
			orig = CPU::A;
		}
		else{
			orig = Memory::read(last_adr);
		}
	
		unsigned char out = (orig << 1)&255;
		
		CPU::set_flag(CARRY_FLAG,orig&(1<<7));
		CPU::set_flag(ZERO_FLAG,out==0);
		CPU::set_flag(NEGATIVE_FLAG,out&128);
		if(last_adr==-1){
			CPU::A = out;
		}
		else{
			Memory::write(last_adr,out);
		}
	}

	// $2a
	
	void LSR_Accumulator(){
		last_adr = -1;
		LSR();
	}
	// $46
	void LSR_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		LSR();
	}
	// $56
	void LSR_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		LSR();
	}
	// $4e
	void LSR_Absolute(unsigned int arg1){
		Absolute(arg1);
		LSR();
	}
	// 5e
	void LSR_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		LSR();
	}

	// $6a
	
	void ROR_Accumulator(){
		last_adr = -1;
		ROR();
	}
	// $66
	void ROR_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		ROR();
	}
	// $76
	void ROR_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		ROR();
	}
	// $6e
	void ROR_Absolute(unsigned int arg1){
		Absolute(arg1);
		ROR();
	}
	// $7e
	void ROR_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		ROR();
	}


	// $2a
	
	void ROL_Accumulator(){
		last_adr = -1;
		ROL();
	}
	// $26
	void ROL_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		ROL();
	}
	// $36
	void ROL_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		ROL();
	}
	// $2e
	void ROL_Absolute(unsigned int arg1){
		Absolute(arg1);
		ROL();
	}
	// $3e
	void ROL_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		ROL();
	}

	// $0a
	
	void ASL_Accumulator(){
		last_adr = -1;
		ASL();
	}
	// $06
	void ASL_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		ASL();
	}
	// $16
	void ASL_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		ASL();
	}
	// $0e
	void ASL_Absolute(unsigned int arg1){
		Absolute(arg1);
		ASL();
	}
	// $1e
	void ASL_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		ASL();
	}

	void INC(){
		
		Memory::RAM[last_adr]++;
		CPU::set_flag(ZERO_FLAG,Memory::RAM[last_adr] == 0);
		CPU::set_flag(NEGATIVE_FLAG,Memory::RAM[last_adr] & 128);
	}

	// $e6
	void INC_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		INC();
	}
	// $f6
	void INC_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		INC();
	}
	// $ee
	void INC_Absolute(unsigned int arg1){
		Absolute(arg1);
		INC();
	}
	// $fe
	void INC_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		INC();
	}

	
	void DEC(){
		Memory::RAM[last_adr]--;
		CPU::set_flag(ZERO_FLAG,Memory::RAM[last_adr] == 0);
		CPU::set_flag(NEGATIVE_FLAG,Memory::RAM[last_adr] & 128);
	}

	// $c6
	void DEC_Zero_Page(unsigned char arg1){
		Zero_Page(arg1);
		DEC();
	}
	// $d6
	void DEC_Zero_Page_X(unsigned char arg1){
		Zero_Page_X(arg1);
		DEC();
	}
	// $ce
	void DEC_Absolute(unsigned int arg1){
		Absolute(arg1);
		DEC();
	}
	// $de
	void DEC_Absolute_X(unsigned int arg1){
		Absolute_X(arg1);
		DEC();
	}
}