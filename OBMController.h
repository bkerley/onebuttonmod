/* OBMController */

#import <Cocoa/Cocoa.h>

@interface OBMController : NSObject
{
    IBOutlet NSTextField *titleField;
    IBOutlet NSTextField *typeField;
		IBOutlet NSTableView *insTable;
		IBOutlet NSButton *playButton;
		IBOutlet NSWindow *window;
		NSArray *instruments;
		
	NSString *filePath;
}
- (IBAction)play:(id)sender;
- (IBAction)open:(id)sender;

- (int)numberOfRowsInTableView:(NSTableView *)aTableView;
- (id)tableView:(NSTableView *)aTableView
		objectValueForTableColumn:(NSTableColumn *)aTableColumn
						row:(int) row;
@end
