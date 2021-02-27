#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/* char_fsm.c version */
#define _MAJOR_VERSION_tokenizer		0
#define _MINOR_VERSION_tokenizer		0


int Tpush(char * token);
int Qpush(char * token, char * string);
char * Tpop(char * token);
int tokenizer(char *lbuf);
int reset_tokenQ(void); 
int is_a_delimiter(char * c);



#endif // _TOKENIZER_H_