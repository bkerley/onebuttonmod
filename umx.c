/*
Carp Unreal Music eXtractor v.1.0(console)
 Copyright(C) Carp co., 1999-2000
 Copyright(C) Stepan Karpenko, 1999-2000
 
 Visit www.carp.stealth.ru for updates.
 
 ! Modify it, if it's necessary !
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "umx.h"

#define BUFS 50000



/* Searches for then character from the end of the string */
int strchrb(char *string, char symbol)
{
    int strnglen=strlen(string);
    int i;
	
    for(i=strnglen; i>=0; i--)
    {
        if(*(string+i)==symbol)
			return i; /* Return character position */
    }
	
    return -1; /* Character not found */
}

/* Returns audio data offset */
int GetDataOffset(FILE *fh, char *buffer)
{
    int i;
	
    /* Searching for the Stream Tracker header */
    for(i=0; i<BUFS; i++)
    {
		if(*(buffer+i)=='S' && *(buffer+i+1)=='C' && *(buffer+i+2)=='R'
		   && *(buffer+i+3)=='M')
		{
			return i-44; /* Return start of audio data */
		}
    }
	
    /* Searching for the Module header */
    for(i=0; i<BUFS; i++)
    {
		if(*(buffer+i)=='I' && *(buffer+i+1)=='M' && *(buffer+i+2)=='P'
		   && *(buffer+i+3)=='M')
		{
			return i; /* Return start of audio data */
		}
    }
	
    return -1;   /* No audio data detected */
}

/* Copies data from infile to outfile */
int CopyData(FILE *fi, FILE *fo, int fsize)
{
	char *buf=(char *)malloc(BUFS);
	int seek1, seek2;
		
	if(!buf)
	{
        /* no buffer */
		return CUMX_E_MEM;
	}
	
	if(!fread(buf, 1, BUFS, fi))
	{
		/* couldn't read input */
		free(buf);
		return CUMX_E_NORW;
	}
	seek1=GetDataOffset(fi, buf);
	if(seek1==-1)
	{
		/* no module audio */
		free(buf);
		return CUMX_E_NOMOD; /* Exit function with error code */
	}
	
	seek2=0;
	/* Begin extraction */
	while(1)
	{
		fseek(fi, seek1, 0);
		fseek(fo, seek2, 0);
		if((fsize-seek1)<=BUFS)
		{
			if(!fread(buf, 1, fsize-seek1, fi))
			{
				free(buf);
				return CUMX_E_NORW; /* Exit function with error code */
			}
			if(!fwrite(buf, 1, fsize-seek1, fo))
			{
				free(buf);
				return CUMX_E_NORW; /* Exit function with error code */
			}
			
			free(buf);
			return 0;  /* Exit function */ 
		}
		if(!fread(buf, 1, BUFS, fi))
		{
			free(buf);
			return CUMX_E_NORW; /* Exit function with error code */
		}
		
		if(!fwrite(buf, 1, BUFS, fo))
		{
			free(buf);
			return CUMX_E_NORW; /* Exit function with error code */
		}
		
		seek1+=BUFS;
		seek2+=BUFS;
	}
	/* can't actually get here? */
	
	return 0; /* Exit function */
}
/* END */

/* checks to see if filename points to a valid unreal package */
int checkUMX(FILE * file) {

	/* magic number recoded as four chars
	lifted from http://www.unrealwiki.com/wiki/Package_File_Format */
	char sig[4] = {0xc1, 0x83, 0x2a, 0x9e};
	char cur[4] = {0x00, 0x00, 0x00, 0x00};
	int ne = 0;
	int chk = 0;
	
	rewind(file);
	chk = fread(&cur, 1, 4, file);
	if (chk != 4) {
		if (ferror(file)) {
			/* couldn't read file */
			return UMXCHK_E_NOREAD;
		}
		else if (feof(file)) {
			return 0;
		}
		else {
			/* something strange */
			return UMXCHK_E_GENERR;
		}
	} /* end if (chk != 4) */
	
	ne = memcmp(&sig, &cur, 4);
	
	return (ne?UMXCHK_NOTUMX:UMXCHK_ISUMX);
}