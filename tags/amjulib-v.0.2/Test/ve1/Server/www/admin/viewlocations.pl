#!/usr/bin/perl -w

#############################################
## VIEW LOCATIONS 
#############################################

require "common.pl";

print "Content-type:text/html\n\n";

print <<END_HTML;
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html>
<head> <meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> <title>Thanks!</title>
END_HTML

my_connect();

sub showlocs;
showlocs();


##### end of main function

sub showlocs()
{
  # Get num locations
  my $sql = "select max(loc) from objectpos limit 1";
  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";
  $query->execute;
  my ($max_location) = $query->fetchrow_array;

print "Max location: $max_location\n";  

  for (my $i = 0; $i <= $max_location; $i++)
  {
    my $sql = "select a.id, type, assetfile, datafile, owner, x, y, z from object as a, objectpos as b where a.id=b.id and b.loc=$i";
#    print "Query: $sql\n\n";
    my $query = $dbh->prepare($sql) or die
      "Query prepare failed for this query: $sql\n";
    $query->execute;

    print "<br>Location $i<br><table border=\"1\">\n";
    print "<tr><b><td>ID</td><td>Type</td><td>Asset File</td><td>Data file</td><td>Owner</td><td>x</td><td>y</td><td>z</td></b></tr>\n";
    while (my ($id, $type, $assetfile, $datafile, $owner, $x, $y, $z) = $query->fetchrow_array)
    {
      print "<tr><td>$id</td><td>$type</td><td>$assetfile</td><td>$datafile</td><td>$owner</td><td>$x</td><td>$y</td><td>$z</td><tr>\n";
    }

    print "</table><br>";
    print "<a href=\"../www/newobject.html\">Create a new object in this location...</a><br>\n";
    print "<br><br>\n";
  }
  print "<a href=\"../www/newlocation.html\">Create a new location...</a><br>\n";
}


