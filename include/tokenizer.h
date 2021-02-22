#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* char_fsm.c version */
#define _MAJOR_VERSION_tokenizer		0
#define _MINOR_VERSION_tokenizer		0


int Tpush(char * token);
char * Tpop(char * token);
int tokenizer(char *lbuf); 

/* command list */
static char    *keyword[_CMD_TOKENS] = {
	/*  0 */    "temp",
	/*  1 */    "*",
	/*  2 */    "humid",
	/*  3 */    "schedule",
	/*  4 */    "?",
	/*  5 */    "display",
	/*  6 */    "yes",
	/*  7 */    "cancel",
	/*  8 */    "replace",
	/*  9 */    "sedit",
	/* 10 */    "delete",
	/* 11 */    "active",
	/* 12 */    "on",
	/* 13 */    "off",
	/* 14 */    "clear",
	/* 15 */    "status",
	/* 16 */    "time",
	/* 17 */    "sensor",
	/* 18 */    "cycle",
	/* 19 */    "ipc",
	/* 20 */    "show",
	/* 21 */    "save",
	/* 22 */    "template",
	/* 23 */    "hide",
	/* 24 */    "load",
	/* 25 */    "set",
	/* 26 */    "q",
	/* 27 */    "done",
	/* 28 */    "back",
	/* 29 */    "system",
	/* 30 */    "channel",
	/* 31 */    "ssch",
	/* 32 */    "wsch",
	/* 33 */    "slib",
	/* 34 */    "tlib",
	/* 35 */    "INT",
	/* 36 */    "STR",
	/* 37 */    "OTHER",
	/* 38 */	"slist",
	/* 39 */	"group",
	/* 40 */	"description"
};


#endif // _TOKENIZER_H_