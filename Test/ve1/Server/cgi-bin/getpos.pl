#!/usr/bin/perl -w

#############################################
## GET POSITIONS
#############################################

require "common.pl";

my_connect();

sub getpos();
getpos();


##### end of main function

sub getpos()
{
  if (check_session() == 0)
  {
    return;
  }

  my $time = param('time') or die "Expected time since last check";

  my $sql = "SELECT id, x, y, z FROM objectpos WHERE whenchanged >= FROM_UNIXTIME($time)";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<objs>\n";
  while (my ($id, $x, $y, $z) = $query->fetchrow_array)
  {
    print "<obj>";
    print "<id>$id</id> <x>$x</x> <y>$y</y> <z>$z</z>";
    print "</obj>\n";
  }
  print "</objs>\n";

}


