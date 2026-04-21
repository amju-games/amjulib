/*
	BASSMIDI test player
	Copyright (c) 2006-2022 Un4seen Developments Ltd.
*/

#import "ViewController.h"
#include "bass.h"
#include "bassmidi.h"

@implementation ViewController
@synthesize fxSwitch;
@synthesize positionSlider;
@synthesize positionText;
@synthesize infoText;
@synthesize fontinfo;
@synthesize tempoText;
@synthesize cpuText;
@synthesize voicesText;
@synthesize voicesSlider;
@synthesize lyricsText;

HSTREAM chan;        // channel handle
HSOUNDFONT font;    // soundfont
float speed=1;    // tempo adjustment
char lyrics[1000]; // lyrics buffer

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

void CALLBACK LyricSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_MIDI_MARK mark;
	const char *text;
	char *p;
	int lines;
	BASS_MIDI_StreamGetMark(channel,(DWORD)user,data,&mark); // get the lyric/text
	text=mark.text;
	if (text[0]=='@') return; // skip info
	if (text[0]=='\\') { // clear display
		p=lyrics;
		text++;
	} else {
		p=lyrics+strlen(lyrics);
		if (text[0]=='/') { // new line
			*p++='\n';
			text++;
		}
	}
	sprintf(p,"%.*s",(int)(lyrics+sizeof(lyrics)-p-1),text); // add the text to the lyrics buffer
	for (lines=1,p=lyrics;(p=strchr(p,'\n'));lines++,p++) ; // count lines
	if (lines>3) { // remove old lines so that new lines fit in display...
		int a;
		for (a=0,p=lyrics;a<lines-3;a++) p=strchr(p,'\n')+1;
		memmove(lyrics,p,strlen(p)+1);
	}
	dispatch_async(dispatch_get_main_queue(), ^{
		ViewController *view=(ViewController*)[NSApplication sharedApplication].keyWindow.contentViewController;
		view.lyricsText.stringValue=[NSString stringWithCString:lyrics encoding:NSISOLatin1StringEncoding];
	});
}

void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	lyrics[0]=0; // clear lyrics
	dispatch_async(dispatch_get_main_queue(), ^{
		ViewController *view=(ViewController*)[NSApplication sharedApplication].keyWindow.contentViewController;
		view.lyricsText.stringValue=@"";
	});
}

// look for a marker (eg. loop points)
BOOL FindMarker(HSTREAM handle, const char *text, BASS_MIDI_MARK *mark)
{
	int a;
	for (a=0;BASS_MIDI_StreamGetMark(handle,BASS_MIDI_MARK_MARKER,a,mark);a++) {
		if (!strcasecmp(mark->text,text)) return TRUE; // found it
	}
	return FALSE;
}

void CALLBACK LoopSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_MIDI_MARK mark;
	if (FindMarker(channel,"loopstart",&mark)) // found a loop start point
		BASS_ChannelSetPosition(channel,mark.pos,BASS_POS_BYTE|BASS_MIDI_DECAYSEEK); // rewind to it (and let old notes decay)
	else
		BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE|BASS_MIDI_DECAYSEEK); // else rewind to the beginning instead
}

- (IBAction)openFile:(id)sender {
	NSOpenPanel *panel=[NSOpenPanel openPanel];
	[panel setAllowedFileTypes:[NSArray arrayWithObjects:@"mid", @"kar", nil]];
	if ([panel runModal] == NSModalResponseOK) {
		BASS_StreamFree(chan); // free old stream before opening new
		dispatch_async(dispatch_get_main_queue(), ^{
			ViewController *view=(ViewController*)[NSApplication sharedApplication].keyWindow.contentViewController;
			view.lyricsText.stringValue=@"";
		});
		NSString *file=[panel filename];
		if (!(chan=BASS_MIDI_StreamCreateFile(0,[file UTF8String],0,0,BASS_SAMPLE_FLOAT|BASS_SAMPLE_LOOP|BASS_MIDI_DECAYSEEK|(self.fxSwitch.state?0:BASS_MIDI_NOFX),1))) {
			// it ain't a MIDI
			[sender setTitle:@"Open MIDI file..."];
			self.infoText.stringValue=@"";
			positionText.stringValue=@"-";
			[positionSlider setEnabled:false];
			Error(@"Can't play the file");
		} else {
			[sender setTitle:file];
			{ // set the title (track name of first track)
				BASS_MIDI_MARK mark;
				if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_TRACK,0,&mark) && !mark.track)
					self.infoText.stringValue=[NSString stringWithCString:mark.text encoding:NSISOLatin1StringEncoding];
				else
					self.infoText.stringValue=@"";
			}
			// update pos scroller range (using tick length)
			self.positionSlider.maxValue=BASS_ChannelGetLength(chan,BASS_POS_MIDI_TICK)/120;
			[positionSlider setEnabled:true];
			{ // set looping syncs
				BASS_MIDI_MARK mark;
				if (FindMarker(chan,"loopend",&mark)) // found a loop end point
					BASS_ChannelSetSync(chan,BASS_SYNC_POS|BASS_SYNC_MIXTIME,mark.pos,LoopSync,0); // set a sync there
				BASS_ChannelSetSync(chan,BASS_SYNC_END|BASS_SYNC_MIXTIME,0,LoopSync,0); // set one at the end too (eg. in case of seeking past the loop point)
			}
			{ // clear lyrics buffer and set lyrics syncs
				BASS_MIDI_MARK mark;
				lyrics[0]=0;
				if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_LYRIC,0,&mark)) // got lyrics
					BASS_ChannelSetSync(chan,BASS_SYNC_MIDI_MARK,BASS_MIDI_MARK_LYRIC,LyricSync,(void*)BASS_MIDI_MARK_LYRIC);
				else if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_TEXT,20,&mark)) // got text instead (over 20 of them)
					BASS_ChannelSetSync(chan,BASS_SYNC_MIDI_MARK,BASS_MIDI_MARK_TEXT,LyricSync,(void*)BASS_MIDI_MARK_TEXT);
				BASS_ChannelSetSync(chan,BASS_SYNC_END,0,EndSync,0);
			}
			BASS_ChannelSetAttribute(chan,BASS_ATTRIB_MIDI_SPEED,speed); // apply tempo adjustment
			{ // get default soundfont in case of matching soundfont being used
				BASS_MIDI_FONT sf;
				BASS_MIDI_StreamGetFonts(chan,&sf,1);
				font=sf.font;
			}
			BASS_ChannelPlay(chan,FALSE); // start playing
		}
	}
}

