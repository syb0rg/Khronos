# Khronos - Version 0.5 (alpha) 

Khronos is a program that uses speech recognition to perform a command.  Khronos also synthesizes speech in response to the given commands.

[![Build Status](https://travis-ci.org/syb0rg/Khronos.svg?branch=master)](https://travis-ci.org/syb0rg/Khronos) [![Gitter](https://badges.gitter.im/khronos-project/Lobby.svg)](https://gitter.im/khronos-project/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

---

### How Khronos Works 

The process starts by recording a `.wav` file with LibSndFile and PortAudio.  PortAudio finds the default input device, and starts recording audio.  Once finished, PortAudio passes along the raw PCM data to LibSndFile, which stores it in a `.wav` file.  

This `.wav` file is passed to pocketsphinx for speech processing, which then returns the spoken text. Khronos takes this text and responds with its [own speech synthesis software](https://syb0rg.github.io/tritium.io/).

---

### Dependencies

This is a list of the libraries Khronos requires to compile properly.

- [CMake](https://cmake.org/) (version 2.8 minimum): a portable Makefile generator.

- [git](https://git-scm.com/): distributed version control system for downloading missing dependencies.

- [LibSndFile](http://www.mega-nerd.com/libsndfile/): a library for reading and writing files containing sampled audio data.

- [PortAudio](https://travis-ci.org/syb0rg/PortAudio2): a portable audio I/O library.

- [sphinxbase](https://github.com/syb0rg/sphinxbase): the basic libraries shared by the CMU Sphinx trainer and all the Sphinx decoders

- [pocketsphinx](https://github.com/syb0rg/pocketsphinx): open source, large vocabulary, speaker-independent speech recognition engine

- [tritium](https://syb0rg.github.io/tritium.io/): a free, premium quality speech synthesis engine written completely in C.

If CMake and git are installed, Khronos will handle the downloading of all of the other dependencies (if they are not already installed on the system).

---

### Configuration & Installation Instructions

##### Windows

In addition to CMake, Windows builds require a C compiler such as [MinGW-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download).  Cygwin may also be an option, but has not been tested.  Instructions are based on a build with MinGW-w64.

1. Make sure your PATH variable contains the location to the MinGW-w64 `bin/` folder.
2. Locate the main source directory in your terminal.  Change into the `build/` folder (it should be empty, create it if it does not exist).
3. Run `cmake -G "MinGW Makefiles" ..` and configuration should begin.  This will create a Makefile tailored for your specific environment.  Any dependencies that you need will be flagged for downloading.
4. Run `make`.  All flagged dependencies will download to be configured and built for Khronos to link with.  Once everything has finished downloading and linked together, the build should be complete.  Now you can run `Khronos.exe`.

##### Unix

1. Locate the main source directory in your terminal.  Change into the `build/` folder (it should be empty, create it if it does not exist).
2. Run `cmake ..` and configuration should begin.  This will create a Makefile tailored for your specific environment.  Any dependencies that you need will be flagged for downloading.
3. Run `make`.  All flagged dependencies will download to be configured and built for Khronos to link with.  Once everything has finished downloading and linked together, the build should be complete.  Now you can run `./Khronos`.
