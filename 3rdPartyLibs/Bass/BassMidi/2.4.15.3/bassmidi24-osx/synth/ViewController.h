/*
	BASSMIDI synth
	Copyright (c) 2011-2020 Un4seen Developments Ltd.
*/

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController
@property (weak) IBOutlet NSPopUpButton *octaveSelector;
@property (weak) IBOutlet NSPopUpButton *velocitySelector;
@property (weak) IBOutlet NSPopUpButton *presetSelector;
@property (weak) IBOutlet NSButton *drumSwitch;
@property (weak) IBOutlet NSPopUpButton *inputSelector;
@property (weak) IBOutlet NSTextField *activityIndicator;
@property (weak) IBOutlet NSTextField *loadedText;

@end
