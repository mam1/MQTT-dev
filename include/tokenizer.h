#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* char_fsm.c version */
#define _MAJOR_VERSION_tokenizer		0
#define _MINOR_VERSION_tokenizer		0


int Tpush(char *token_buffer);
int Qpush(_TOKEN * token, char * string);
char * Tpop(_TOKEN * token);
int tokenizer(char *lbuf);
int reset_tokenQ(void); 
int is_a_delimiter(char * c);
_TOKEN * token_type(_TOKEN *c);



#endif // _TOKENIZER_H_