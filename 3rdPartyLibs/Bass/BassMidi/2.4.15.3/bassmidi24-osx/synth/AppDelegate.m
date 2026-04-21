/*
 	BASSMIDI synth
 	Copyright (c) 2011-2020 Un4seen Developments Ltd.
*/

#import "AppDelegate.h"
#include "bass.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	BASS_Free();
	BASS_PluginFree(0);
}

@end
