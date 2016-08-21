#!/usr/bin/perl -w

#############################################
## GET OBJECTS
#############################################

require "common.pl";

my_connect();

sub showsessions();
showsessions();


##### end of main function

sub showsessions()
{
  my $sql = "select s.id, player_id, start, expires, expires-start, playername, email from session as s, player as p where s.player_id=p.id order by s.id DESC";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<p style=\"font-family:Arial\">";
  print "<b>Sessions</b><br><br>\n<table border=1>";

  print "<b> <tr><td>Session ID</td> <td>Player ID</td> <td>Start timestamp</td> <td>Expires</td> <td>Duration</td> <td>Player name</td> <td>Email</td>  </tr> </b>\n";

  while (my ($id, $player_id, $start, $expires, $duration, $playername, $email) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$id</td> <td>$player_id</td> <td>$start</td> <td>$expires</td> <td>$duration</td> <td>$playername</td> <td>$email</td>";
    print "</tr>\n";
  }
  print "</table>\n";

}


