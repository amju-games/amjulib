#!/usr/bin/perl -w

#############################################
## SHOW PLAYER SESSIONS
## Creates a .csv file for each player with
##  login info 
#############################################

require "common.pl";

my_connect();

sub showsessions($);
sub showresults();

showresults();


##### end of main function

sub showsessions($)
{
  my $this_player = shift;

  # public_html/ve2/results
  my $dir = "../../ve2/results/";
  my $filename = $dir . "player-$this_player-sessions.csv";

  open (OUTFILE, ">$filename") or die "Failed to open output file $filename";
  print "Writing to file $filename<br>\n";

  print OUTFILE "Date, Yes, Session, Duration\n";

  my $sql = "select s.id, player_id, start, expires, expires-start, playername, email from session as s, player as p where s.player_id=p.id and p.id=$this_player and start > DATE('2014-05-08')";

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

    print OUTFILE "$start, 1, $id, $duration\n";
  }
  print "</table>\n";

  close(OUTFILE);
}

sub showresults()
{
  my $sql = "SELECT id, playername, email from player";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";

  $query->execute;
  while (my ($player_id, $playername, $email) = $query->fetchrow_array)
  {
    print "Player ID: $player_id Name: $playername Email: $email<br>\n";

    showsessions($player_id);
    print "\n<br><br>\n";
  }
}

