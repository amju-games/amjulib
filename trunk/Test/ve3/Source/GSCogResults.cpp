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
  m_testId = AMJU_COG_TEST_REACTION_TIME;
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
  static const int MAX_KEY = 5; // num items in Key in GUI

std::cout << "Setting chart up for " << GetTestName(test) << "\n";

  GuiText* testname = (GuiText*)GetElementByName(m_gui, "test-name");
  testname->SetText(GetTestName(test));

  GuiText* key[MAX_KEY];
  GuiElement* colourSquares[MAX_KEY];
  for (int i = 0; i < MAX_KEY; i++) 
  {
    key[i] = (GuiText*)GetElementByName(m_gui, "key" + ToString(i + 1));
    key[i]->SetVisible(false);
    colourSquares[i] = GetElementByName(m_gui, "key" + ToString(i + 1) + "-img");
    colourSquares[i]->SetVisible(false);
  }

  ChartData* cd = new ChartData;
  GuiChart* chart = (GuiChart*)GetElementByName(m_gui, "results-chart");
  Assert(chart);
  chart->GetDataDisplay()->SetData(cd);

  typedef std::map<Time, Results> DateMap;
  DateMap datemap;

  // Get historical results from CogTestResults
  const bool DISPLAY_ONLY = true;
  Results r = TheCogTestResults::Instance()->GetResultsForTestType(test, DISPLAY_ONLY);
  if (r.empty())
  {
    // TODO Hide unused GUI
std::cout << "No test results for " << GetTestName(test) << "\n";
    return;
  }

std::cout << "Got " << r.size() << " results for " << GetTestName(test) << "\n";

  for (unsigned int i = 0; i < r.size(); i++)
  {
    Result* res = r[i];    
    Time t = res->GetTimestamp();
    t.RoundDown(TimePeriod(ONE_DAY_IN_SECONDS));

    // Put data into buckets for dates
    datemap[t].push_back(res);
  }
std::cout << " .. spread over " << datemap.size() << " dates.\n";

  // TODO Detect and fix situations where data is missing

  // Look at data, to set Key of chart and find max number of data columns
  // HACK as chart data cannot have missing values
  typedef std::map<std::string, int> KeyValMap;
  KeyValMap keyvalmap;

  // Reverse through the results, so the last results we look at will be 
  //  drawn first.
  for (DateMap::reverse_iterator it = datemap.rbegin(); it != datemap.rend(); ++it)
  {
    Results& results = it->second;
    int num = results.size();
    
    std::cout << "Got " << num << " results for test: "
      << GetTestName(test) << " for date: " 
      << datemap.begin()->first.ToStringJustDate()
      << " :-( \n";

    for (int i = 0; i < num; i++)
    {
      const std::string& key = results[i]->GetKey();
      int val = ToInt(results[i]->GetVal());
      keyvalmap[key] = val;
    }
  }

  // Set key text
  int k = 0;
  for (KeyValMap::iterator it = keyvalmap.begin(); it != keyvalmap.end(); it++)
  {
    if (k < 5) // TODO we need more keys
    {
      key[k]->SetText(it->first);
      key[k]->SetVisible(true);
      colourSquares[k]->SetVisible(true);
    }
    k++;
  }
  //for ( ; k < 5; k++)
  //{
  //  // Set remaining keys to invisible
  //  key[k]->SetVisible(false);
  //}

  int x = 0;
  int maxX = datemap.size();
  for (DateMap::iterator it = datemap.begin(); it != datemap.end(); ++it)
  {
    Time t = it->first;

    Results& r = it->second;
    int numResults = r.size();
    Assert(numResults > 0);

std::cout << "Num results for " << GetTestName(test) 
  << " for date " 
  << it->first.ToStringJustDate() 
  << ": " << numResults << "\n";
for (Results::iterator jt = r.begin(); jt != r.end(); ++jt)
{
  Result* res = *jt;
  std::cout << " key: " << res->GetKey() << " val: " << res->GetVal() << "\n";
}

    for (int i = 0; i < numResults; i++)
    {
      const std::string& key = r[i]->GetKey();
      int val = ToInt(r[i]->GetVal());
      keyvalmap[key] = val; // overwriting values, so we draw new values, or
        // previous values where no new value
    }

    ChartData::Row row;
    row.first = x;
    x++;

    /*
    for (Results::iterator jt = r.begin(); jt != r.end(); ++jt)
    {
      Result* res = *jt;

      float f = ToInt(res->GetVal());
      row.second.push_back((ChartData::YTYPE)f);
    }
    */

    for (KeyValMap::iterator it = keyvalmap.begin(); it != keyvalmap.end(); ++it)
    {
      int f = it->second;
      row.second.push_back((ChartData::YTYPE)f);
    }

    cd->AddRow(row);
    std::string label = "";
    if (maxX < 5 || 
        x == 1 || x == maxX) // first and last
    {
      label = t.ToStringJustDate();
    }
    cd->AddXAxisLabel(label);
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
    m_testId = AMJU_COG_TEST_STROOP_WORD; // no letter canc tests AMJU_COG_TEST_LETTER_CAN;
  }
  SetChart((TestId)m_testId);
}

void GSCogResults::OnPrev()
{
  m_testId--;
  if (m_testId < AMJU_COG_TEST_STROOP_WORD) // no no letter canc tests AMJU_COG_TEST_LETTER_CAN
  {
    m_testId = AMJU_COG_TEST_MAX - 1;
  }
  SetChart((TestId)m_testId);
}
} // namespace
