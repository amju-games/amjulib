//
//  iPhoneAppDelegate.h
//  iPhone
//
//  Created by Qantm on 12/07/2010.
//  Copyright Qantm 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface iPhoneAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

