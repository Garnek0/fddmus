#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "packet-if.h"
#include "packet.h"
#include "midi.h"

int main(int argc, char** argv){
	system("clear");

	printf("\n"
			" /$$$$$$$$ /$$$$$$$  /$$$$$$$  /$$      /$$ /$$   /$$  /$$$$$$ \n"
			"| $$_____/| $$__  $$| $$__  $$| $$$    /$$$| $$  | $$ /$$__  $$\n"
			"| $$      | $$  \\ $$| $$  \\ $$| $$$$  /$$$$| $$  | $$| $$  \\__/\n"
			"| $$$$$   | $$  | $$| $$  | $$| $$ $$/$$ $$| $$  | $$|  $$$$$$ \n"
			"| $$__/   | $$  | $$| $$  | $$| $$  $$$| $$| $$  | $$ \\____  $$\n"
			"| $$      | $$  | $$| $$  | $$| $$\\  $ | $$| $$  | $$ /$$  \\ $$\n"
			"| $$      | $$$$$$$/| $$$$$$$/| $$ \\/  | $$|  $$$$$$/|  $$$$$$/\n"
			"|__/      |_______/ |_______/ |__/     |__/ \\______/  \\______/ \n"
			"Ver. 1.0\n\n");
	printf(""
			"|----------------------------------------|\n"
			"| |------------------------------------| |\n"
			"| |___________              ___________| |\n"
			"|     _       \\____________/   ____      |\n"
			"|    |_|                      |____|     |\n"
			"|----------------------------------------|\n"
			"\n");
	printf("By Popa Vlad (Garnek)\n\n\n");

	int opt;
	char* dev = NULL;
	char* smfFile = NULL;
	int octaveMod = 0;
    while((opt = getopt(argc, argv, "d:m:o:")) != -1) {
        switch (opt) {
        	case 'd':
				dev = optarg;
				break;
			case 'm':
				smfFile = optarg;
				break;
			case 'o':
				octaveMod = atoi(optarg);
				break;
			default:
        		fprintf(stderr, "Usage: %s -d uartDevice -m midiFile [-o octaveModifier]\n"
				                "-d uartDevice: The UART device of the controller board\n"
				                "-m midiFile: Standard MIDI File (SMF) format file to be played\n"
				  				"-o octaveModifier: Relative octave modifier. Default value is 0 (song is played at pitch, no octave modifications)\n", argv[0]);
	    		return 1;
				break;
		}
	}

	if(!dev || !smfFile){
		fprintf(stderr, "Usage: %s -d uartDevice -m midiFile [-o octaveModifier]\n"
				                "-d uartDevice: The UART device of the controller board\n"
				                "-m midiFile: Standard MIDI File (SMF) format file to be played\n"
				  				"-o octaveModifier: Relative octave modifier. Default value is 0 (song is played at pitch, no octave modifications)\n", argv[0]);
		return 1;
	}

	if(packet_init(dev) != 0){
		return 1;
	}	

	if(midi_play(smfFile, octaveMod) != 0){
		return 1;
	}

	return 0;
}
