# Khronos - Version 0.4 (alpha)

Khronos is an intellegent program that utilizes speech recognition to carry out a user's command.  Khronos can also sythesize speech in order to respond to the commands given to him.

---

### How Khronos Works 

The process starts off by recording a `.wav` file with the help of LibSndFile and PortAudio working together.  PortAudio scans for the default input device (set by your operating system), and starts the audio recording process.  Once finished, PortAudio passes along the raw PCM data that was just recorded to LibSndFile, which performs the necessary instructions to store it in a `.wav` file.  

This `.wav` file is then given to pocketsphinx for speech processing, which then returns the said text with reasonable accuracy. Khronos then takes this processed information and responds to it in an approprate way with it's [own speech synthesis software](https://syb0rg.github.io/tritium.io/).

---

### Dependencies

This is a list of the required libraries Khronos needs in order to compile properly.

 - CMake (version 2.8.7 minimum): a portable Makefile generator.
 
 - LibSndFile: a library for reading and writing files containing sampled audio data.  This library has a subdependecy on the FLAC library.
 
 - PortAudio: a portable audio I/O library.

 - sphinxbase: the basic libraries shared by the CMU Sphinx trainer and all the Sphinx decoders

 - pocketsphinx: open source large vocabulary, speaker-independent continuous speech recognition engine

 - tritium: a free, premium quality speech synthesis engine written completely in C.
 
 If CMake is installed and run, it will handle the downloading of all of the other dependencies (if they are not already installed on the system).

---

### Configuration & Installation

You chould be located in the main source directory in your terminal.  Change your directory into the `build/` folder (it should be empty). If it does not exist then create one. This will be where the final build will be housed through the building process.

Run `cmake ..` and the configuation should begin.  This will create a Makefile tailored for running in your specific environment.  Any dependencies that you do not have will also be noted and flagged for downloading.

Next run `make`.  Any flagged dependencies will now begin downloading into their own special folder, where they will have to be configured and built so that Khronos can link to their libraries.  Once everything has finished downloading and everything is linked together, the build should be complete.  Now you can run `./Khronos` and the program should start executing.