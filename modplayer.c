/*
 *  modplayer.c
 *  zerobutanmod
 *
 *  Created by Bryce Kerley on 8/6/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libmikmod-coreaudio/mikmod.h>


#include "modplayer.h"
#include "umx.h"

MODULE *module;
int playerReady = 0;

int initPlayer() {
	if (playerReady == 1) return 0;
	
	MikMod_RegisterAllDrivers();
	MikMod_RegisterAllLoaders();
	
	md_mode |= DMODE_SOFT_MUSIC;
	
	if(MikMod_Init("")) {
		fprintf(stderr, "Error: %s\n",
						MikMod_strerror(MikMod_errno)
						);
		return -1;
	}
	playerReady = 1;
	return 0;
}

trackinfo playFD(int fd, unsigned long long insize) {
	BOOL handleUMX;
	FILE * temp;
	FILE * in;
	int ret;
	unsigned long i;
	trackinfo info = {
		NULL, NULL, NULL
	};
	
	in = fdopen(fd, "rb");
	
	handleUMX = checkUMX(in);
	
	switch(handleUMX) {
		case UMXCHK_E_GENERR :
			fprintf(stderr,"Warning: A non-error non-eof read problem occurred.\n");
			return info;
		case UMXCHK_E_NOREAD :
			fprintf(stderr,"Error: Couldn't read file for UMX signature checking.\n");
			return info;
		case UMXCHK_ISUMX :
			rewind(in);
			
			/* make a temporary filehandle */
			temp = tmpfile();
			switch (CopyData(in, temp, insize)) {
				case CUMX_E_MEM:
					fprintf(stderr, "Error: Not enough memory for UMX buffer\n");
					fclose(in); fclose(temp);
					return info;
				case CUMX_E_NOMOD:
					fprintf(stderr, "Error: No module data in this Unreal package\n");
					fclose(in); fclose(temp);
					return info;
				case CUMX_E_NORW:
					fprintf(stderr, "Error: Couldn't read from UMX or write to tmpfile\n");
					fclose(in); fclose(temp);
					return info;
			}
				printf("done!\n");
			in = temp;
			break;
		default:
			break;
	}
	
	rewind(in);
	ret = startPlay(in);
	info.comment = module->comment;
	info.modtype = module->modtype;
	info.trackname = module->songname;
	info.numIns = module->numsmp;
	info.insNames = calloc(info.numIns, sizeof(char*));
	for (i = 0; i < info.numIns; i++) {
		info.insNames[i] = module->samples[i].samplename;
	}

	fclose(in);
	return info;
}

int startPlay(FILE* in) {
	module = Player_LoadFP(in,256,4);
	
	usleep(100);
	
	Player_Start(module);
	
	if (NOLOOP == 1) {
		module->wrap = 0;
		module->loop = 0;
	}
	return 0;
}

int endPlay() {
		
	Player_Stop();
	Player_Free(module);
	module = NULL;
	
	MikMod_Reset(NULL);
	return 0;
}

void feedPlayer() {
	while(Player_Active()) {
		usleep(10000);
		MikMod_Update();
	}
}

