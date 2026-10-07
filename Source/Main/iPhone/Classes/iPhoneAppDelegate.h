// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#import <UIKit/UIKit.h>

@class EAGLView;

@interface iPhoneAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

