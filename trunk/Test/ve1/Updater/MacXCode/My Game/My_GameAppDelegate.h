//
//  My_GameAppDelegate.h
//  My Game
//
//  Created by Jason Colman on 10/02/2012.
//  Copyright 2012 Amju Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface My_GameAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
