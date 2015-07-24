#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#define TOKEN_RESISTOR  		256
#define TOKEN_CAPACITOR 	257
#define TOKEN_NODES     		258
#define TOKEN_NUMBER    		259
#define TOKEN_FLOAT     			260
#define TOKEN_INDUCTOR		261
#define TOKEN_EOF					262
#define TOKEN_INPUT				263
#define TOKEN_OUTPUT			264
#define TOKEN_GROUND			265
#define TOKEN_SWEEP				266
#define TOKEN_VCCS				267

#define FALSE	0
#define TRUE	1

class Component;

extern int Lex(FILE *in);
extern int Parse(FILE *in,Component **ppList,Component **ppEnd);



#endif // PARSER_H_INCLUDED
