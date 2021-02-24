#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* char_fsm.c version */
#define _MAJOR_VERSION_tokenizer		0
#define _MINOR_VERSION_tokenizer		0


int Tpush(char * token, char * string);
int * Tpop(char * token);
int tokenizer(char *lbuf); 



#endif // _TOKENIZER_H_