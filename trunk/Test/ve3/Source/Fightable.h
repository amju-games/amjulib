#ifndef FIGHTABLE_H_INCLUDED
#define FIGHTABLE_H_INCLUDED

namespace Amju
{
// Base class for attacks
class Attack
{
};

// Base class for game objects which can be in Fights.
class Fightable
{
public:
  void SetOpponent(Fightable* opponent);

  void SendAttack(Attack*); // do thie attack on opponent
  void RecvAttack(Attack*); // the attack is done on us

  bool IsDead();

};

}

#endif

