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
  my $sql = "select * from objectpos";

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


