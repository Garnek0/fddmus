#include "packet.h"
#include "packet-if.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int tty;

int packet_init(const char* ttyPath){
	tty = open(ttyPath, (O_RDWR | O_NOCTTY | O_SYNC));
	if(tty < 0){
		goto packetInitFail;
	}

	printf("Initialising UART interface. Board may reset.\n");

	struct termios status;
	memset(&status, 0, sizeof(struct termios));

	if(tcgetattr(tty, &status) != 0){
		close(tty);
		goto packetInitFail;
	}

	cfsetospeed(&status, B115200);
	cfsetispeed(&status, B115200);

	status.c_cflag &= ~CSIZE;
	status.c_cflag |= CS8;
	status.c_iflag &= ~IGNBRK;
	status.c_lflag = 0;
	status.c_oflag = 0;
	status.c_cc[VMIN] = 1;
	status.c_cc[VTIME] = 10;

	status.c_iflag &= ~(IXON | IXOFF |IXANY);

	status.c_cflag |= (CLOCAL | CREAD);

	status.c_cflag &= ~(PARENB | PARODD);
	status.c_cflag &= ~CSTOPB;
	status.c_cflag &= ~CRTSCTS;

	if(tcsetattr(tty, TCSANOW, &status) != 0){
		close(tty);
		goto packetInitFail;
	}

	//Recent arduino board models reset when connecting to the UART.
	//Wait for the reset cycle to finish before proceeding.
	sleep(5);

	printf("TTY device \'%s\' initialised for sending packets.\n", ttyPath);

	return 0;

packetInitFail:
	fprintf(stderr, "Could not initialise TTY device \'%s\' for sending packets: %s\n", ttyPath, strerror(errno));
	return -1;
}

int packet_send(packet_t packet){
	if(write(tty, &packet, sizeof(packet_t)) <= 0){
		fprintf(stderr, "Failed to send packet!\n");
		return -1;
	}

	printf("Sent %s packet. (note freq = %f)\n", packet.toggle == NOTE_ON ? "\e[0;32mNOTE_ON\e[0m" : "\e[0;31mNOTE_OFF\e[0m", packet.note);

	return 0;
}

void packet_wait_for_rdy(){
	char chr = 0;

	while(chr != PACKET_RESPONSE_RDY){
		read(tty, &chr, 1);
	}

	return;
}
