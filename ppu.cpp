
#define NMI_ENABLE 7
#define PPU_CTRL 0x2000
#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <winuser.h>
using namespace std;

namespace PPU{
	#define WIDTH 256
	#define HEIGHT 256
	//	NES color palette
unsigned int palette[64] = {
		0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400, 0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
		0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10, 0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
		0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044, 0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
		0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8, 0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
};
	int cycles = 0;
	ofstream *FOUT;
	unsigned char PPUCTRL;
	unsigned char PPUMASK;
	unsigned char PPUSTATUS;
	unsigned char OAMDATA[256];
	unsigned char PPUSCROLL;
	unsigned int PPUADDR;
	unsigned char PPUDATA;
	unsigned char OAMDMA;
	unsigned char OAMADDR;
	unsigned char VRAM[0xFFFFFF];
	unsigned char pixels[WIDTH][HEIGHT][3];
	int ppu_adr_phase = 0;
	unsigned short keys[8];
	unsigned char key_place = -1;
	
	void resetCtrl(){
		for(int i = 0 ; i < 8;i++)keys[i]=0;
	}
	void resetKeyPlace(){
		
		key_place = 0;
	}
	unsigned char readCtrl(){
		if(key_place>7){
			key_place = -1;
	
		}
		if(key_place < 0 )return 0;
	
		return keys[key_place++]!=0;
	}
	void getKeys(unsigned char k,int x, int y){
	
	
		keys[0]=GetAsyncKeyState('O');
		keys[1]=GetAsyncKeyState('P');;
		keys[2]=GetAsyncKeyState('T');;
		keys[3]=GetAsyncKeyState('Y');;
		keys[4]=GetAsyncKeyState('W');;
		keys[5]=GetAsyncKeyState('S');;
		keys[6]=GetAsyncKeyState('A');;
		keys[7]=GetAsyncKeyState('D');;
		
	}

	unsigned int getNtAddress(){
		unsigned char bits = PPUCTRL&3;
		return 0x2000 + 0x400 * bits;
	}
	unsigned int getBgAddress(){
		return ((PPUCTRL & (1<<4))!=0)*0x1000;
	}
	unsigned int getSpAddress(){
		return 0x0000;
		return ((PPUCTRL & (1<<3))!=0)*0x1000;
	}
	unsigned int getIncrement(){
		if((PPUCTRL & (1<<2)) != 0 )return 32;
		else return 1;
	}
	unsigned char readOAMDATA(){
		return OAMDATA[OAMADDR];
	}

	unsigned char readPPUSTATUS(){
		return PPUSTATUS;
	}
	void writePPUCTRL(unsigned char data){
		PPUCTRL = data;
	}
	void writePPUADDR(unsigned char data){
		(*FOUT) << "wrote PPUADDR " << hex << (unsigned int)data << "\n";
		
		if(ppu_adr_phase==0){
			PPUADDR = data << 8;
			ppu_adr_phase = 1;
		}
		else{
			PPUADDR |= data;
			ppu_adr_phase = 0;
		}
	}

	void writeDATA(unsigned char val){
		(*FOUT) << "wrote PPUDATA " << hex<< (unsigned int)val << " at ADDR " << hex <<(unsigned int) PPUADDR<<"\n";
		
		
		VRAM[PPUADDR]=val;
		PPUADDR+=getIncrement();
	}
	void writeOAMADDR(unsigned char var){
		OAMADDR = var;
	}
	void writeOAMDATA(unsigned char v){
		
		OAMDATA[OAMADDR++] = v;
	}
	//(4014)
	void writeOAMDMA(unsigned char v){
		for(int i = 0 ; i < 256;i++){
			OAMDATA[i] = Memory::read((v<<8) + i);
		}
	}

	class Pixel{
		public:
			char z;
			Pixel(char z){
				this->z = z;
			};
	};
	void draw(int x,int y,unsigned char z){
		if(x>=256)return;
		if(x<0)return;
		if(y>=240)return;
		if(y<0)return;
		COORD c;
		c.X = x;
		c.Y = y;
		//HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		//SetConsoleCursorPosition(h,c);
		pixels[x][y][0] = (palette[z] >> 16)&0xff;
		pixels[x][y][1] = (palette[z] >> 8)&0xff;
		pixels[x][y][2] = (palette[z])&0xff;
		//cout << (char)z;
		
		
	}
	void move(int x,int y){
		COORD c;
		c.X = x;
		c.Y = y;
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(h,c);
	}
	
