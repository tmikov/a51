typedef union
{
  UINT     uValue;
  TIdent * pIdent;
  TExpr    expr;
} YYSTYPE;
#define	LEX_IF	258
#define	LEX_ELSE	259
#define	LEX_ENDIF	260
#define	LEX_IFDEF	261
#define	LEX_IFNDEF	262
#define	LEX_A	263
#define	LEX_C	264
#define	LEX_DPTR	265
#define	LEX_PC	266
#define	LEX_AB	267
#define	LEX_DOLLAR	268
#define	LEX_ORG	269
#define	LEX_USING	270
#define	LEX_END	271
#define	LEX_AT	272
#define	LEX_CSEG	273
#define	LEX_DSEG	274
#define	LEX_ISEG	275
#define	LEX_BSEG	276
#define	LEX_XSEG	277
#define	LEX_EQU	278
#define	LEX_SET	279
#define	LEX_CODE	280
#define	LEX_DATA	281
#define	LEX_IDATA	282
#define	LEX_BIT	283
#define	LEX_XDATA	284
#define	LEX_DB	285
#define	LEX_DW	286
#define	LEX_DS	287
#define	LEX_DBIT	288
#define	LEX_NOMOD51	289
#define	LEX_NOPAGING	290
#define	LEX_PAGELENGTH	291
#define	LEX_NOSYMBOLS	292
#define	LEX_DEBUG	293
#define	LEX_PAGEWIDTH	294
#define	LEX_NOTABS	295
#define	LEX_XREF	296
#define	LEX_TITLE	297
#define	LEX_NAME	298
#define	LEX_DATE	299
#define	LEX_INCLUDE	300
#define	LEX_EJECT	301
#define	LEX_NOLIST	302
#define	LEX_LIST	303
#define	LEX_EXTRN	304
#define	LEX_PUBLIC	305
#define	LEX_OR	306
#define	LEX_XOR	307
#define	LEX_AND	308
#define	LEX_NE	309
#define	LEX_LE	310
#define	LEX_GE	311
#define	LEX_SHL	312
#define	LEX_SHR	313
#define	LEX_MOD	314
#define	UPLUS	315
#define	UMINUS	316
#define	LEX_NOT	317
#define	LEX_HIGH	318
#define	LEX_LOW	319
#define	LEX_NUMBER	320
#define	LEX_IDENT	321
#define	LEX_INSTR	322
#define	LEX_RN	323
#define	LEX_ARN	324
#define	LEX_STRING	325


extern YYSTYPE yylval;
