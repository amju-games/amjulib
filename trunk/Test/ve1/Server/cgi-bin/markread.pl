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

  my $sql = "update chat set candelete=1 where id=$id";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

}


