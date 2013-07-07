#ifndef GS_COGRESULTS_H_INCLUDED
#define GS_COGRESULTS_H_INCLUDED

#include <Singleton.h>
#include <GuiChart.h>
#include "GSGui.h"
#include "CogTestResults.h"

namespace Amju 
{
class GSCogResults : public GSGui
{
  GSCogResults();
  friend class Singleton<GSCogResults>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

protected:
  void SetChart(TestId);

protected:
  RCPtr<GuiChart> m_chart;
};
typedef Singleton<GSCogResults> TheGSCogResults;
} // namespace
#endif
