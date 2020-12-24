#include <stdio.h>
#pragma once
namespace PPU{
	void writePPUCTRL(unsigned char value);
	void writeDATA(unsigned char value);
	void writePPUADDR(unsigned char value);
	void writeOAMDATA(unsigned char value);
	void writeOAMDMA(unsigned char value);
	void writeOAMADDR(unsigned char value);
	unsigned char readOAMDATA();
	unsigned char readPPUSTATUS();
};
namespace Memory{
	
	unsigned char RAM[0x10000];
	

	unsigned char read(unsigned int addr){
		if(addr == 0x2002){
			return PPU::readPPUSTATUS();
		}
		else if(addr == 0x2004){
			return PPU::readOAMDATA();
		}
		if(addr <= 0x2000)return RAM[addr%0x800];
		return RAM[addr];
	}
	void write(int addr,unsigned char value){
	//	printf("write at adress %d value %d\n" ,addr,value );
		if(addr == 0x2000){
			PPU::writePPUCTRL(value);
		}
		else if(addr == 0x2001){

		}
		else if(addr == 0x2003){
			PPU::writeOAMADDR(value);
		}
		else if(addr == 0x2004){
			PPU::writeOAMDATA(value);
		}
		else if(addr == 0x2005){

		}
		else if(addr == 0x2006){
			PPU::writePPUADDR(value);

		}
		else if(addr == 0x2007){
			PPU::writeDATA(value);
		}
		else if(addr == 0x4014){
			PPU::writeOAMDMA(value);
		}
		else if(addr < 0x2000 ) RAM[addr%0x800] = value;
		else{
			RAM[addr] = value;
		}

	}
}
