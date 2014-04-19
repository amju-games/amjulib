#!/usr/bin/perl -w

require "common.pl";

sub logout()
{
  my $session_id = param('session_id') or die "No session id";

  # Get Object ID for player from session ID
  my $sql = "select player.obj_id from player, session where player.id = session.player_id and session.id=$session_id limit 1";
  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute;
  if (my ($player_obj_id) = $query->fetchrow_array)
  {
    # Set 'loggedin' to 'n'
    $sql = "insert into objectstate (`id`, `key`, `val`) values($player_obj_id, 'loggedin', 'n') on duplicate key update val='n'";
    insert($sql); 


    # Make sure any carried object is dropped: any row where we have picked up an object 
    #  has the 'carrier' ID set to 0
    $sql = "update objectstate set val='0' where `key`='pickup' and val='$player_obj_id'";
    update($sql);  

  }

  $sql = "update session set `expires`=now() where id=$session_id";
  update($sql);
}


