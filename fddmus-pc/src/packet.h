// fddmus packet interface

#ifndef PACKET_H
#define PACKET_H

#include "packet-if.h"

int packet_init(const char* ttyPath);
int packet_send(packet_t packet);
void packet_wait_for_rdy();

#endif //PACKET_H
