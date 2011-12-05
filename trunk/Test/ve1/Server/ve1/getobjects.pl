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
  my $sql = "select * from object";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<objs>\n";
  while (my ($id, $type, $assetfile, $datafile, $owner, $createtime) = $query->fetchrow_array)
  {
    print "<obj>";
    print "<id>$id</id> <type>$type</type> <assetfile>$assetfile</assetfile> <datafile>$datafile</datafile> <owner>$owner</owner> <createtime>$createtime</createtime>";
    print "</obj>\n";
  }
  print "</objs>\n";

}


