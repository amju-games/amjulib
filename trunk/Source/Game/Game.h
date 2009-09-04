#ifndef AMJU_GAME_H
#define AMJU_GAME_H

#include "Singleton.h"
#include "GameState.h"
#include "GameObject.h"
#include "EventListener.h"

namespace Amju
{
class Game : public NonCopyable, public EventListener
{
private:
  Game();
  friend class Singleton<Game>;

public:
  void Update();
  void Draw();
  void Run();

  // Event Listener
  virtual void OnQuitEvent();

  // Game States
  PGameState GetState(); // Get current state
  PGameState GetState(const char* stateName); // Get named state
  void SetCurrentState(const char* stateName);
  bool AddState(const char* stateName, PGameState state);

  // Game Objects
  typedef std::map<int, PGameObject> GameObjects;

  PGameObject GetGameObject(int id);
  void AddGameObject(PGameObject object);
  void EraseGameObject(int id);
  void ClearGameObjects(); // erase all
  GameObjects* GetGameObjects();

  // Functions commonly used by Game States
  void UpdateGameObjects();

private:
  void UpdateState(); // go to new state if set

private:
  typedef std::map<std::string, PGameState> GameStates;
  GameStates m_states;
  PGameState m_currentState;
  PGameState m_newState;

  GameObjects m_objects;
};
typedef Singleton<Game> TheGame;
}

#endif
