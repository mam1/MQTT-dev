#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* char_fsm.c version */
#define _MAJOR_VERSION_tokenizer		0
#define _MINOR_VERSION_tokenizer		0


int Tpush(char * token);
char * Tpop(char * token);
int tokenizer(char *lbuf, char *string); 



#endif // _TOKENIZER_H_