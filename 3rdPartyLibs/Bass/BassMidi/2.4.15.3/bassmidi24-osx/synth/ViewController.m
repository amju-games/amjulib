/*
 	BASSMIDI synth
	Copyright (c) 2011-2022 Un4seen Developments Ltd.
*/

#import "ViewController.h"
#include "bass.h"
#include "bassmidi.h"

@implementation ViewController
@synthesize inputSelector;
@synthesize activityIndicator;
@synthesize octaveSelector;
@synthesize velocitySelector;
@synthesize drumSwitch;
@synthesize presetSelector;

DWORD input;		// MIDI input device
HSTREAM stream;		// output stream
HSOUNDFONT font;	// soundfont
DWORD preset;		// current preset
BOOL drums;			// drums enabled?
BOOL preload;		// preload samples?
BOOL chans16;		// 16 MIDI channels?

NSTimer *timer;

const DWORD fxtype[5]={BASS_FX_DX8_REVERB,BASS_FX_DX8_ECHO,BASS_FX_DX8_CHORUS,BASS_FX_DX8_FLANGER,BASS_FX_DX8_DISTORTION};
HFX fx[5];        	// effect handles

#define KEYS 20
const CGKeyCode keys[KEYS]={
	'Q','2','W','3','E','R','5','T','6','Y','7','U',
	'I','9','O','0','P','[','=',']'
};

// display error messages
void Error(NSString *es)
{
	es = [es stringByAppendingFormat:@"\n(error code: %d)",BASS_ErrorGetCode()];
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			NSAlert *alert=[[NSAlert alloc] init];
			[alert setMessageText:es];
			[alert runModal];
		});
	} else {
		NSAlert *alert=[[NSAlert alloc] init];
		[alert setMessageText:es];
		[alert runModal];
	}
}

// MIDI input function
void CALLBACK MidiInProc(DWORD handle, double time, const BYTE *buffer, DWORD length, void *user)
{
	if (chans16) // using 16 channels
		BASS_MIDI_StreamEvents(stream,BASS_MIDI_EVENTS_RAW,buffer,length); // send MIDI data to the MIDI stream
	else
		BASS_MIDI_StreamEvents(stream,(BASS_MIDI_EVENTS_RAW+17)|BASS_MIDI_EVENTS_SYNC,buffer,length); // send MIDI data to channel 17 in the MIDI stream
	dispatch_async(dispatch_get_main_queue(), ^{
		ViewController *view=(__bridge ViewController*)user;
		if (timer) [timer invalidate];
		[view.activityIndicator setBackgroundColor:NSColor.greenColor];
		timer=[NSTimer scheduledTimerWithTimeInterval:0.1 target:view selector:@selector(activityOver:) userInfo:nil repeats:NO];
	});
}

// program/preset event sync function
void CALLBACK ProgramEventSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	preset=LOWORD(data);
	dispatch_async(dispatch_get_main_queue(), ^{
		ViewController *view=(__bridge ViewController*)user;
		[view.presetSelector selectItemAtIndex:preset];
	});
	BASS_MIDI_FontCompact(0); // unload unused samples
}

- (void)updatePresetList
{
	int a;
	[presetSelector removeAllItems];
	for (a=0;a<128;a++) {
		const char *name=BASS_MIDI_FontGetPreset(font,a,drums?128:0); // get preset name
		[presetSelector addItemWithTitle:[NSString stringWithFormat:@"%03d: %s",a,name?name:""]];
	}
	[presetSelector selectItemAtIndex:preset];
}

- (void)activityOver:(NSTimer*)timer {
	[activityIndicator setBackgroundColor:NSColor.whiteColor];
}

- (IBAction)changeInput:(id)sender {
	int sel=(int)[sender indexOfSelectedItem];
	if (sel!=input) {
		BASS_MIDI_InFree(input); // free current input device
		input=sel; // set new input device selection
		if (BASS_MIDI_InInit(input,MidiInProc,(__bridge void*)self)) // successfully initialized...
			BASS_MIDI_InStart(input); // start it
		else
			Error(@"Can't initialize MIDI device");
	}
}
- (IBAction)changeInputChannels:(id)sender {
	chans16=[sender state];
}
- (IBAction)reset:(id)sender {
	BASS_MIDI_StreamEvent(stream,0,MIDI_EVENT_SYSTEM,MIDI_SYSTEM_GS); // send system reset event
	if (drums) BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_DRUMS,drums); // send drum switch event
	BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_PROGRAM,preset); // send program/preset event
}

- (IBAction)changeOctave:(id)sender {
	BASS_MIDI_StreamEvent(stream, 16, MIDI_EVENT_NOTESOFF, 0);
}

