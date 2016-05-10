# Khronos - Version 0.4 (alpha)

Khronos is an intellegent program that utilizes speech recognition to carry out a user's command.  Khronos can also sythesize speech in order to respond to the commands given to him.

---

### How Khronos Works 

The process starts by recording a `.wav` file with LibSndFile and PortAudio.  PortAudio finds the default input device, and starts recording audio.  Once finished, PortAudio passes along the raw PCM data to LibSndFile, which stores it in a `.wav` file.  

This `.wav` file is passed to pocketsphinx for speech processing, which then returns the spoken text. Khronos takes this text and responds with its [own speech synthesis software](https://syb0rg.github.io/tritium.io/).

---

### Dependencies

This is a list of the libraries Khronos requires to compile properly.

 - [CMake](https://cmake.org/) (version 2.8.7 minimum): a portable Makefile generator.
 
 - [LibSndFile](http://www.mega-nerd.com/libsndfile/): a library for reading and writing files containing sampled audio data.
 
 - [PortAudio](http://www.portaudio.com/): a portable audio I/O library.

 - [sphinxbase](https://github.com/cmusphinx/sphinxbase): the basic libraries shared by the CMU Sphinx trainer and all the Sphinx decoders

 - [pocketsphinx](https://github.com/cmusphinx/pocketsphinx): open source, large vocabulary, speaker-independent speech recognition engine

 - [tritium](https://syb0rg.github.io/tritium.io/): a free, premium quality speech synthesis engine written completely in C.
 
 If CMake is installed and run, it will handle the downloading of all of the other dependencies (if they are not already installed on the system).

---

### Configuration & Installation

You should be in the main source directory in your terminal.  Change into the `build/` folder (it should be empty, if it does not exist then create it). This will be where the final build will be housed.

Run `cmake ..` and configuation should begin.  This will create a Makefile tailored for your specific environment.  Any dependencies that you need will be flagged for downloading.

Run `make`.  All flagged dependencies will download into their own special folder, where they are configured and built for Khronos to link with.  Once everything has finished downloading and linked together, the build should be complete.  Now you can run `./Khronos`.