#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <GuiButton.h>
#include <TimePeriod.h>
#include "GSCogResults.h"
#include "CogTestNag.h"
#include <AmjuFinal.h>

namespace Amju
{
static void OnNext()
{
  TheGSCogResults::Instance()->OnNext();
}

static void OnPrev()
{
  TheGSCogResults::Instance()->OnPrev();
}

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
  m_testId = AMJU_COG_TEST_LETTER_CAN;
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
  GSGui::Draw2d();
}

void GSCogResults::SetChart(TestId test)
{
  GuiText* testname = (GuiText*)GetElementByName(m_gui, "test-name");
  testname->SetText(GetTestName(test));

  GuiText* key[3] = 
  {
    (GuiText*)GetElementByName(m_gui, "key1"),
    (GuiText*)GetElementByName(m_gui, "key2"),
    (GuiText*)GetElementByName(m_gui, "key3")
  };

  ChartData* cd = new ChartData;
  GuiChart* chart = (GuiChart*)GetElementByName(m_gui, "results-chart");
  Assert(chart);
  chart->GetDataDisplay()->SetData(cd);

  typedef std::map<Time, Results> DateMap;
  DateMap datemap;

  // Get historical results from CogTestResults
  Results r = TheCogTestResults::Instance()->GetResultsForTestType(test);
  if (r.empty())
  {
    // TODO Hide unused GUI
    return;
  }

  for (unsigned int i = 0; i < r.size(); i++)
  {
    Result* res = r[i];    
    Time t = res->GetTimestamp();
    t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

    // Put data into buckets for dates
    datemap[t].push_back(res);
  }

  // TODO Detect and fix situations where data is missing

  // Look at first date of data
  Results& results = datemap.begin()->second;
  int num = results.size();
  if (num != 2 && num != 3)
  {
    // Unexpected number of results :-(
  }
  // Set key text
  for (int i = 0; i < num; i++)
  {
    key[i]->SetText(results[i]->GetKey());
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

  // TODO Set focus element, cancel element, command handlers
  GuiButton* ok = (GuiButton*)GetElementByName(m_gui, "ok-button");
  ok->SetCommand(OnCogTestResultsOk);

  GuiButton* next = (GuiButton*)GetElementByName(m_gui, "next-button");
  next->SetCommand(Amju::OnNext);

  GuiButton* prev = (GuiButton*)GetElementByName(m_gui, "prev-button");
  prev->SetCommand(Amju::OnPrev);

  SetChart((TestId)m_testId);
}

void GSCogResults::OnNext()
{
  m_testId++;
  if (m_testId >= AMJU_COG_TEST_MAX)
  {
    m_testId = AMJU_COG_TEST_LETTER_CAN;
  }
  SetChart((TestId)m_testId);
}

void GSCogResults::OnPrev()
{
  m_testId--;
  if (m_testId < AMJU_COG_TEST_LETTER_CAN)
  {
    m_testId = AMJU_COG_TEST_MAX - 1;
  }
  SetChart((TestId)m_testId);
}
} // namespace
