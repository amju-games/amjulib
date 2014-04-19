#!/usr/bin/perl -w

#############################################
## REPORT COG TEST RESULTS 
#############################################

require "common.pl";

my_connect();

print "<p style=\"font-family:Arial\">";

print "<b>Cog test results</b><br>\n";
sub showresults();
showresults();


##### end of main function

sub showresults()
{
  my $sql = "SELECT DISTINCT p.playername, p.email, s.start, rt.`key`, rt.`val`, rct.test_type FROM `research_cogtest` as rct, research_testresult as rt, session as s, player as p where rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id  order by rct.session_id desc, rct.test_type, rt.`key`";

  print "Query: $sql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  my %test_names = (
    3 => "Stroop word",
    4 => "Stroop colour",
    5 => "Stroop colour word",
    6 => "Reaction time",
    7 => "Trail making (sequential)",
    8 => "Trail making (alternate)"
  );

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


