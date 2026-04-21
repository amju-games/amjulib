/*
	BASSMIDI test player
	Copyright (c) 2006-2020 Un4seen Developments Ltd.
*/

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController
@property (weak) IBOutlet NSButton *fxSwitch;
@property (weak) IBOutlet NSSlider *positionSlider;
@property (weak) IBOutlet NSTextField *infoText;
@property (weak) IBOutlet NSTextField *fontinfo;
@property (weak) IBOutlet NSTextField *tempoText;
@property (weak) IBOutlet NSTextField *cpuText;
@property (weak) IBOutlet NSTextField *voicesText;
@property (weak) IBOutlet NSTextField *lyricsText;
@property (weak) IBOutlet NSSlider *voicesSlider;
@property (weak) IBOutlet NSTextField *positionText;

@end