- (IBAction)openFont:(id)sender {
	NSOpenPanel *panel=[NSOpenPanel openPanel];
	[panel setAllowedFileTypes:[NSArray arrayWithObjects:@"sf2", @"sf2pack", @".sf3", @".sfz", nil]];
	if ([panel runModal] == NSModalResponseOK) {
		NSString *file=[panel filename];
		HSOUNDFONT newfont=BASS_MIDI_FontInit([file UTF8String],0);
		if (newfont) {
			BASS_MIDI_FONT sf;
			sf.font=newfont;
			sf.preset=-1; // use all presets
			sf.bank=0; // use default bank(s)
			BASS_MIDI_StreamSetFonts(0,&sf,1); // set default soundfont
			BASS_MIDI_StreamSetFonts(stream,&sf,1); // apply to current stream too
			BASS_MIDI_FontFree(font); // free old soundfont
			font=newfont;
			{
				BASS_MIDI_FONTINFO i;
				BASS_MIDI_FontGetInfo(font,&i);
				[sender setTitle:i.name?[NSString stringWithCString:i.name encoding:NSISOLatin1StringEncoding]:[file lastPathComponent]];
				if (i.presets==1) { // only 1 preset, auto-select it...
					DWORD p;
					BASS_MIDI_FontGetPresets(font,&p);
					drums=(HIWORD(p)==128); // bank 128 = drums
					preset=LOWORD(p);
					[drumSwitch setState:false];
					BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_DRUMS,drums); // send drum switch event
					BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_PROGRAM,preset); // send program/preset event
					[presetSelector setEnabled:false];
					[drumSwitch setEnabled:false];
				} else {
					[presetSelector setEnabled:true];
					[drumSwitch setEnabled:true];
				}
			}
			[self updatePresetList];
			if (preload) BASS_MIDI_FontLoadEx(font, preset, drums ? 128 : 0, 50, BASS_MIDI_FONTLOAD_NOWAIT | BASS_MIDI_FONTLOAD_TIME); // preload 50ms of current preset
		}
	}
}
- (IBAction)changePreset:(id)sender {
	preset=(int)[sender indexOfSelectedItem]; // get the selection
	BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_PROGRAM,preset); // send program/preset event
	BASS_MIDI_FontCompact(0); // unload unused samples
	if (preload) BASS_MIDI_FontLoadEx(font, preset, drums ? 128 : 0, 50, BASS_MIDI_FONTLOAD_NOWAIT | BASS_MIDI_FONTLOAD_TIME); // preload 50ms of current preset
}
- (IBAction)changeDrums:(id)sender {
	drums=[sender state];
	BASS_MIDI_StreamEvent(stream,16,MIDI_EVENT_DRUMS,drums); // send drum switch event
	BASS_MIDI_StreamEvents(stream,BASS_MIDI_EVENTS_FLUSH,0,0); // process pending events before GetEvent
	preset=BASS_MIDI_StreamGetEvent(stream,16,MIDI_EVENT_PROGRAM); // preset is reset in drum switch
	[self updatePresetList];
	BASS_MIDI_FontCompact(0); // unload unused samples
	if (preload) BASS_MIDI_FontLoadEx(font, preset, drums ? 128 : 0, 50, BASS_MIDI_FONTLOAD_NOWAIT | BASS_MIDI_FONTLOAD_TIME); // preload 50ms of current preset
}
- (IBAction)changePreload:(id)sender {
	preload=[sender state];
	if (preload) BASS_MIDI_FontLoadEx(font, preset, drums ? 128 : 0, 50, BASS_MIDI_FONTLOAD_NOWAIT | BASS_MIDI_FONTLOAD_TIME); // preload 50ms of current preset
}
- (IBAction)changeInterpolation:(id)sender {
	BASS_ChannelSetAttribute(stream, BASS_ATTRIB_MIDI_SRC, [sender tag]);
}
- (IBAction)changeReverb:(id)sender {
	// send GS reverb type macro
	int n=(int)[sender indexOfSelectedItem];
	BASS_MIDI_StreamEvent(stream, 0, MIDI_EVENT_REVERB_MACRO, n ? 0x8000 + n - 1 : 0);
}
- (IBAction)changeReverbLevel:(id)sender {
	// reverb level
	DWORD value=[sender doubleValue];
	BASS_MIDI_StreamEvent(stream, 16, MIDI_EVENT_REVERB, value);
}
- (IBAction)changeChorus:(id)sender {
	// send GS chorus type macro
	int n=(int)[sender indexOfSelectedItem];
	BASS_MIDI_StreamEvent(stream, 0, MIDI_EVENT_CHORUS_MACRO, n ? 0x8000 + n - 1 : 0);
}
- (IBAction)changeChorusLevel:(id)sender {
	// chorus level
	DWORD value=[sender doubleValue];
	BASS_MIDI_StreamEvent(stream, 16, MIDI_EVENT_CHORUS, value);
}
- (IBAction)changeEffects:(id)sender {
	// toggle DX8 effects
	int n=(int)[sender tag];
	if (fx[n]) {
		BASS_ChannelRemoveFX(stream,fx[n]);
		fx[n]=0;
	} else
		fx[n]=BASS_ChannelSetFX(stream,fxtype[n],n);
}

