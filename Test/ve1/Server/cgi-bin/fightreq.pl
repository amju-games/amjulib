#!/usr/bin/perl -w

#############################################
## FIGHT REQUEST 
#############################################

require "common.pl";

my_connect();

sub fight();
fight();


##### end of main function

sub getstat($$)
{
  return 10;
}

sub fight()
{
  if (check_session() == 0)
  {
    return;
  }

  # Fighting player object ID (not required, get from session ?)
  my $id = 1; #param('id') or die "No id";
  my $opp = 3; #param('opp') or die "No opponent id";

  # TODO Special stats table ????? OR read-only flag for keys
  
  # TODO Look up skill stats 
  my $skill = getstat($id, "skill");
  my $oppskill = getstat($opp, "skill");

print "Your skill: $skill  Opponent skill: $oppskill\n";

#  my $stamina = getstat($id, "stamina");
#  my $oppstamina = getstat($opp, "stamina");
#print "Your stamina: $stamina  Opponent stamina: $oppstamina\n";

  # Do Fighting Fantasy algo to get damages
  my $attack = rand(12) + 1 + $skill;
  my $oppattack = rand(12) + 1 + $oppskill;

print "Your attack: $attack Opponent attack: $oppattack\n";

  my $damage = 0;
  my $oppdamage = 0; 
  if ($attack > $oppattack)
  {
    $oppdamage = 1;
#    $oppstamina -= $oppdamage;
    update("update objectstate set `val`=`val` - 1, whenchanged=now() where `id`=$opp and `key`='stamina'");
  }
  else
  {
    $damage = 1;
#    $stamina -= $damage;
    update("update objectstate set `val`=`val` - 1, whenchanged=now() where `id`=$id and `key`='stamina'");
  }

print "You take: $damage damage  Opponent takes: $oppdamage damage\n";
print "<damage><you>$damage</you><opp>$oppdamage</opp></damage>\n";
# TODO Make this a common func
print "<collect> <num>3</num> <type>0</type> <id>12345</id> </collect>\n";

#print "Your NEW stamina: $stamina  Opponent NEW stamina: $oppstamina\n";

}


