# Khronos - Version 0.4 (alpha)

Khronos is an intellegent program that utilizes speech recognition to carry out a user's command.  Khronos can also sythesize speech in order to respond to the commands given to him.

---

### How Khronos Works 

The process starts off by recording a `.flac` file with the help of LibSndFile and PortAudio working together.  PortAudio scans for the default input device (set by your operating system), and starts the audio recording process.  Once finished, PortAudio passes along the raw PCM data that was just recorded to LibSndFile, which performs the necessary instructions to store it in a `.flac` file.  

This `.flac` file is then sent off to Google for speech processing.  We specify what the sample rate is of the audio, and what sort of response we would like back from Google (JSON, XML, etc.).  Google sends back a nicely packed JSON file for processing to extract what was said during the recording.  This processing is done with my custom JSON parser that has proven to be faster when compared to the benchmarks of other JSON parsers.

Khronos then takes this processed information and responds to it in an approprate way with it's own speech synthesis software.

---

### Dependencies

This is a list of the required libraries Khronos needs in order to compile properly.

 - CMake (version 2.8.7 minimum): a portable Makefile generator.
 
 - LibSndFile: a library for reading and writing files containing sampled audio data.
 
 - PortAudio: a portable audio I/O library.

 - parcel: an efficient and functional JSON parser.
 
 If CMake is installed and run, it will handle the downloading of all of the other dependencies (if they are not already installed on the system).

---

### Configuration & Installation

You chould be located in the main source directory in your terminal.  Change your directory into the `build/` folder (it should be empty). If it does not exist then create one. This will be where the final build will be housed through the building process.

Run `cmake ..` and the configuation should begin.  This will create a Makefile tailored for running in your specific environment.  Any dependencies that you do not have will also be noted and flagged for downloading.

Next run `make`.  Any flagged dependencies will now begin downloading into their own special folder, where they will have to be configured and built so that Khronos can link to their libraries.  Once everything has finished downloading and everything is linked together, the build should be complete.  Now you can run `./Khronos` and the program should start executing.