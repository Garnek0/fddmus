#ifndef MIDI_H
#define MIDI_H

#define MIDI_C0 0
#define MIDI_Cs0 1
#define MIDI_D0 2
#define MIDI_Ds0 3
#define MIDI_E0 4
#define MIDI_F0 5
#define MIDI_Fs0 6
#define MIDI_G0 7
#define MIDI_Gs0 8
#define MIDI_A0 9
#define MIDI_As0 10
#define MIDI_B0 11

#define FREQ_C0 16.35
#define FREQ_Cs0 17.32
#define FREQ_D0 18.35
#define FREQ_Ds0 19.45
#define FREQ_E0 20.60
#define FREQ_F0 21.83
#define FREQ_Fs0 23.12
#define FREQ_G0 24.50
#define FREQ_Gs0 25.96
#define FREQ_A0 27.50
#define FREQ_As0 29.14
#define FREQ_B0 30.87

int midi_play(const char* file, int octaveMod);

#endif //MIDI_H
