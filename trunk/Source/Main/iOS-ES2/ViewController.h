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
#import <CoreLocation/CoreLocation.h> // for compass

@interface ViewController : GLKViewController <CLLocationManagerDelegate>

// j.c. accelerometer - http://www.appcoda.com/ios-maze-game-tutorial-accelerometer/
@property (assign, nonatomic) CMAcceleration acceleration;
@property (strong, nonatomic) CMMotionManager  *motionManager;
@property (strong, nonatomic) NSOperationQueue *queue;

// j.c. compass - http://www.devfright.com/how-to-use-the-iphone-digital-compass-in-your-app/
@property (strong, nonatomic) CLLocationManager *locationManager;


@end
