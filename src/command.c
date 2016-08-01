#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <tritium_version.h>
#include <portaudio.h>
#include <sndfile.h>

#include "command.h"
#include "color.h"

#define KHRONOS_VERSION "0.5"
#define KHRONOS_VERSION_STATE "alpha"
#define DATE_LENGTH_MAX 10


const char* greetingResponse(void)
{
	const char *greetings[] = {"Hello", "Hi", "Hello there", "How is it going?", "Howdy", "How are you?"};
	const int random = rand() % ARRAY_SIZE(greetings);
	return greetings[random];
}

const char* test(void)
{
    return "Test complete";
}

const char* printVersionInfo(void)
{
	fprintf(stdout, BLUE_TEXT("Khronos version: %s (%s)\n"), KHRONOS_VERSION, KHRONOS_VERSION_STATE);
	fprintf(stdout, "Khronos submodule version information:\n");
	fprintf(stdout, "\t- %s\n", Pa_GetVersionText());
	fprintf(stdout, "\t- %s (%s) %s\n", TRITIUM_PROJECT_NAME, TRITIUM_PROJECT_PREFIX, TRITIUM_PROJECT_VERSION);
	fprintf(stdout, "\t- %s\n", sf_version_string());
	return "";
}

const char* printHelp(void)
{
	const char *helpString = "Khronos - an efficient artificial intelligence system\n"
							 "usage: ./Khronos [options] [options-input]\n"
							 "  --help, -h, help \t Displays this help information\n"
							 "  --version, -v, version \t Prints all version information\n"
							 "  --duration, -v, duration \t Changes the duration of the recording\n"
							 "  --say, -s, say <text> \t Uses the speech synthesis engine to say <text>";
	return helpString;
}

const char* getTime(void)
{
	time_t currentTime = time(NULL);
	// printf("%d:%d %s (%s)\n", hour <= 12 ?: hour - 12, tm.tm_min, tm.tm_isdst ? "PM" : "AM", tm.tm_zone);
	return ctime(&currentTime);
}

const char* getDate(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	return asctime(&tm);
}

const char* getDay(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	switch(tm.tm_wday)
	{
		case 0:
			return "Sunday";
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
		default:
			return "Unknown";
	}
}

const char* exitProgram(void)
{
    exit(EXIT_SUCCESS);
    return "";
}
