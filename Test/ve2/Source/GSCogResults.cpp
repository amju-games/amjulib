#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include <TimePeriod.h>
#include "GSCogResults.h"
#include "CogTestNag.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnCogTestResultsOk()
{
  // TODO does this work from anywhere ??
  static GSCogResults* cr = TheGSCogResults::Instance();
  if (cr->GetPrevState())
  {
    cr->GoBack();
  }
  else
  {
    OnCogTestsAllDone();
  }
}

GSCogResults::GSCogResults()
{
}

void GSCogResults::Update()
{
  GSGui::Update();

}

void GSCogResults::Draw()
{
  GSGui::Draw();

}

void GSCogResults::Draw2d()
{
  m_chart->Draw();

  GSGui::Draw2d();
}

void GSCogResults::SetChart(TestId test)
{
  ChartData* cd = new ChartData;
  m_chart->GetDataDisplay()->SetData(cd);

  typedef std::map<Time, Results> DateMap;
  DateMap datemap;

  // Get historical results from CogTestResults
  Results r = TheCogTestResults::Instance()->GetResultsForTestType(test);
  for (unsigned int i = 0; i < r.size(); i++)
  {
    Result* res = r[i];    
    Time t = res->GetTimestamp();
    t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

    // Put data into buckets for dates
    datemap[t].push_back(res);
  }

  int x = 0;
  for (DateMap::iterator it = datemap.begin(); it != datemap.end(); ++it)
  {
    Time t = it->first;

    Results& r = it->second;
    ChartData::Row row;
    row.first = x;
    x++;

    for (Results::iterator jt = r.begin(); jt != r.end(); ++jt)
    {
      Result* res = *jt;

      float f = ToFloat(res->GetVal()) * 100.0f;
      row.second.push_back((ChartData::YTYPE)f);
    }

    cd->AddRow(row);
    cd->AddXAxisLabel(t.ToStringJustDate());
  }

}

void GSCogResults::OnActive()
{
  GSBase::OnActive();

  LoadCogTestBg();

  m_gui = LoadGui("gui-cogresults.txt");
  Assert(m_gui);

  m_chart = (GuiChart*)CreateChart();
  if (!m_chart->OpenAndLoad("gui-results-chart.txt"))
  {
    Assert(0);
  }
  GuiComposite* comp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  Assert(comp);
  comp->AddChild(m_chart);

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnCogTestResultsOk);

  SetChart(AMJU_COG_TEST_REACTION_TIME);
}

} // namespace
