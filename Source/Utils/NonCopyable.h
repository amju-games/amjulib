// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026

#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

namespace Amju
{
class NonCopyable
{
protected:
  NonCopyable() {}
private:
  NonCopyable(const NonCopyable&);
  NonCopyable& operator=(const NonCopyable&);
};
}

#endif
