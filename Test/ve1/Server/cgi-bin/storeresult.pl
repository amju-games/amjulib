#!/usr/bin/perl -w

#############################################
## STORE RESULT FROM COGNITIVE TEST
#############################################

require "common.pl";

my_connect();

sub storeresult();
storeresult();


##### end of main function

sub storeresult()
{
  if (check_session() == 0)
  {
    return;
  }

#  my $testid = param('testid') or die "No test ID";

  my $session = param('session_id'); # must be OK
  my $key = param('key') or die "No key";
  my $val = param('val') or die "No val";

  my $sql = "insert into research_testresult (`session_id`, `key`, `val`) values ($session, '$key', '$val')";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  # Acknowledge result stored
  print "<stored>1</stored>";
}


