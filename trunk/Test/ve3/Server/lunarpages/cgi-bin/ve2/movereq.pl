#!/usr/bin/perl -w

#############################################
## MOVE REQUEST 
#############################################

require "common.pl";

my_connect();

#sub move();
#move();

sub update_research_table();
update_research_table();

sub update_research_table()
{
  if (check_session() == 0)
  {
    return;
  }

  my $x = param('x');
  my $y = param('y');
  my $z = param('z');
  my $loc = param('loc');
  my $obj_id = param('obj_id');

  # Update "research" table - log all data for analysis
  $sql = "insert into research_objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, $x, $y, $z, $loc, now())";
  insert($sql);
}


##### end of main function

sub move()
{
  if (check_session() == 0)
  {
    return;
  }

  my $x = param('x');
  my $y = param('y');
  my $z = param('z');
  my $loc = param('loc');
  my $obj_id = param('obj_id');

  # If location is not the same as the location the object is currently in, discard this request.
  # This is so once we go to another location, any outstanding move reqs from the old location are ignored.
  my $sql = "select loc from objectpos where id=$obj_id limit 1";
  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";
  $query->execute;
  if (my ($found_loc) = $query->fetchrow_array)
  {
    if ($found_loc != $loc)
    {
      print "Discarding move req from old location.\n";
      return;
    }
  }

  # TODO Check LOS to new requested location

  $sql = "insert into objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, $x, $y, $z, $loc, now()) on duplicate key update x=$x, y=$y, z=$z, loc=$loc, whenchanged=now() ";

  print "Query: $sql\n\n";

  $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;


  # Update "research" table - log all data for analysis
  $sql = "insert into research_objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, $x, $y, $z, $loc, now())";
  insert($sql);
}


