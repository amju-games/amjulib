//
//  ViewController.h
//  Amjulib
//
//  Created by Jason Colman on 29/07/2013.
//  Copyright (c) 2013 Jason Colman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreMotion/CoreMotion.h>

@interface ViewController : GLKViewController

// j.c. accelerometer - http://www.appcoda.com/ios-maze-game-tutorial-accelerometer/
@property (assign, nonatomic) CMAcceleration acceleration;
@property (strong, nonatomic) CMMotionManager  *motionManager;
@property (strong, nonatomic) NSOperationQueue *queue;

@end
