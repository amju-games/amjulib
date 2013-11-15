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
  # Table of counts of concurrently logged in people
  my $sql = "select s.id, player_id, start, UNIX_TIMESTAMP(start), UNIX_TIMESTAMP(expires), expires-start, playername, email from session as s, player as p where s.player_id=p.id and start > DATE('2013-08-23') and start < DATE('2013-09-22') order by start ASC ";

  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute;

  print "<p style=\"font-family:Arial\">";
  print "<b>Concurrent Sessions</b><br><br>\n<table border=1>";

  print "<b> <tr><td>TIME</td> <td>COUNT</td> <td>Session ID</td> <td>Player ID</td> <td>START</td> <td>Start timestamp</td> <td>Expires</td> <td>Duration</td> <td>Player name</td> <td>Email</td>  <td>Count</td> <td>WHY</td> </tr> </b>\n";

  my $count = 0;
  my @end_q; # priority Q would be best here I think

  while (my ($id, $player_id, $start2, $start, $expires, $duration, $playername, $email) = $query->fetchrow_array)
  {
    # have we passed an ended session?
    # Loop over all the timestamps in end_q, in asc order. For any that are less than start time,
    #  dec count, write a new row, and remove elem from end_q.
    @end_q = sort(@end_q);
    while (@end_q && ($end_q[0] lt $start))
    {
      $count--;

      print "<tr>";
      print "<td>$end_q[0]</td> <td>$count</td> <td></td> <td/> <td></td> <td></td> <td>$end_q[0]</td> <td></td> <td></td> <td></td> <td>$count</td> <td>Finito: $end_q[0] </td>";
      print "</tr>\n";

      shift(@end_q); # remove first element
      @end_q = sort(@end_q);
    }
    
    if ($duration < 7200) # only treat sessions under 2 hours as serious
    {
      $count++;

      print "<tr>";
      print "<td>$start</td> <td>$count</td> <td>$id</td> <td>$player_id</td> <td>$start2</td> <td>$start</td> <td>$expires</td> <td>$duration</td> <td>$playername</td> <td>$email</td> <td>$count</td> <td>Started, @end_q </td>";
      print "</tr>\n";

      push(@end_q, $expires);
    }
  }
  print "</table>\n";
}


