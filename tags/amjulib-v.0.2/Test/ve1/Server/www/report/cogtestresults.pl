#!/usr/bin/perl -w

#############################################
## REPORT COG TEST RESULTS 
#############################################

require "common.pl";

my_connect();

print "<img src=\"../mygame.png\" align=center> <p style=\"font-family:Arial\">";

print "<b>Cog test results</b><br>\n";
sub showresults();
showresults();


##### end of main function

sub showresults()
{
  my $sql = "SELECT p.playername, p.email, s.start, rtn.name, rt.`key`, rt.`val` FROM `research_cogtest` as rct, research_testresult as rt, research_testname as rtn, session as s, player as p where rct.session_id=s.id and s.player_id=p.id and rct.id=rt.test_id and rtn.id=rct.test_type order by rct.session_id, rct.test_type";

  print "Query: $sql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;
  print "<table border=1>";
  print "<tr><td>Name</td> <td>Email</td> <td>Date/Time</td> <td>Test name</td> <td>Key</td><td>Value</td> <td></td><td></td></tr>\n";
  while (my ($playername, $email, $starttime, $testname, $key, $val) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$playername</td> <td>$email</td> <td>$starttime</td> <td>$testname</td> <td>$key</td> <td>$val</td> <td></td>\n";
    print "<td> </td>";

    #<input type=\"button\" onclick=\"window.location = 'editplayer.pl'\" value=\"edit...\" /></td>\n";

    #print "<form name=\"input\" method=\"get\" action=\"adminsendmsg.pl\"><input name=\"recip\" type=\"hidden\" value=\"$obj_id\"/><input type=\"text\" name=\"msg\"/><input type=\"submit\" value=\"Send msg\" /></form></td>";
    print "</tr>";
  }
  print "</table>";

}


