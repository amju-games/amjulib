#!/usr/bin/perl -w

#############################################
## NEW OBJECT ID
## Creates new, empty row in object table
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

  my $obj_type = param('type');
  my $assetfile = param('assetfile');
  my $datafile = param('datafile');
  my $alive = param('alive');

  # Get owner
  my $session_id = param('session_id');
  my $sql = "select player_id from session where id=$session_id limit 1";
  my $query = $dbh->prepare($sql) or die "Prepare failed for: '$sql'";
  $query->execute or die "Failed to execute '$sql'";
  my $owner;
  if (($owner) = $query->fetchrow_array)
  {
    print "Got owner! $owner";
  }
  else
  {
    print "Couldn't get owner for this session..?!";
    return;
  }

  # No object ID yet, and early timestamp, so we don't recognise it as new until we populate it.
  # If we set "alive" param, the object is fully created with valid timestamp.
  my $createtime = "1";
  if ($alive)
  {
    $createtime = "now()";
  }

  $sql = "insert into object (type,assetfile,datafile,owner,createtime) values ('$obj_type','$assetfile','$datafile',$owner, $createtime)";

#  print "Query: $sql\n\n";

  insert($sql);

  $sql = "select LAST_INSERT_ID() limit 1";

  $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  if (my ($id) = $query->fetchrow_array)
  {
    print "<id>$id</id>";
  }
  else 
  {
    print "<id>-1</id> This means we failed to get last_insert_id()";
  }
}


