#include <iostream>
#include "memory.cpp"
#include "decoder.cpp"
#include "ppu.cpp"
#include "cpu.cpp"

using namespace std;

int main(int argc, char** argv) {
		cout << "NES EMULATOR 0.3" ; 
	cout << "##############################"; 
	Decoder::loadRom("roms/donkey_kong.nes");
	glutInit(&argc, argv); 
	glutInitWindowSize(768,768);
	glutCreateWindow("Test");
	
	glutDisplayFunc(PPU::display);
	Decoder::automatic();
	glutIdleFunc(Decoder::tick);
	glutMainLoop();





	return 0;
}