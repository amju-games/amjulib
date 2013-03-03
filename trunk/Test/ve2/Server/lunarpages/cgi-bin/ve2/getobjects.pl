#!/usr/bin/perl -w

#############################################
## GET OBJECTS
#############################################

require "common.pl";

my_connect();

sub showobjects();
showobjects();


##### end of main function

sub showobjects()
{
  if (check_session() == 0)
  {
    return;
  }

  my $time = param('time') or die "Expected time since last check";

  my $sql = "select id, type, assetfile, datafile, owner from object where createtime >= FROM_UNIXTIME($time)";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<objs>\n";
  while (my ($id, $type, $assetfile, $datafile, $owner) = $query->fetchrow_array)
  {
    print "<obj>";
    print "<id>$id</id> <type>$type</type> <assetfile>$assetfile</assetfile> <datafile>$datafile</datafile> <owner>$owner</owner>";
    print "</obj>\n";
  }
  print "</objs>\n";

}


