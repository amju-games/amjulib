#!/usr/bin/perl -w

#############################################
## LIST PLAYERS
#############################################

require "common.pl";

my_connect();

print "<img src=\"../mygame.png\" align=center> <p style=\"font-family:Arial\">";

print "<b>Players</b><br>\n";
sub showplayers();
showplayers();


##### end of main function

sub showplayers()
{
  my $findplayersql = "select * from player";

  print "Query: $findplayersql\n\n";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($findplayersql) or die
    "Query prepare failed for this query: $findplayersql\n";

  $query->execute;
  print "<table border=1>";
  print "<tr><td>ID</td> <td>Name</td> <td>Email</td> <td>Hashpw</td> <td>Thumbnail</td> <td>Last time</td> <td>Object ID</td><td></td></tr>\n";
  while (my ($player_id, $name, $email, $hashpw, $thumbnail, $lasttime, $obj_id) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$player_id</td> <td>$name</td> <td>$email</td> <td>$hashpw</td> <td>$thumbnail</td> <td>$lasttime</td> <td>$obj_id</td>\n";
    print "<td>";

    #<input type=\"button\" onclick=\"window.location = 'editplayer.pl'\" value=\"edit...\" /></td>\n";

    print "<form name=\"input\" method=\"get\" action=\"adminsendmsg.pl\"><input name=\"recip\" type=\"hidden\" value=\"$player_id\"/><input type=\"text\" name=\"msg\"/><input type=\"submit\" value=\"Send msg\" /></form></td>";
    print "</tr>";
  }
  print "</table>";

}


