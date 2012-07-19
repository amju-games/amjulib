#!/usr/bin/perl -w

#############################################
## UPDATE OBJECT STATE REQUEST 
#############################################

require "common.pl";

my_connect();

sub updatestate();
updatestate();


##### end of main function

sub updatestate()
{
  if (check_session() == 0)
  {
    return;
  }

  my $key = param('key') or die "No key";
  my $val = param('val');
  my $obj_id = param('obj_id') or die "No ID";

  my $sql = "insert into objectstate values ($obj_id, '$key', '$val', now()) on duplicate key update val='$val', whenchanged=now()";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

}