- (void)TimerProc:(NSTimer*)timer
{
	// display loaded sample data
	DWORD loaded = 0;
	BASS_MIDI_FONTINFO i;
	if (BASS_MIDI_FontGetInfo(font, &i))
		loaded = (i.samload + 1023) / 1024;
	self.loadedText.stringValue=[NSString stringWithFormat:@"Loaded: %u KB", loaded];
}

- (void)viewDidLoad {
	[super viewDidLoad];
	
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		Error(@"An incorrect version of BASS was loaded");
		exit(0);
	}
	
	// initialize default output device
	if (!BASS_Init(-1,44100,0,NULL,NULL)) {
		Error(@"Can't initialize device");
		exit(0);
	}
	
	stream=BASS_MIDI_StreamCreate(17,BASS_MIDI_ASYNC|BASS_SAMPLE_FLOAT,1); // create the MIDI stream with async processing and 16 MIDI channels for device input + 1 for keyboard input
	BASS_ChannelSetAttribute(stream,BASS_ATTRIB_BUFFER,0); // no buffering for minimum latency
	BASS_ChannelSetSync(stream,BASS_SYNC_MIDI_EVENT|BASS_SYNC_MIXTIME,MIDI_EVENT_PROGRAM,ProgramEventSync,(__bridge void*)self); // catch program/preset changes
	BASS_MIDI_StreamEvent(stream,0,MIDI_EVENT_SYSTEM,MIDI_SYSTEM_GS); // send GS system reset event
	BASS_ChannelPlay(stream,0); // start it
	{ // enumerate available input devices
		BASS_MIDI_DEVICEINFO di;
		int dev;
		for (dev=0;BASS_MIDI_InGetDeviceInfo(dev,&di);dev++) {
			[inputSelector addItemWithTitle:[NSString stringWithUTF8String:di.name]];
		}
		if (dev) { // got sone, try to initialize one
			int a;
			for (a=0;a<dev;a++) {
				if (BASS_MIDI_InInit(a,MidiInProc,(__bridge void*)self)) { // succeeded, start it
					input=a;
					BASS_MIDI_InStart(input);
					[inputSelector selectItemAtIndex:input];
					break;
				}
			}
			if (a==dev) Error(@"Can't initialize MIDI device");
		} else {
			[inputSelector addItemWithTitle:@"no devices"];
			[inputSelector setEnabled:false];
		}
	}
	[self updatePresetList];
	{
		int a;
		for (a=0;a<10;a++)
			[octaveSelector addItemWithTitle:[NSString stringWithFormat:@"%d",a*12]];
		[octaveSelector selectItemAtIndex:4];
		for (a=1;a<128;a++)
			[velocitySelector addItemWithTitle:[NSString stringWithFormat:@"%d",a]];
		[velocitySelector selectItemAtIndex:109];
	}
	
	// load optional plugins for packed soundfonts (others may be used too)
	BASS_PluginLoad("libbassflac.dylib",0);
	BASS_PluginLoad("libbasswv.dylib",0);
	BASS_PluginLoad("libbassopus.dylib", 0);
	
	[[NSRunLoop currentRunLoop] addTimer:[NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(TimerProc:) userInfo:nil repeats:YES] forMode:NSRunLoopCommonModes];
	
	[NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown|NSEventMaskKeyUp handler:^NSEvent *(NSEvent *event) {
		if ([event modifierFlags] & NSEventModifierFlagCommand) return event;
		if (![event isARepeat]) {
			int ch=toupper([[event characters] characterAtIndex:0]);
			int key;
			for (key = 0; key < KEYS; key++) {
				if (ch==keys[key]) {
					// send note on/off event
					BASS_MIDI_StreamEvent(stream, 16, MIDI_EVENT_NOTE, MAKEWORD([self.octaveSelector indexOfSelectedItem] * 12 + key, [event type] == NSEventTypeKeyDown ? 1 + [self.velocitySelector indexOfSelectedItem] : 0));
					break;
				}
			}
		}
		return nil; // disable the beeps!
	}];
}

@end
