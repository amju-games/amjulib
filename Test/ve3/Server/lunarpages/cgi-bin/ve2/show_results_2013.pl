#!/usr/bin/perl -w

#############################################
## WRITE CSV FILES FOR TEST RESULTS BY PLAYER
##  THIS IS FOR 2013 EXPERIMENT
#############################################

require "common.pl";

my_connect();

  my %test_names = (
    3  => "Stroop word",
    4  => "Stroop colour",
    5  => "Stroop colour word",
    6  => "Reaction time",
    7  => "Trail making (sequential)",
    8  => "Trail making (alternate)"
  );

print "<p style=\"font-family:Arial\">";

print "<b>Test results by player</b><br>\n";
sub showresults();
showresults();


##### end of main function

sub showresultsforplayerfortest($$)
{
  my $player_id = shift;
  my $test_id = shift;

  print "Test results for player $player_id for test $test_id:<br>\n";

  my $sql = "SELECT DISTINCT p.playername, p.email, s.start, rt.`key`, rt.`val`, rct.test_type FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where p.id=$player_id and rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and rct.test_type=$test_id order by rct.session_id desc, rct.test_type, rt.`key`";

  #print "Query: $sql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<table border=1>";
  print "<tr><td>Name</td> <td>Email</td> <td>Date/Time</td> <td>Test name</td> <td>Key</td><td>Value</td> </tr>\n";
  while (my ($playername, $email, $starttime, $key, $val, $test_type) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$playername</td> <td>$email</td> <td>$starttime</td> <td>$test_names{$test_type}</td> <td>$key</td> <td>$val</td> \n";

    #<input type=\"button\" onclick=\"window.location = 'editplayer.pl'\" value=\"edit...\" /></td>\n";

    #print "<form name=\"input\" method=\"get\" action=\"adminsendmsg.pl\"><input name=\"recip\" type=\"hidden\" value=\"$obj_id\"/><input type=\"text\" name=\"msg\"/><input type=\"submit\" value=\"Send msg\" /></form></td>";
    print "</tr>";
  }
  print "</table>";

}

sub showresultsforplayerbytest($)
{
  my $player_id = shift;
  
  for (my $i = 3; $i <= 10; $i++)
  {
    showresultsforplayerfortest($player_id, $i);
  }
  print "\n<br>\n";
}

sub show_headings_forplayerfordate_test($$$$)
{
  my $handle = shift;
  my $player_id = shift;
  my $date = shift;
  my $test_type = shift;

  my $sql = "SELECT DISTINCT rt.`key`  FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where p.id=$player_id and rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and rct.test_type=$test_type and s.start >= '2013-08-23' and s.start <= '2013-09-27' order by rt.`key`";

  #print "Query: $sql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";

  $query->execute;

  my $first = 1;
  print "<tr>";

  while (my ($key) = $query->fetchrow_array)
  {
    if ($first)
    {
      print "<td>Date</td> ";
      print $handle "Date";
      $first = 0;
    }
    print " <td>$key</td> \n";
    print $handle ",$key";
  }
  print "</tr>\n";
  print $handle "\n";
}

sub showresultsforplayerfordate_test($$$$)
{
  my $handle = shift;
  my $player_id = shift;
  my $date = shift;
  my $test_type = shift;

  my $sql = "SELECT DISTINCT p.playername, p.email, s.start, rt.`key`, rt.`val`, rct.test_type FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where p.id=$player_id and rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and rct.test_type=$test_type and s.start='$date' order by rct.test_type, rt.`key`";

#  print "Query: $sql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";

  $query->execute;

  my $first = 1;
  print "<tr>";

  while (my ($playername, $email, $starttime, $key, $val, $test_type) = $query->fetchrow_array)
  {
    if ($first)
    {
      print "<td>$starttime</td> \n";
      print $handle "$starttime";
      $first = 0;
    }
    print " <td>$val</td> \n";
    print $handle ",$val";
  }
  print "</tr>";
  print $handle "\n";
}

sub showresultsforplayerbydate_testtype($$)
{
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $player_id = shift;
  my $test_type = shift;
 
  my $sql = "SELECT count(DISTINCT s.start) FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where p.id=$player_id and rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and s.start >= '2013-08-23' and s.start <= '2013-09-27' order by s.start";
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute;
  my($count) = $query->fetchrow_array or die "Couldn't run query: $sql";
  if ($count == 0)
  {
    print "No results for test: $test_names{$test_type}<br>\n";
    return;
  } 

  $sql = "SELECT DISTINCT s.start FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where p.id=$player_id and rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and s.start >= '2013-08-23' and s.start <= '2013-09-27' order by s.start";

  #print "Query: $sql\n\n";

  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";

  $query->execute;

  # public_html/ve2/results
  my $dir = "../../ve2/results/2013/";
  my $filename = $dir . "player-$player_id-test-$test_type.csv";

  open (OUTFILE, ">$filename") or die "Failed to open output file $filename";
  print "Writing to file $filename<br>\n";

  print "Test: $test_names{$test_type}<br>\n";
  print "<table border=1>";
  my $first = 1;
  while (my ($date) = $query->fetchrow_array)
  {
    if ($first)
    {
      show_headings_forplayerfordate_test(OUTFILE, $player_id, $date, $test_type); 
      $first = 0;
    }

    showresultsforplayerfordate_test(OUTFILE, $player_id, $date, $test_type); 
  }
  print "</table>\n<br><br>\n";

  close (OUTFILE);
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

    for (my $i = 3; $i <= 8; $i++)
    {
      showresultsforplayerbydate_testtype($player_id, $i);
    }
    print "\n<br><br>\n";
  }
}

