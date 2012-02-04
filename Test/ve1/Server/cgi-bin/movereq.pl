#!/usr/bin/perl -w

#############################################
## MOVE REQUEST 
#############################################

require "common.pl";

my_connect();

sub move();
move();


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


  # TODO Check LOS to new requested location

  my $sql = "insert into objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, $x, $y, $z, $loc, now()) on duplicate key update x=$x, y=$y, z=$z, loc=$loc, whenchanged=now() ";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;


  # Update "research" table - log all data for analysis
  $sql = "insert into research_objectpos (id, x, y, z, loc, whenchanged) values ($obj_id, $x, $y, $z, $loc, now())";
  insert($sql);
}


