#!/usr/bin/perl -w

#############################################
## MARK MSG AS READ REQUEST 
#############################################

require "common.pl";

my_connect();

sub markread();
markread();


##### end of main function

sub markread()
{
  if (check_session() == 0)
  {
    return;
  }

  my $id = param('id') or die "No id";

  # Get info about the specified chat, then mark all similar, older msgs as read too.
  my $sql = "select recip, sender, UNIX_TIMESTAMP(whensent) from chat where id=$id";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";
  $query->execute;
  if (my ($recip, $sender, $whensent) = $query->fetchrow_array)
  {
    $sql = "update chat set candelete=1 where recip=$recip and sender=$sender and whensent <= FROM_UNIXTIME($whensent)";

    # Bad one, the < in the query looks like bad XML to the client!
    #print "Query: $sql\n\n";

    update($sql);
  }
  else 
  {
    print "Failed to execute query, $query";
  }
}


