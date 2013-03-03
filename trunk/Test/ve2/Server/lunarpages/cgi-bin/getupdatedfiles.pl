#!/usr/bin/perl -w

#############################################
## GET UPDATED FILES
#############################################

require "common.pl";

my_connect();

sub showupdatedfiles();
showupdatedfiles();


##### end of main function

sub showupdatedfiles()
{
  if (check_session() == 0)
  {
    return;
  }

  my $time = param('time') or die "Expected time since last check";

  my $sql = "select filename from fileupdate where whenchanged >= FROM_UNIXTIME($time)";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<files>\n";
  while (my ($filename) = $query->fetchrow_array)
  {
    print "<file>$filename</file>\n";
  }
  print "</files>\n";

}


