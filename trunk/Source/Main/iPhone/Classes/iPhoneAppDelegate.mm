//
//  iPhoneAppDelegate.m
//  iPhone
//
//  Created by Qantm on 12/07/2010.
//  Copyright Qantm 2010. All rights reserved.
//

#import "iPhoneAppDelegate.h"
#import "EAGLView.h"
#include "GSPaused.h"
#include "GSMain.h"
#include <Game.h>

@implementation iPhoneAppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [glView startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
  // TODO This is game-specific -- call a game-specific function
 
	// Pause if we are currently playing
	if (Amju::TheGame::Instance()->GetState() == Amju::TheGSMain::Instance())
	{
	  Amju::TheGame::Instance()->SetCurrentState(Amju::TheGSPaused::Instance());
	  // Force change - doesn't work :-( 
      Amju::TheGame::Instance()->Update();
	}
}


- (void)dealloc
{
    [window release];
    [glView release];

    [super dealloc];
}

@end
