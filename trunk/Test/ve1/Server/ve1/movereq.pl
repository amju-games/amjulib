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
  my $x = param('x');
  my $y = param('y');
  my $z = param('z');
  my $obj_id = param('obj_id');


  # TODO Check LOS to new requested location

  my $sql = "insert into objectpos (id, x, y, z) values ($obj_id, $x, $y, $z) on duplicate key update x=$x, y=$y, z=$z ";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

}


