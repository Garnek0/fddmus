# FDDMUS

Simple program and packet interface for making floppy drives play MIDI files.

## Dependencies & Other Things You Need

### Required hardware
- A Linux machine (fddmus-pc doesn't support Windows/MacOS (yet))
- Arduino
- Floppy disk drive(s) (the more, the better. Just make sure you have enough pins on your Arduino. A floppy drive takes up 2-3 pins depending on how you wire it up.)
- Some way to power the drives. (Using an ATX/Lab power supply, for example)

### Software dependencies
- libsmf-dev
- Arduino IDE (to upload the Arduino code)

## Building & Vibin'

### Building

Upload the code in `fddmus-arduino` to your Arduino board using Arduino IDE (make sure to change the size and contents of the  `drives` array according to your setup). Next, run `make` in `fddmus-pc`.

### Vibin'

Make sure your board is connected to your PC. Find the TTY device of your board (should be `/dev/ttyACMx` or `/dev/ttyUSBx`) and run `fddmus` from the `fddmus-pc` directory like this:
```
./fddmus -d /dev/yourttydevicehere -m pathtomidifile
```
You can offset all notes in the song by `n` octaves with the `-o` option. This is useful if the song has notes outside of the floppy drives' range:
```
./fddmus -d /dev/yourttydevicehere -m pathtomidifile -o n # play all notes n octaves higher
./fddmus -d /dev/yourttydevicehere -m pathtomidifile -o -n # play all notes n octaves lower
```
Replace `n` with an actual number.

If everything is done correctly, the drives should start playing the MIDI file.
