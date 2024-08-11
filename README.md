# FDDMUS

Simple program and packet interface for making floppy drives play MIDI files.

## Dependencies & Other Things You Need

### Required hardware
- A Linux machine (fddmus-pc doesn't support Windows/MacOS (yet))
- Arduino
- Floppy disk drive(s) (the more, the better. Just make sure you have enough pins on your Arduino. A floppy drive takes up 2-3 pins depending on how you wire them up.)
- Some way to power the drives. (An ATX/Lab power supply, for example)

### Software dependencies
- libsmf-dev
- Arduino IDE (to upload the Arduino code)

## Building & Vibin'

### Building

Upload the code in `fddmus-arduino` to your Arduino board using Arduino IDE. Next, run `make` in `fddmus-pc`.

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
