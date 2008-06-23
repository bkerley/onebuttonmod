//
//  TrackInfo.h
//  OneButtonMod
//
//  Created by Bryce Kerley on 12/19/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#ifndef _TRACKINFO_H_
#define _TRACKINFO_H_

typedef struct TrackInfo {
	char *trackname;
	char *modtype;
	char *comment;
	unsigned long numIns;
	char **insNames;
} trackinfo;

#endif