- (IBAction)changePosition:(id)sender {
	DWORD p=[sender doubleValue];
	BASS_ChannelSetPosition(chan,p*120,BASS_POS_MIDI_TICK);
	// clear lyrics
	lyrics[0]=0;
	self.lyricsText.stringValue=@"";
}

- (IBAction)openFont:(id)sender {
	NSOpenPanel *panel=[NSOpenPanel openPanel];
	[panel setAllowedFileTypes:[NSArray arrayWithObjects:@"sf2", @"sf2pack", @"sf3", nil]];
	if ([panel runModal] == NSModalResponseOK) {
		NSString *file=[panel filename];
		HSOUNDFONT newfont=BASS_MIDI_FontInit([file UTF8String],0);
		if (newfont) {
			BASS_MIDI_FONT sf;
			sf.font=newfont;
			sf.preset=-1; // use all presets
			sf.bank=0; // use default bank(s)
			BASS_MIDI_StreamSetFonts(0,&sf,1); // set default soundfont
			BASS_MIDI_StreamSetFonts(chan,&sf,1); // set for current stream too
			BASS_MIDI_FontFree(font); // free old soundfont
			font=newfont;
		}
	}
}

- (IBAction)changeFX:(id)sender {
	if ([sender state])
		BASS_ChannelFlags(chan,0,BASS_MIDI_NOFX); // enable FX
	else
		BASS_ChannelFlags(chan,BASS_MIDI_NOFX,BASS_MIDI_NOFX); // disable FX
}

- (IBAction)changeTempo:(id)sender {
	double value=[sender doubleValue];
	speed=(20+value)/20.f; // up to +/- 50% bpm
	BASS_ChannelSetAttribute(chan,BASS_ATTRIB_MIDI_SPEED,speed); // apply tempo adjustment
}

- (IBAction)changeVoices:(id)sender {
	DWORD value=[sender doubleValue];
	BASS_SetConfig(BASS_CONFIG_MIDI_VOICES, value); // set default voice limit
	if (chan) BASS_ChannelSetAttribute(chan,BASS_ATTRIB_MIDI_VOICES,value); // apply to current MIDI file too
}

- (void)TimerProc:(NSTimer*)timer {
	static int updatefont=0;
	float active=0;
	if (chan) {
		DWORD tick=(DWORD)BASS_ChannelGetPosition(chan,BASS_POS_MIDI_TICK); // get position in ticks
		int tempo=BASS_MIDI_StreamGetEvent(chan,0,MIDI_EVENT_TEMPO); // get the file's tempo
		positionText.stringValue=[NSString stringWithFormat:@"%d",tick];
		self.positionSlider.doubleValue=tick/120; // update position
		self.tempoText.stringValue=[NSString stringWithFormat:@"%.1f",speed*60000000.0/tempo]; // display bpm
		BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MIDI_VOICES_ACTIVE,&active); // get active voices
	}
	self.voicesText.stringValue=[NSString stringWithFormat:@"%u / %u",(int)active,BASS_GetConfig(BASS_CONFIG_MIDI_VOICES)]; // display voices
	self.cpuText.stringValue=[NSString stringWithFormat:@"CPU: %d%%",(int)BASS_GetCPU()]; // display CPU usage
	if (++updatefont&1) { // only updating font info once a second
		char text[80]="no soundfont";
		BASS_MIDI_FONTINFO i;
		if (BASS_MIDI_FontGetInfo(font,&i))
			snprintf(text,sizeof(text),"name: %s\nloaded: %d / %d",i.name,i.samload,i.samsize);
		self.fontinfo.stringValue=[NSString stringWithUTF8String:text];
	}
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
	
	// load optional plugins for packed soundfonts (others may be used too)
	BASS_PluginLoad("libbassflac.dylib",0);
	BASS_PluginLoad("libbasswv.dylib",0);
	BASS_PluginLoad("libbassopus.dylib",0);

	self.voicesSlider.doubleValue=BASS_GetConfig(BASS_CONFIG_MIDI_VOICES); // get default voice limit
	[[NSRunLoop currentRunLoop] addTimer:[NSTimer timerWithTimeInterval:0.5 target:self selector:@selector(TimerProc:) userInfo:nil repeats:YES] forMode:NSRunLoopCommonModes];
}

@end
