/*
 *  player.h
 *  zerobutanmod
 *
 *  Created by Bryce Kerley on 8/6/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include "TrackInfo.h"
trackinfo playFD(int fd, unsigned long long insize);
int startPlay(FILE* in);
int endPlay();

/*
NOLOOP is 1 to force wrap and loop bools to zero 
without this 2ND_SKAV.S3M plays forever
*/
#define NOLOOP 1

/* buffer replenishment loop */
void feedPlayer();
