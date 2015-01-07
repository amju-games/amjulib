//
//  ViewController.mm
//  Amjulib
//
//  Created by Jason Colman on 29/07/2013.
//  Copyright (c) 2013 Jason Colman. All rights reserved.
//

#import "ViewController.h"

#include <AmjuGL.h>
#include <Screen.h>
#include <AmjuGL-OpenGLES.2.h>
#include <EventPoller.h>
#include <Game.h>
#include <DegRad.h>
#include <StartUp.h>

#define AMJU_USE_GYRO

#ifdef AMJU_USE_GYRO
// http://stackoverflow.com/questions/3245733/apple-gyroscope-sample-code
// https://developer.apple.com/library/ios/documentation/CoreMotion/Reference/CMAttitude_Class/index.html#//apple_ref/occ/instp/CMAttitude/roll
static CMMotionManager *motionManager = nullptr;
static CMAttitude *referenceAttitude = nullptr;
#endif // AMJU_USE_GYRO

#ifdef AMJU_USE_ACCELEROMETER
// Accelerom poll freq - j.c. - http://www.appcoda.com/ios-maze-game-tutorial-accelerometer/
// If frequency is too high, frame rate seems to get choppy.
// TODO Make this a setting.
#define kUpdateInterval (1.0f / 10.0f)
#endif // AMJU_USE_ACCELEROMETER

@interface ViewController () {
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [_effect release];
    [super dealloc];
}

#ifdef AMJU_USE_GYRO
-(void) enableMotion{
    CMDeviceMotion *deviceMotion = motionManager.deviceMotion;
    CMAttitude *attitude = deviceMotion.attitude;
    referenceAttitude = [attitude retain];
    [motionManager startDeviceMotionUpdates];
}
#endif // AMJU_USE_GYRO

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
#ifdef AMJU_USE_GYRO
    motionManager = [[CMMotionManager alloc] init];
    referenceAttitude = nil;
    [self enableMotion];
#endif // AMJU_USE_GYRO
  
#ifdef AMJU_USE_ACCELEROMETER

    // j.c. accelerometer - http://www.appcoda.com/ios-maze-game-tutorial-accelerometer/
    self.motionManager = [[CMMotionManager alloc]  init];
    self.queue         = [[NSOperationQueue alloc] init];
  
    self.motionManager.accelerometerUpdateInterval = kUpdateInterval;

    [self.motionManager startAccelerometerUpdatesToQueue:self.queue withHandler:
     ^(CMAccelerometerData *accelerometerData, NSError *error) {
       [(id) self setAcceleration:accelerometerData.acceleration];
       [self performSelectorOnMainThread:@selector(update) withObject:nil waitUntilDone:NO];
     }];
#endif // AMJU_USE_ACCELEROMETER
  
#ifdef AMJU_USE_COMPASS
    
    // j.c. compass - http://www.devfright.com/how-to-use-the-iphone-digital-compass-in-your-app/
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    [self.locationManager startUpdatingHeading];
    // Not reqd here: this starts up getting actual location
    //  [self.locationManager startUpdatingLocation];
#endif // AMJU_USE_COMPASS
}

