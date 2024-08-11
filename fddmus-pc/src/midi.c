#include "midi.h"
#include "packet.h"
#include "packet-if.h"

#include <smf.h>
#include <unistd.h>

static float midi_get_freq_from_note_on(smf_event_t* event, int octaveMod){
	float octaveMultiplier = 1;
	float freq;
	int note;
	if(event->midi_buffer[0] == 0x90 || event->midi_buffer[0] == 0x80){
		note = event->midi_buffer[1];
		while(note - 12 >= 0){
			note-=12;
			octaveMultiplier*=2;
		}

		if(octaveMod > 0){
			for(int i = 0; i < octaveMod; i++) octaveMultiplier*=2;
		} else if(octaveMod < 0){
			for(int i = 0; i < -octaveMod; i++) octaveMultiplier/=2;
		}

		switch(note){
			case 0:
			{
				freq = FREQ_C0 * octaveMultiplier;
				break;
			}
			case 1:
			{
				freq = FREQ_Cs0 * octaveMultiplier;
		    	break;
			}
			case 2:
			{
				freq = FREQ_D0 * octaveMultiplier;
				break;
			}
			case 3:
			{
				freq = FREQ_Ds0 * octaveMultiplier;
				break;
			}
			case 4:
			{
				freq = FREQ_E0 * octaveMultiplier;
				break;
			}
			case 5:
			{
				freq = FREQ_F0 * octaveMultiplier;
				break;
			}
			case 6:
			{
				freq = FREQ_Fs0 * octaveMultiplier;
				break;
			}
			case 7:
			{
				freq = FREQ_G0 * octaveMultiplier;
				break;
			}
			case 8:
			{
				freq = FREQ_Gs0 * octaveMultiplier;
				break;
			}
			case 9:
			{
				freq = FREQ_A0 * octaveMultiplier;
				break;
			}
			case 10:
			{
				freq = FREQ_As0 * octaveMultiplier;
				break;	
			}
			case 11:
			{
				freq = FREQ_B0 * octaveMultiplier;
				break;
			}
			default:
			{
				fprintf(stderr, "Unknown Note!!!\n");
				break;
			}
		}
	}

	return freq;
}

int midi_play(const char* filename, int octaveMod){
	smf_t* smf = smf_load(filename);
	if(!smf){
		fprintf(stderr, "Failed to load %s!\n", filename);
		return -1;
	} else {
		printf("%s loaded!\nNumber of Tracks: %u\nPPQN: %u\n\n", filename, smf->number_of_tracks, smf->ppqn);
	}

	smf_track_t* track = smf_get_track_by_number(smf, 1);
	if(!track){
		fprintf(stderr, "Failed to load MIDI Track 1.\n");
		return -1;
	} else {
		printf("MIDI Track 1 loaded!\n");
	}

	packet_t packet;
	smf_tempo_t* tempo = smf_get_tempo_by_number(smf, 0);
	if(!tempo){
		fprintf(stderr, "Failed to get first tempo.\n");
		return -1;
	} {
		printf("Got first tempo.\nMicroseconds per Quarter Note: %u\n", tempo->microseconds_per_quarter_note);
	}

	long long pulses = 0;
	long long microsPerPulse = tempo->microseconds_per_quarter_note/smf->ppqn;  

	for(smf_event_t* event = smf_track_get_next_event(track); event; event = smf_track_get_next_event(track)){
		//TODO: add support for tempo changes

		if(event->midi_buffer[0] == 0x90){
			packet.note = midi_get_freq_from_note_on(event, octaveMod); 
			packet.toggle = NOTE_ON;

			usleep(microsPerPulse*(event->time_pulses-pulses));
			pulses = event->time_pulses;

			packet_send(packet);
			packet_wait_for_rdy();
		}

		if(event->midi_buffer[0] == 0x80){
			packet.note = midi_get_freq_from_note_on(event, octaveMod);
			packet.toggle = NOTE_OFF;

			usleep(microsPerPulse*(event->time_pulses-pulses));
			pulses = event->time_pulses;

			packet_send(packet);
			packet_wait_for_rdy();
		}
	}

	return 0;
}
