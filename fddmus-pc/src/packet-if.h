// fddmus packet interface

#ifndef PACKET_IF_H
#define PACKET_IF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NOTE_OFF false
#define NOTE_ON true

// Sent by the drive controller when it is ready to receive another packet
#define PACKET_RESPONSE_RDY 0x52

typedef struct {
  // Frequency of the note
	float note;

  // NOTE_ON/NOTE_OFF
  bool toggle;
}__attribute__((packed)) packet_t;

#endif //PACKET_IF_H
