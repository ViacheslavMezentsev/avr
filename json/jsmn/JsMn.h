#pragma once


/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
enum EnJsonType {

	JSMN_PRIMITIVE = 0,
	JSMN_OBJECT = 1,
	JSMN_ARRAY = 2,
	JSMN_STRING = 3

};


enum EnJsonError {

	/* Not enough tokens were provided */
	JSMN_ERROR_NOMEM = -1,

	/* Invalid character inside JSON string */
	JSMN_ERROR_INVAL = -2,

	/* The string is not a full JSON packet, more bytes expected */
	JSMN_ERROR_PART = -3,

	/* Everything was fine */
	JSMN_SUCCESS = 0

};


/**
 * JSON token description.
 * @param		type	type (object, array, string etc.)
 * @param		start	start position in JSON data string
 * @param		end		end position in JSON data string
 */
typedef struct {

	EnJsonType type;
	int start;
	int end;
	int size;
#ifdef JSMN_PARENT_LINKS
	int parent;
#endif

} jsmntok_t;


/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {

	unsigned int pos; /* offset in the JSON string */
	int toknext; /* next token to allocate */
	int toksuper; /* superior token node, e.g parent object or array */

} jsmn_parser;


class CJsMn {

private:

    /**
     * Allocates a fresh unused token from the token pull.
     */
    static jsmntok_t * AllocToken( jsmn_parser * parser, 
        jsmntok_t * tokens, size_t num_tokens );

    /**
     * Fills token type and boundaries.
     */
    static void FillToken( jsmntok_t * token, EnJsonType type, 
        int start, int end );

    /**
     * Fills next available token with JSON primitive.
     */
    static EnJsonError ParsePrimitive( jsmn_parser * parser, const char * js,
		jsmntok_t * tokens, size_t num_tokens );

    /**
     * Fills next token with JSON string.
     */
    static EnJsonError ParseString( jsmn_parser * parser, const char * js,
		jsmntok_t * tokens, size_t num_tokens );

public:

    /**
     * Create JSON parser over an array of tokens
     */
    CJsMn( jsmn_parser * parser );

    /**
     * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
     * a single JSON object.
     */
    static EnJsonError Parse( jsmn_parser * parser, const char * js, 
        jsmntok_t * tokens, unsigned int num_tokens );

};

