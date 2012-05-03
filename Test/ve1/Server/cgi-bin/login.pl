#!/usr/bin/perl -w

#############################################
## LOGIN
#############################################

require "common.pl";

my_connect();

sub login();
login();


##### end of main function

sub login()
{
  my $email = param('email') or die "Expected email\n";

print "Email: \"$email\"\n";

  my $sql = "select id from player where email='$email'";

print "Query: $sql\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  if (my ($player_id) = $query->fetchrow_array)
  {
    # DON'T Delete old session
    #$sql = "delete from session where player_id=$player_id";
    #print "Query: $sql\n";
    #$query = $dbh->prepare($sql) or die
    #  "Query prepare failed for this query: $sql\n";
    #$query->execute;

    # Create new session
    # Session lasts 24 hours..?
    $sql = "insert into session (player_id, start, expires) values ($player_id, now(), FROM_UNIXTIME(UNIX_TIMESTAMP(now()) + 3600*24)) on duplicate key update id=id+1";
    insert($sql);

    # Get session ID for this player and start location
    $sql = "select now(), a.id, b.playername, b.obj_id, c.loc from session as a, player as b, objectpos as c where a.player_id=$player_id and a.player_id=b.id and b.obj_id=c.id and a.id=LAST_INSERT_ID()";

print "Query: $sql\n";


    $query = $dbh->prepare($sql) or die
      "Query prepare failed for this query: $sql\n";
    $query->execute;
    if (my ($now, $session_id, $playername, $objid, $loc) = $query->fetchrow_array)
    {
print "Your new session ID: $session_id\n";

      print "<now>$now</now> <session>$session_id</session><playername>$playername</playername><objid>$objid</objid><loc>$loc</loc>\n";
   
      # Set logged in flag as object value 
      $sql = "insert into objectstate (`id`, `key`, `val`) select player.obj_id, 'loggedin', 'y' from player, session where player.id = session.player_id and session.id=$session_id on duplicate key update val='y'";
      insert($sql);
    }
    else
    {
      print "Errr.. failed to create session?!\n";
    }
  }
  else
  {
    print "No such email\n";
  }
}


