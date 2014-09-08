#!/usr/bin/perl -w

#############################################
## SHOW PLAYERS 
#############################################

require "common.pl";

my_connect();

sub showplayers();
showplayers();


##### end of main function

sub showplayers()
{
  my $sql = "select id, playername, email, obj_id from player as p order by id";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<p style=\"font-family:Arial\">";
  print "<b>Players</b><br><br>\n<table border=1>";

  print "<b> <tr><td>Player ID</td> <td>Player name</td> <td>Email</td> <td/> <td/> <td/> </tr> </b>\n";

  while (my ($id, $playername, $email, $obj_id) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$id</td> <td>$playername</td> <td>$email</td> <td>";
    my $mug = "../../../../ve2/mugshots/mugshot-" . $obj_id . ".png";
##    if (-e $mug)
    {
      print "<img src='$mug'>";
    }
    print "</td> <td><a href=\"show_player_sessions.pl?player=$id\">Sessions</a></td><td><a href=\"show_player_state.pl?player=$id\">State</a></td>";
    print "</tr>\n";
  }
  print "</table>\n";

}


