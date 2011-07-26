//
//  EAGLView.m
//  Untitled
//
//  Created by Student on 05/08/2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "EAGLView.h"
#import "ES1Renderer.h"

#include <EventPollerImplGeneric.h>
#include <EventTypes.h>

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
		// 16-bit frame buffer, from "iPhone Games Projects" p. 112 - doesn't give a speedup :-(
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], 
										kEAGLDrawablePropertyRetainedBacking, 
										kEAGLColorFormatRGB565, // was: kEAGLColorFormatRGBA8, 
										kEAGLDrawablePropertyColorFormat, nil];

        renderer = [[ES1Renderer alloc] init];

        if (!renderer)
		{
			[self release];
			return nil;
		}

        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
		
		[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / 30.0)]; // TODO frequenc makes difference to response ?
		[[UIAccelerometer sharedAccelerometer] setDelegate:self];		
    }

    return self;
}

- (void)drawView:(id)sender
{
    [renderer render];
}

- (void)layoutSubviews
{
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
    }
}

- (void)dealloc
{
    [renderer release];

    [super dealloc];
}

void QueueEvent(Amju::Event* e)
{
	((Amju::EventPollerImplGeneric*)Amju::TheEventPoller::Instance()->GetImpl())->QueueEvent(e);
}

void PopulateCursorEvent(Amju::CursorEvent* ce, int x, int y)
{
	ce->controller = 0;

  // Rotate cursor data to screen orientation - TODO make this run time
#ifdef LANDSCAPE
	ce->x = 1.0f - (float)y / 240.0f;	
	ce->y = 1.0f - (float)x / 160.0f;

#else
	ce->x = (float)x / 160.0f - 1.0f;
	ce->y = 1.0f - (float)y / 240.0f;

#endif
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	int i = 0;
	for (UITouch* touch in touches)
	{
		if (i++ == 0)
		{
			Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
			mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
			mbe->isDown = true;
			QueueEvent(mbe);
		}
		
		CGPoint touchPoint = [touch locationInView:self];
		Amju::CursorEvent* ce = new Amju::CursorEvent;
		PopulateCursorEvent(ce, touchPoint.x, touchPoint.y);
		QueueEvent(ce);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	int i = 0;
	for (UITouch* touch in touches)
	{
		if (i++ == 0)
		{
			Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
			mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
			mbe->isDown = false;
			QueueEvent(mbe);
			return; // ignore other data, it this ok ?
		}
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch* touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self];
		Amju::CursorEvent* ce = new Amju::CursorEvent;
		PopulateCursorEvent(ce, touchPoint.x, touchPoint.y);
		QueueEvent(ce);
	}	
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	const float kFilteringFactor = 0.1;
	static float accel[3] = { 0, 0, 0 };
	
	accel[0] = acceleration.x * kFilteringFactor + accel[0] * (1.0 - kFilteringFactor);
	accel[1] = acceleration.y * kFilteringFactor + accel[1] * (1.0 - kFilteringFactor);
	accel[2] = acceleration.z * kFilteringFactor + accel[2] * (1.0 - kFilteringFactor);	
	
	// accel[0] corresponds to tilting forward/back, i.e. rotation about x-axis when in landscape mode
	// accel[1] corresponds to z-rotation, like twisting a Wii remote
	
	Amju::BalanceBoardEvent* be = new Amju::BalanceBoardEvent;
  
  // TODO This depends on iphone orientation
  be->x = accel[1];
  be->y = accel[0];
  
#ifdef ACC_DEBUG
	std::cout << "ACCEL: X: " << accel[0] << " Y: " << accel[1] << " Z: " << accel[2] << "\n";
#endif
  
	QueueEvent(be);
}


@end
