//
//  player.h
//  OneButtonMod
//
//  Created by Bryce Kerley on 6/9/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <stdio.h>
#include <stdlib.h>
#include "modplayer.h"
#include "trackinfo.h"

@interface Player : NSObject
{
	NSString *filename;
	unsigned long long filesize;
	trackinfo info;
}

- (id)initWithFilename:(NSString*)module_filename;
- (void)startPlay;
- (void)dealloc;
- (NSString *)getTitle;
- (NSString *)getType;
- (NSString *)getComment;
- (NSArray *)getInstruments;
@end
