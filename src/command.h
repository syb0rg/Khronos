#ifndef COMMAND_H
#define COMMAND_H

#ifndef streq
#define streq(x, y) (strcmp((x), (y)) == 0)
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

typedef struct
{
	const char *cmd;
	const char* (*fn)(void);
} __attribute__((__packed__)) Command;

const char* greetingResponse(void);
const char* test(void);
const char* printVersionInfo(void);
const char* printHelp(void);
const char* getTime(void);
const char* getDate(void);
const char* getDay(void);
const char* exitProgram(void);

static const Command commands[] =
{
	{"hello", greetingResponse},
    {"test", test},
    {"version", printVersionInfo},
    {"help", printHelp},
    {"time", getTime},
	{"date", getDate},
	{"day", getDay},
    {"quit", exitProgram},
    {"exit", exitProgram}
};

#endif // COMMAND_H