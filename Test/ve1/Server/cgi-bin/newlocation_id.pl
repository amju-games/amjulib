#!/usr/bin/perl -w

#############################################
## NEW LOCATION ID
## Creates new, empty row in location table
## Returns new ID 
#############################################

require "common.pl";

my_connect();

sub newid();
newid();


##### end of main function

sub newid()
{
  if (check_session() == 0)
  {
    return;
  }

  my $name = param('name');

  # No object ID yet, and early timestamp, so we don't recognise it as new until we populate it.
  my $sql = "insert into location (obj_id, whenchanged, name) values (-1, 1, $name) on duplicate key update id=id+1";

#  print "Query: $sql\n\n";

  insert($sql);

  $sql = "select LAST_INSERT_ID() limit 1";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  if (my ($id) = $query->fetchrow_array)
  {
    print "<id>$id</id>";
  }
  else 
  {
    print "<id>-1</id>";
  }
}


