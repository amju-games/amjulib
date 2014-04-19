#!/usr/bin/perl -w

#############################################
## GET OBJECT STATES
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

  my $sql = "SELECT id, `key`, val FROM objectstate WHERE whenchanged >= FROM_UNIXTIME($time)";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  print "<states>\n";
  while (my ($id, $key, $val) = $query->fetchrow_array)
  {
    print "<os>";
    print "<id>$id</id> <key>$key</key> <val>$val</val>";
    print "</os>\n";
  }
  print "</states>\n";

}


