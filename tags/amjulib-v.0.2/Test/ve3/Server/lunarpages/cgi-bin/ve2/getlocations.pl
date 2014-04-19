#!/usr/bin/perl -w

#############################################
## GET LIST OF VALID LOCATIONS
#############################################

require "common.pl";

my_connect();

sub showlocs();
showlocs();


##### end of main function

sub showlocs()
{
  if (check_session() == 0)
  {
    return;
  }

  my $sql = "select id, name from location where obj_id >= 1";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<locs>\n";
  while (my ($id, $name) = $query->fetchrow_array)
  {
    print "<loc>";
    print "<id>$id</id> <name>$name</name>";
    print "</loc>\n";
  }
  print "</locs>\n";

}


