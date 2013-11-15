#!/usr/bin/perl -w

#############################################
## SHOW PLAYER STATE
#############################################

require "common.pl";

my_connect();

sub showplayerstate();
showplayerstate();


##### end of main function

sub showplayerstate()
{
  my $player = param('player');
  my $sql = "select `key`, `val`, whenchanged from objectstate as os, player as pl where os.id=pl.obj_id and pl.id=$player order by whenchanged desc";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<p style=\"font-family:Arial\">";
  print "<b>Player state for player ID $player</b><br><br>\n<table border=1>";

  print "<b> <tr><td>Key</td> <td>Value</td> <td>Last changed</td> </tr> </b>\n";

  while (my ($key, $val, $whenchanged) = $query->fetchrow_array)
  {
    print "<tr>";
    print "<td>$key</td> <td>$val</td> <td>$whenchanged</td> ";
    print "</tr>\n";
  }
  print "</table>\n";

}


