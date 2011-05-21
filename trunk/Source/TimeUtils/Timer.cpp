#include "Timer.h"

#if defined(WIN32) && defined(AMJU_USE_SDL)
#include <SDL.h>
#endif // WIN32 + SDL

#if defined(MACOSX) || defined (IPHONE)
#include <sys/time.h>
#endif // MACOSX

#ifdef GEKKO
#include <gccore.h>

// From http://forum.wiibrew.org/read.php?11,11264
// Posted by AerialX

/****************************************************************************
* tmbinc msec timer
****************************************************************************/
#ifdef HW_RVL
#define TB_CLOCK  60750000 /* WII */
#else
#define TB_CLOCK  40500000
#endif

struct MillisecondTimer
{
	MillisecondTimer()
	{
		Update();
	}

	void Update()
	{
		{u32 u; do {
			asm volatile ("mftbu %0" : "=r" (u));
			asm volatile ("mftb %0" : "=r" (tm.l));
			asm volatile ("mftbu %0" : "=r" (tm.u));
		} while (u != (tm.u)); }
	}

	u32 operator -(MillisecondTimer& timer)
	{
		tb_t* end = &tm;
		tb_t* start = &timer.tm;
		u32 upper;
		u32 lower;
		upper = end->u - start->u;
		if (start->l > end->l)
			upper--;
		lower = end->l - start->l;
		return ((upper * ((unsigned long) 0x80000000 / (TB_CLOCK / 2000))) +
			(lower / (TB_CLOCK / 1000)));
	}

protected:
	struct tb_t
	{
		u32 l;
		u32 u;
	};

	tb_t tm;
};
#endif // GEKKO

namespace Amju
{
static float dt = 0;
static float elapsed = 0;

void Timer::Update()
{
//#if defined(MACOSX) && defined(AMJU_USE_GLUT_TIMER)
//  int millisecs = glutGet(GLUT_ELAPSED_TIME);
//  double d = double(millisecs) / 1000.0;
//  dt = (float)d;
//#endif

#if defined(MACOSX) || defined(IPHONE) 
  timeval tv;
  gettimeofday(&tv, 0);
  static timeval old = tv;
  double diff = tv.tv_sec - old.tv_sec + (tv.tv_usec - old.tv_usec) * 1e-6;
  dt = (float)diff;
  old = tv;

#endif // MACOSX

//#ifdef IPHONE
//  dt = 1.0f / 60.0f; // TODO TEMP TEST
//#endif

#if defined(WIN32) && defined(AMJU_USE_SDL)
  static unsigned int oldt = 0;
  unsigned int t = SDL_GetTicks();
  unsigned int diff = t - oldt;
  oldt = t;
  dt = (float)diff / 1000.0f;
#endif // WIN32 + SDL

#ifdef GEKKO
  static MillisecondTimer oldt;
  static MillisecondTimer t;
  t.Update();
  u32 diff = t - oldt;
  oldt = t;
  dt = (float)diff / 1000.0f;
#endif // GEKKO

  static const float MAX_DT = 0.02f;
  if (dt > MAX_DT)
  {
    dt = MAX_DT;
  }
  elapsed += dt;
}

float Timer::GetDt() const
{
  return dt;
}

float Timer::GetElapsedTime() const
{
  return elapsed;
}
}
