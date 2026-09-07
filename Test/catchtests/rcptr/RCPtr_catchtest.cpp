#include "../catch.hpp"
#include "../../../Source/Utils/RCPtr.h"

class Widget : public Amju::RefCounted {};

// TODO Test cases for assignment - what is the correct behaviour with
//  rc and weak ptrs??

TEST_CASE("Constructing RCPtrs and WeakPtrs", "[RCPtr]")
{
  using namespace Amju;

  auto pWidget = new Widget;
  REQUIRE(pWidget->GetRefCount() == 0);
  REQUIRE(pWidget->GetWeakCount() == 0);

  {
    RCPtr<Widget> rcWidget = pWidget; // rc from raw pointer
    REQUIRE(pWidget->GetRefCount() == 1);
    REQUIRE(pWidget->GetWeakCount() == 0);

    RCPtr<Widget> rcWidget2 = rcWidget; // rc from rc
    REQUIRE(pWidget->GetRefCount() == 2);
    REQUIRE(pWidget->GetWeakCount() == 0);

    WeakPtr<Widget> weakWidget = pWidget; // weak from raw pointer 
    REQUIRE(pWidget->GetRefCount() == 2);
    REQUIRE(pWidget->GetWeakCount() == 1);

    {
      WeakPtr<Widget> weakWidget2 = rcWidget; // weak from rc
      REQUIRE(pWidget->GetRefCount() == 2);
      REQUIRE(pWidget->GetWeakCount() == 2);
  
      {
        WeakPtr<Widget> weakWidget3 = weakWidget; // weak from another weak
        REQUIRE(pWidget->GetRefCount() == 2);
        REQUIRE(pWidget->GetWeakCount() == 3);
  
        RCPtr<Widget> rcWidget3 = weakWidget; // rc from weak
        REQUIRE(pWidget->GetRefCount() == 3);
        REQUIRE(pWidget->GetWeakCount() == 3);
      }
  
      REQUIRE(pWidget->GetRefCount() == 2);
      REQUIRE(pWidget->GetWeakCount() == 2);
    }
    
    REQUIRE(pWidget->GetRefCount() == 2);
    REQUIRE(pWidget->GetWeakCount() == 1);
  }

  // Oops, widget is now deleted! We can't check the ref counts on
  //  a deleted object. How to test this?  
//  REQUIRE(pWidget->GetRefCount() == 0);
//  REQUIRE(pWidget->GetWeakCount() == 0);
}

TEST_CASE("Reset RCPtrs and WeakPtrs", "[RCPtr]")
{
  using namespace Amju;
  auto pWidget = new Widget;
  RCPtr<Widget> rcWidget = pWidget; // rc from raw pointer
  RCPtr<Widget> rcWidget2 = rcWidget; // rc from rc
  WeakPtr<Widget> weakWidget = pWidget; // weak from raw pointer 
  REQUIRE(pWidget->GetRefCount() == 2);
  REQUIRE(pWidget->GetWeakCount() == 1);
  rcWidget.Reset();
  REQUIRE(pWidget->GetRefCount() == 1);
  REQUIRE(pWidget->GetWeakCount() == 1);
  weakWidget.Reset();  
  REQUIRE(pWidget->GetRefCount() == 1);
  REQUIRE(pWidget->GetWeakCount() == 0);
}

TEST_CASE("WeakPtrs still outstanding", "[RCPtr]")
{
  // Skip this test usually; it will crash, unavoidably.
  return;

  using namespace Amju;
  auto pWidget = new Widget;
  RCPtr<Widget> rcWidget = pWidget; // rc from raw pointer
  WeakPtr<Widget> weakWidget = pWidget; // weak from raw pointer 
  REQUIRE(pWidget->GetRefCount() == 1);
  REQUIRE(pWidget->GetWeakCount() == 1);

  // rcWidget.Reset() should assert because
  //  there is an outstanding weak ptr on the widget.
  // We have hijacked Assert to increment a counter, not terminate.
  ResetAssertCounter();
  REQUIRE(GetAssertCounter() == 0);
  // Asserts because weak counter is not zero.
  rcWidget.Reset();
  REQUIRE(GetAssertCounter() > 0);

  // Argh, we crash here because we try to decrement the weak count 
  //  on the deleted widget when weakWidget is destroyed.
}

