//
//  player.m
//  OneButtonMod
//
//  Created by Bryce Kerley on 6/9/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "player.h"


@interface Player (PrivateAPI)
- (void) instanceThread;

- (void)playMod;
@end

@implementation Player

- (id) initWithFilename:(NSString*)module_filename {
	filename = [[NSString alloc] initWithString:module_filename];
	initPlayer();
	info.trackname = NULL;
	info.modtype = NULL;
	info.comment = NULL;
	return self;
}

- (void) dealloc {
	[filename release];
	[super dealloc];
}


- (void) startPlay {
	
	[NSThread
		detachNewThreadSelector:@selector(instanceThread:) toTarget:self withObject:nil];
	sleep(1);
}

- (void) instanceThread:(id)someObj{	
	[self retain];
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSFileHandle *data = [NSFileHandle fileHandleForReadingAtPath:filename];
	[data seekToEndOfFile];
	filesize = [data offsetInFile];
	info = playFD([data fileDescriptor], filesize);
	feedPlayer();
	endPlay();
	[data closeFile];
	free(info.insNames);
	[pool release];
	[self release];
}

-(NSString *)getTitle {
	if (info.trackname == NULL) return @"Untitled";
	return [[NSString alloc] initWithCString:info.trackname];
}
-(NSString *)getType {
	if (info.modtype == NULL) return @"Unknown Type";
	return [[NSString alloc] initWithCString:info.modtype];
}
-(NSString *)getComment {
	if (info.comment == NULL) return @"";
	return [[NSString alloc] initWithCString:info.comment];
}
- (NSArray *)getInstruments {
	unsigned long i;
	NSMutableArray *a = [[NSMutableArray alloc] init];
	for (i = 0; i < info.numIns; i++) {
		char* str = info.insNames[i];
		if (str == NULL) {
			 asprintf(&str,"Unnamed instrument #%d",i);
			
			[a addObject:[[NSString alloc] initWithCString:str]];
			free(str);
		}
		else {
			[a addObject:[[NSString alloc] initWithCString:str]];	
		}
	}
	return a;
}
@end