	void write(){

	}
	void display() {
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
		//glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	 	glEnable (GL_TEXTURE_2D);
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Draw a Red 1x1 Square centered at origin
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,WIDTH,HEIGHT,0,
		GL_RGB,GL_UNSIGNED_BYTE,&pixels[0][0][0]);
		glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0, -1.0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0,  1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0,  1.0);
  		glEnd();
		glFlush();  // Render now
		glutSwapBuffers();
	}
	void init(){

	
	
		FOUT = new ofstream("ppu.txt");
		(*FOUT) << "FILE INIT" << "\n";
	}
	void drawChr(){
		int width = 256;
		int height = 128;
		int offset = 0;
		for(int i = 0 ; i < 256;i++){
			for(int j = 0 ; j < 128;j++){
			
			
				int adr = (i/8 * 0x100) + (i%8) + ( j/8) * 0x10;

				unsigned char p = ((VRAM[adr] >> (7-(j%8)))&1) +
				((VRAM[adr+8] >> (7-(j%8)))&1) * 2;
				//printf("%X %X %X\n",adr,Memory::read(adr),palette[p]);
				draw(256-i,j, p*63 );
			}
		}
	}
	void draw(){
		int width = 240;
		int height = 256;
		int offset = 0;
		for(int i = 0 ; i < width;i++){
			for(int j = 0 ; j < height;j++){
				int tile_id = ((i/8) * 32) + (j/8);
				int tile_nr = VRAM[0x2000 + tile_id];
				int tile_atr = VRAM[((0x2000 + tile_id)&0xfc00) + 0x03c0
				+  ((i/32)*8 ) + (j/32)];
				int attr =(((tile_id%32)/2 %2 ) +(tile_id / 64 % 2)*2)*2;
				int pal = ((tile_atr >> attr) & 0x3)*4;
				int adr = getBgAddress()+ (tile_nr * 0x10)+(i % 8);
				unsigned char p = 	((VRAM[adr] >> (7-(j%8)))&1) +
				(((VRAM[adr+8] >> (7-(j%8)))&1) * 2);

				int x =  j;
				int y = width- i;
				draw(y,x, VRAM[p + pal + 0x3f00] );
	
			}
		}
		
	}
	void drawSprite(){
		for(int i = 63; i >=0 ; i--){
			unsigned char Y_pos = OAMDATA[i*4];
			unsigned char pattern = OAMDATA[i*4 + 1];
			unsigned char attr = OAMDATA[i*4 + 2];
			unsigned char X_pos = OAMDATA[i*4 + 3];
			unsigned int pal = 0x3f10 + ((attr & 3) *4);
			for(int j = 0 ; j < 8 ; j ++){
				for(int k = 0 ; k < 8; k++){
					unsigned char type = 	(attr>>6)&3;
					unsigned char p;
					if(type == 00){
					p = 
					((VRAM[getSpAddress() + pattern * 0x10 + j] >> (7 -(k%8)))&1)
					+ ((VRAM[getSpAddress() + pattern * 0x10 + j + 8] >> 
					(7 -(k%8)))&1) * 2;
					}
					else if(type==1){
						p=(((VRAM[getSpAddress() + pattern * 0x10 + j] >> (k%8)))&1)
					+ ((VRAM[getSpAddress() + pattern * 0x10 + j + 8] >> 
					((k%8)))&1) * 2;
					}					
				//	printf("%d %d %d \n",Y_pos+j,Y_pos+k,VRAM[pal+p]);
					if(p==0)continue;
					draw(240-(Y_pos+1+j) ,X_pos+k,VRAM[pal + p]);
				}
			}	
		}
	}
	int keyboard_cycle = 0;
	void tick(){

		
		 keyboard_cycle += 1;
		if(keyboard_cycle > 10){
			keyboard_cycle = 0;
			getKeys(0,0,0);
		}
	//	printf("%X",Memory::read(PPU_CTRL));
		//draw();
		cycles+=3;
	
//		move(300,100);
		if(cycles>=30000 ){
			CPU::nmi();
			PPUSTATUS = 0x80;
			cycles=-10000;
			//set_flag(PPU_CTRL,NMI_ENABLE,0);
		
			//	printf("ppu draw\n");
		}
		else if(cycles>=20100 && cycles <= 20103){
			PPUSTATUS = 0x0;
			draw();
			drawSprite();
		display();
		
		}
	}
};