-(void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)newHeading
{
  Amju::RotationEvent* r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_X; // TODO Sort out - this should be Y axis
  r->degs = DegToRad(newHeading.magneticHeading) * -1.0f; 
  QueueEvent(r);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
  
    Amju::TheEventPoller::Instance()->SetImpl(new Amju::EventPollerImplGeneric);
    
    Amju::AmjuGL::SetImpl(new Amju::AmjuGLOpenGLES2);
    
    std::cout << "Set AmjuGL impl...\n";
    
    Amju::StartUpBeforeCreateWindow();
    Amju::AmjuGL::Init();
    Amju::StartUpAfterCreateWindow();
  
    // TODO TEMP TEST - landscape
////    Amju::Screen::SetSize(self.view.bounds.size.height, self.view.bounds.size.width);
////    Amju::TheGame::Instance()->Update();  // must Update once before Draw -- why
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

void QueueEvent(Amju::Event* e)
{
  Amju::TheEventPoller::Instance()->GetImpl()->QueueEvent(e);
}

- (void)update
{
#ifdef AMJU_USE_ACCELEROMETER

  // j.c. send accelerom events but TODO only if changed

  const float kFilteringFactor = 0.1;
  static float accel[3] = { 0, 0, 0 };
   
  accel[0] = self.acceleration.x * kFilteringFactor + accel[0] * (1.0 - kFilteringFactor);
  accel[1] = self.acceleration.y * kFilteringFactor + accel[1] * (1.0 - kFilteringFactor);
  accel[2] = self.acceleration.z * kFilteringFactor + accel[2] * (1.0 - kFilteringFactor);
   
  // accel[0] corresponds to tilting forward/back, i.e. rotation about x-axis when in landscape mode
  // accel[1] corresponds to z-rotation, like twisting a Wii remote
  
   // This is for LANDSCAPE mode
//   Amju::BalanceBoardEvent* be = new Amju::BalanceBoardEvent(accel[1], accel[0]);

  // This is for PORTRAIT mode
  Amju::BalanceBoardEvent* be = new Amju::BalanceBoardEvent(accel[0], accel[2]);

   // TODO This depends on iphone orientation
//   be->x = accel[1];
//   be->y = accel[0];
  
#ifdef ACCELEROM_DEBUG
  std::cout << "ACCEL: X: " << accel[0] << " Y: " << accel[1] << " Z: " << accel[2] << "\n";
#endif
   
  QueueEvent(be);
  
  // Also queue rotation events. The axes depend on the phone orientation.
  Amju::RotationEvent* r = 0;
  // Y axis is taken care of by compass
//  r = new Amju::RotationEvent;
//  r->controller = 0;
//  r->axis = Amju::AMJU_AXIS_X;
//  r->degs = accel[0];
//  QueueEvent(r);
  r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_Y;
  r->degs = accel[1];
  QueueEvent(r);
  r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_Z;
  r->degs = accel[2];
  QueueEvent(r);

#endif // AMJU_USE_ACCELEROMETER
    
#ifdef AMJU_USE_GYRO
  CMDeviceMotion *deviceMotion = motionManager.deviceMotion;
  CMAttitude *attitude = deviceMotion.attitude;
    
  // With this turned off, the direction you start off in is looking in the default
  //  camera dir, so I think it's better
//  if (referenceAttitude != nil) [attitude multiplyByInverseOfAttitude:referenceAttitude];

  // TODO Quaternion would be preferable
  // Queue rotation events. The axes depend on the phone orientation.
  Amju::RotationEvent* r = 0;
    
  r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_X;
  r->degs = RadToDeg(attitude.roll) - 90.0f;
  QueueEvent(r);
    
  r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_Y;
  r->degs = RadToDeg(attitude.yaw);
  QueueEvent(r);
    
  r = new Amju::RotationEvent;
  r->controller = 0;
  r->axis = Amju::AMJU_AXIS_Z;
  r->degs = RadToDeg(attitude.pitch);
  QueueEvent(r);
    
#endif

  Amju::TheGame::Instance()->Update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
  static int count = 0;
  count++;
  
  float w = rect.size.width;
  float h = rect.size.height;
  float s = self.view.contentScaleFactor;

  Amju::Screen::SetSize(w * s, h * s);
  Amju::AmjuGL::Viewport(0, 0, w * s, h * s);
  
  if (count > 1)
  {
    Amju::TheGame::Instance()->Draw();
  }
  Amju::AmjuGL::Flip();
}

void PopulateMBEvent(Amju::MouseButtonEvent* mbe, int x, int y)
{
  float scrX2 = float(Amju::Screen::X() / 2);
  float scrY2 = float(Amju::Screen::Y() / 2);

  mbe->x = (float)x / scrX2 - 1.0f;
  mbe->y = 1.0f - (float)y / scrY2;
}

void PopulateCursorEvent(Amju::CursorEvent* ce, int x, int y)
{
  Amju::MouseButtonEvent mbe;
  PopulateMBEvent(&mbe, x, y);
  ce->controller = 0;
  ce->x = mbe.x;
  ce->y = mbe.y;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;
  
	int i = 0;
	for (UITouch* touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		if (i++ == 0)
		{
			Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
			mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
			mbe->isDown = true;
			PopulateMBEvent(mbe, touchPoint.x * s, touchPoint.y * s);
			QueueEvent(mbe);
		}
		
		Amju::CursorEvent* ce = new Amju::CursorEvent;
		PopulateCursorEvent(ce, touchPoint.x * s, touchPoint.y * s);
		QueueEvent(ce);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;
  
	int i = 0;
	for (UITouch* touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		if (i++ == 0)
		{
			Amju::MouseButtonEvent* mbe = new Amju::MouseButtonEvent;
			mbe->button = Amju::AMJU_BUTTON_MOUSE_LEFT;
			mbe->isDown = false;
			PopulateMBEvent(mbe, touchPoint.x * s, touchPoint.y * s);
			QueueEvent(mbe);
			return; // ignore other data, it this ok ?
		}
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  float s = self.view.contentScaleFactor;
  
	for (UITouch* touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		Amju::CursorEvent* ce = new Amju::CursorEvent;
		PopulateCursorEvent(ce, touchPoint.x * s, touchPoint.y * s);
		QueueEvent(ce);
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  // TODO Don't know what to do here
}

@end
