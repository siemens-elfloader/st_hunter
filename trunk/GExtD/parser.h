/* 
 * File:   parser.h
 * Author: Tonich
 *
 * Created on 27 Октябрь 2011 г., 18:55
 */

#ifndef PARSER_H
#define	PARSER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define NONE_COMMENT            0
#define MULTILINE_COMMENT       1
#define ONELINE_COMMENT         2

typedef struct _str_in_mem{
    char *str;
    int len;
}SIM;

typedef struct _strs_in_mem{
    SIM str;
    char comtype;
    struct _strs_in_mem *next,*prev;
}SSIM;

SSIM *DoParse(char *mem);
void ParseClear(SSIM *root);

#ifdef	__cplusplus
}
#endif

#endif	/* PARSER_H */

