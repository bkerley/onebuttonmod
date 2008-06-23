/* umx.h */

/*
CUMX_E_NOMOD - couldn't find module data
CUMX_E_MEM - not enough memory
CUMX_E_NORW - couldn't read or write
*/
#define CUMX_E_NOMOD -2
#define CUMX_E_MEM -3
#define CUMX_E_NORW -4

int strchrb(char *string, char symbol);
int GetDataOffset(FILE *fh, char *buffer);
int CopyData(FILE *fi, FILE *fo, int fsize);

int FileSize(char *fname);

/*
UMXCHK_E_NOREAD couldn't read file
UMXCHK_E_GENERR other unspecified error
*/
#define UMXCHK_E_NOREAD -2 
#define UMXCHK_E_GENERR -1
#define UMXCHK_ISUMX 1
#define UMXCHK_NOTUMX 0


int checkUMX(FILE * file);
/**
	Input: FILE * file - filehandle for reading file
		to be checked for umx-ness
	Output: return int - zero if not umx, one if umx
*/