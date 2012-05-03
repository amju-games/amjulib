#!/usr/bin/perl -w

#############################################
## LOGOUT
## Well-behaved clients call this when user
##  wants to end session.
#############################################

require "common.pl";

my_connect();

sub logout();
logout();


##### end of main function

sub logout()
{
  my $session_id = param('session_id') or die "No session id";

  my $sql = "update session set `expires`=now() where id=$session_id";
  insert($sql);

  $sql = "insert into objectstate (`id`, `key`, `val`) select player.obj_id, 'loggedin', 'n' from player, session where player.id = session.player_id and session.id=$session_id on duplicate key update val='n'";
  insert($sql); 
  
}


