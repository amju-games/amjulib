#!/usr/bin/perl -w

#############################################
## UPDATE OBJECT STATE REQUEST 
#############################################

require "common.pl";

sub publishRecentChanges();
sub updatestate();

my_connect();
updatestate();


##### end of main function

sub updatestate()
{
  if (check_session() == 0)
  {
    return;
  }

  my $key = param('key') or die "No key";
  my $val = param('val'); # could be zero!
  my $obj_id = param('obj_id') or die "No ID";

  my $sql = "insert into objectstate values ($obj_id, '$key', '$val', now()) on duplicate key update val='$val', whenchanged=now()";
  update_or_insert($sql);

  publishRecentChanges();
}

sub publishRecentChanges()
{
  # The idea here is that updates occur much less frequently than polling.
  # So publish changes in a document, so we don't hit the DB so frequently.
  # If file is locked, we would like to wait until it's unlocked.
  # OR we just fail and don't write this time.

  my $reportPeriod = 5; # seconds
  my $sql = "SELECT id, `key`, val, UNIX_TIMESTAMP(whenchanged) FROM objectstate WHERE whenchanged >= FROM_UNIXTIME(UNIX_TIMESTAMP(now()) - $reportPeriod)";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  # Open file
  my $root = "../../ve2/updates"; 
  my $filename = "$root/updates.txt";

  if (!open(theFile, ">$filename"))
  {
    print "Failed to open file for writing: $filename\n";
    return;
  }

  print theFile "<states>\n";
  while (my ($id, $key, $val, $timestamp) = $query->fetchrow_array)
  {
    print theFile "<os> <id>$id</id> <key>$key</key> <val>$val</val> <timestamp>$timestamp</timestamp> </os>\n";
  }
  print theFile "</states>\n";

  close(theFile);
}

