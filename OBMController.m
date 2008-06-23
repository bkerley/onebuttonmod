#import "OBMController.h"
#import "Player.h"
@implementation OBMController

- (void)awakeFromNib {
	int res = initPlayer();
	instruments = [[NSArray alloc] init];
}

- (IBAction)play:(id)sender
{
	Player* p = [[Player alloc] initWithFilename:filePath];
	[p startPlay];
	[titleField setStringValue:[p getTitle]];
	[typeField setStringValue:[p getType]];
	[instruments release];
	instruments = [p getInstruments];
	[insTable reloadData];
}


- (IBAction)open:(id)sender{
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setAllowsMultipleSelection:false];
	[panel beginSheetForDirectory:nil
													 file:nil
													types: [NSArray arrayWithObjects: @"s3m",
																	@"it", @"umx", @"mod", @"xm", nil]
								 modalForWindow:window
									modalDelegate:self
								 didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)
										contextInfo:NULL];
}

- (void) openPanelDidEnd:(NSOpenPanel *)openPanel
							returnCode:(int)returnCode
						 contextInfo:(void*)x
{
	if (filePath) {
		[filePath release];
	}
	if (returnCode == NSOKButton) {
		filePath = [[[openPanel filenames] objectAtIndex:0] retain];
		[playButton setEnabled:true];
	}
}

- (int)numberOfRowsInTableView:(NSTableView *)aTableView {
	return [instruments count];
}
- (id)tableView:(NSTableView *)aTableView
		objectValueForTableColumn:(NSTableColumn *)aTableColumn
						row:(int) row {
	return [instruments objectAtIndex:row];
}

@end
