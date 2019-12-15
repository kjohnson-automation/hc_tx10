# hc_tx10
 HC-TX10 Remote Repo

 This is the Arduino replication of the baseband signaling needed for the HC-TX10 Christmas Lights.

 Hardware needed to replicate remote:
 - Arduino Uno, though almost all others can probably be used just change the output pin if needed
 - RF Transmitter from original HC-TX10 remote
 - Breadboard/prototyping board
 - [Arduino DigitalWriteFast Lib](https://code.google.com/archive/p/digitalwritefast/downloads)

I used the Saleae Logic USB Logic Analyzer to get the training sequence as well as the preamble and data sequences for each of the original buttons on the remote and map them to binary sequences.

The bitrate of the data signaling looks to be about 10kb/s or 10kHz. So still quite easy to do on an arduino.

There is a training sequence roughly \~3.5-3.8mS before the data sequence for every signal and that is automatically appended to the data sequence.

The data sequence takes in a string of binary data in str/char[] form and converts it to digital high low so it is very easy to add a sequence/button to the file:
```c
void new_sequence(){
  char sequence[] = "1010101010101000101010101111" // or whatever
  send_pattern(sequence)
}
```

There is also a document attached that includes all included sequences and a defined preamble.  The preamble is not used for anything but is obviously common on all captured data signals.

An example waveform can be seen here:
![Baseband Waveform](/images/waveform.png)

The first part of the waveform is what I've deemed the training pattern, getting the Rx on the lights ready for the data transmission, after that we have the coded preamble followed by the actual sequence data used to set the lights for the operation specified.
It can be broken up as follows:
 - Training Pattern
 - 3.8mS Wait
 - Preamble (10001110111011101)
 - Data
