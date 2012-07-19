//
//  My_GameAppDelegate.m
//  My Game
//
//  Created by Jason Colman on 10/02/2012.
//  Copyright 2012 Amju Games. All rights reserved.
//

#import "My_GameAppDelegate.h"
#include "Updater.h"
#include <iostream> // TEMP
#include <string>

using namespace Amju;

std::string reportText = "Hmmm...";

void Report(const char* s)
{
	std::cout << s; // TODO
	reportText += s;
	// TODO Force redisplay
	// Like this:  -- need pointer to window or AppDelegate 
//	[window setViewNeedsDisplay:YES]; 
}

@implementation My_GameAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 

	// TODO Set ptr to this or window
	
	Updater* u = new Updater(Report);
	u->Start();
	// TODO Call OnFinished function so we can quit etc	
}

@end
