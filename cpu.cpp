#pragma once
#define CARRY_FLAG 0
#define ZERO_FLAG 1
#define INTERRUPT_DISABLE_FLAG 2
#define DECIMAL_FLAG 3

#define B_FLAG 5
#define OVERFLOW_FLAG 6
#define NEGATIVE_FLAG 7
#include "memory.cpp"
#include <stdio.h>
#include <fstream>
using namespace std;
namespace CPU{

	unsigned char Y;
	unsigned char X;
	unsigned char A;
	unsigned short int PC = 0;
	unsigned char S = 0xFD;
	unsigned char P = 0;
	ofstream* file_out;
	void set_flag(const int flag,int value){
		value = (value != 0);
		if(value){
			P |= (1<<flag);
		}
		else{
			P &= ~(1<<flag);
		}
	}
	
	int get_flag(const int flag){
		return ((P&(1<<flag))>>flag)!=0;
	}

	void clear_flag(const int flag){
		set_flag(flag,0);
	}

	void push_stack(unsigned char v){

		Memory::write(0x100 + S,v);
		S--;
	}
	unsigned char pop_stack(){
		S++;
		return Memory::read(0x100 + S);
	}

	void print(){
		printf("A: %02X X: %02X Y: %02X PC: %02X S: %02X P: %02X\n", A,X,Y,PC,S,P);
	}
	void log(){
		char bufor[300];
		sprintf(bufor,"%04X A:%02X X:%02X Y:%02X P:%02X SP:%02X",PC,A,X,Y,P,S);
		(*file_out) << bufor << "\n";
	}
	unsigned int getLittleEndianDW(int addr){
		
		return (Memory::read(addr+1)<<8) + Memory::read(addr);
	}

	void irc(){
		if(get_flag(INTERRUPT_DISABLE_FLAG) == 0){
			push_stack((PC>>8)&0x00FF);
			push_stack(PC & 0x00FF);
			set_flag(4,0);
			set_flag(5,1);
			set_flag(INTERRUPT_DISABLE_FLAG,1);
			
			push_stack(P);
			unsigned int addr = 0xFFFE;
			PC = getLittleEndianDW(addr);
		}
	}

	void nmi(){

		push_stack((PC>>8)&0x00FF);
		push_stack(PC & 0x00FF);
		set_flag(4,0);
		set_flag(5,1);
		set_flag(INTERRUPT_DISABLE_FLAG,1);
		push_stack(P);
		unsigned int addr = 0xFFFA;
		PC = getLittleEndianDW(addr);

	}

}

