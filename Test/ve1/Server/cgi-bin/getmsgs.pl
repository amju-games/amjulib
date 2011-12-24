#!/usr/bin/perl -w

#############################################
## GET NEW MSGS 
#############################################

require "common.pl";

my_connect();

sub showmsgs();
showmsgs();


##### end of main function

sub showmsgs()
{
  if (check_session() == 0)
  {
     return;
  }

  my $time = param('time') or die "Expected time since last check";
  my $recip = param('recip') or die "Expected recip";

  my $sql = "SELECT id, sender, whensent, msg FROM chat WHERE recip = $recip and whensent >= FROM_UNIXTIME($time)";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  print "<msgs>\n";
  while (my ($id, $sender, $whensent, $msg) = $query->fetchrow_array)
  {
    print "<msg>";
    print "<id>$id</id> <sender>$sender</sender> <whensent>$whensent</whensent> <msg>$msg</msg>";
    print "</msg>\n";
  }
  print "</msgs>\n";

}


