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

  # The session during which the test took place -- not necessarily this session
  my $session = param('research_sessionid') or die "No research session"; 

  my $test_type = param('test_type') or die "No test type";
  my $key = param('key') or die "No key";
  my $val = param('val'); # values can be zero! ### or die "No val";

  # session and test type are unique.
  # Make sure this test has a row in research_cogtest
  # Look up or create ID for this unique pair (session id, test type).

  my $sql = "select id from research_cogtest where session_id=$session and test_type=$test_type";
  my $query = $dbh->prepare($sql) or die "Prepare failed for: '$sql'";
  $query->execute or die "Failed to execute '$sql'";
  my $testid;
  if (! (($testid) = $query->fetchrow_array))
  {
    $sql = "insert into research_cogtest(`test_type`, `session_id`) values ($test_type, $session)";
    insert($sql); 
    $sql = "select LAST_INSERT_ID() limit 1";
    $query = $dbh->prepare($sql) or die "Prepare failed for: '$sql'";
    $query->execute or die "Failed to execute '$sql'";
    if (! (($testid) = $query->fetchrow_array))
    {
      # Error!
      die "Failed to get last insert id!";
    }
  }

  # Enter row for this test in research_testresult
  $sql = "insert into research_testresult (`test_id`, `key`, `val`) values ($testid, '$key', '$val')";
  insert($sql);
 
  print "Query: $sql\n\n";


  # Acknowledge result stored
  print "<stored>1</stored>";
  
  # Reward for taking test
  # TODO base on result ?
  # TODO more/variety
  print "<collect> <num>3</num> <type>0</type> <id>12345</id> </collect>\n"; 
}


