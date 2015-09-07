/**
 * @file json.h
 * @brief JSON structures, enums, and relevent function declarations.
 */
#ifndef JSON_H
#define JSON_H

#include <unistd.h>

/**
 * JSON type identifier. Basic types are:
 * 	- Object
 * 	- Array
 * 	- String
 * 	- Other primitive: number, boolean (true/false) or null
 */
typedef enum { JSON_PRIMITIVE, JSON_OBJECT, JSON_ARRAY, JSON_STRING } JsonType;

typedef enum
{
	JSON_ERROR_NOMEM = -1, // Not enough tokens were provided
	JSON_ERROR_INVAL = -2, // Invalid character inside JSON string
	JSON_ERROR_PART = -3, // The string is not a full JSON packet, more bytes expected
	JSON_SUCCESS = 0 // Everthing is fine
} JsonError;

/**
 * JSON token description.
 * @var type The type (object, array, string etc.)
 * @var start The start position in JSON data string.
 * @var end The end position in JSON data string.
 */
typedef struct
{
	JsonType type;
	ssize_t start;
	ssize_t end;
	ssize_t size;
	ssize_t parent;
} __attribute__((__packed__)) JsonToken;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 * @var pos Contains the offset in the JSON string.
 * @var toknext Contains the next token to allocate.
 * @var toksuper Contains the superior token node (e.g parent object or array).
 */
typedef struct
{
	ssize_t pos;
	ssize_t toknext;
	ssize_t toksuper;
} __attribute__((__packed__)) JsonParser;

/// Create JSON parser over an array of tokens
void json_initParser(JsonParser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
 * a single JSON object.
 */
JsonError json_parse(JsonParser *parser, const char *js, JsonToken *tokens, unsigned int tokenNum);

/// Retrieve an item from the given JSON data.
const char* getItemFromJSON(const char *json, const char *item);

#endif // JSON_H