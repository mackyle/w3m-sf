/* $Id: myctype.h,v 1.5 2002/12/24 17:20:47 ukai Exp $ */
#ifndef _MYCTYPE_H
#define _MYCTYPE_H

#define MYCTYPE_CNTRL 1
#define MYCTYPE_SPACE 2
#define MYCTYPE_ALPHA 4
#define MYCTYPE_DIGIT 8
#define MYCTYPE_PRINT 16
#define MYCTYPE_HEX   32
#define MYCTYPE_ALNUM (MYCTYPE_ALPHA|MYCTYPE_DIGIT)
#define MYCTYPE_XDIGIT (MYCTYPE_HEX|MYCTYPE_DIGIT)

#define GET_MYCTYPE(x) (MYCTYPE_MAP[(int)(unsigned char)(x)])
#define GET_MYCDIGIT(x) (MYCTYPE_DIGITMAP[(int)(unsigned char)(x)])

#define IS_CNTRL(x) (GET_MYCTYPE(x) & MYCTYPE_CNTRL)
#define IS_SPACE(x) (GET_MYCTYPE(x) & MYCTYPE_SPACE)
#define IS_ALPHA(x) (GET_MYCTYPE(x) & MYCTYPE_ALPHA)
#define IS_DIGIT(x) (GET_MYCTYPE(x) & MYCTYPE_DIGIT)
#define IS_PRINT(x) (GET_MYCTYPE(x) & MYCTYPE_PRINT)
#define IS_ALNUM(x) (GET_MYCTYPE(x) & MYCTYPE_ALNUM)
#define IS_XDIGIT(x) (GET_MYCTYPE(x) & MYCTYPE_XDIGIT)

extern unsigned char MYCTYPE_MAP[];
extern unsigned char MYCTYPE_DIGITMAP[];


#define INTCTYPE_ASCII   1
#define INTCTYPE_ISPACE  2
#define INTCTYPE_LATIN1  4
#define INTCTYPE_KANJI1  8
#define INTCTYPE_KANJI2  16
#define INTCTYPE_KANJI   (INTCTYPE_KANJI1|INTCTYPE_KANJI2)
#define INTCTYPE_INTRL   INTCTYPE_ISPACE

#define GET_INTCTYPE(x)  (INTCTYPE_MAP[(int)(unsigned char)(x)])

#define IS_ASCII(x)     (GET_INTCTYPE(x) & INTCTYPE_ASCII)
#define IS_INTSPACE(x)  (GET_INTCTYPE(x) & INTCTYPE_ISPACE)
#define IS_INTERNAL(x)  (GET_INTCTYPE(x) & INTCTYPE_INTRL)
#define IS_KANJI1(x)    (GET_INTCTYPE(x) & INTCTYPE_KANJI1)
#define IS_KANJI2(x)    (GET_INTCTYPE(x) & INTCTYPE_KANJI2)
#define IS_KANJI(x)     (GET_INTCTYPE(x) & INTCTYPE_KANJI)
#define IS_LATIN1(x)    (GET_INTCTYPE(x) & INTCTYPE_LATIN1)

#define	TOLOWER(x)	(IS_ALPHA(x) ? ((x)|0x20) : (x))
#define	TOUPPER(x)	(IS_ALPHA(x) ? ((x)&~0x20) : (x))

extern unsigned char INTCTYPE_MAP[];

#endif
