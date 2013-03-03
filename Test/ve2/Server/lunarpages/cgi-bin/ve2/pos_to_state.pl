#!/usr/bin/perl -w

#############################################
## POS TO STATE - move data in objectpos to objectstate 
#############################################

require "common.pl";

my_connect();

sub getpos();
getpos();


##### end of main function

sub getpos()
{
  print "Copying data from objectpos to objectstate...<br>\n";

  my $sql = "SELECT ob.id, obpos.x, obpos.y, obpos.z, obpos.loc FROM objectpos as obpos, object as ob where obpos.id=ob.id and ob.createtime > 0";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  while (my ($id, $x, $y, $z, $loc) = $query->fetchrow_array)
  {
    print "Here is object pos: ID: $id x:$x y:$y z:$z loc:$loc<br>\n";
    my $str = "$x,$y,$z,$loc"; # TODO integerise ?
    my $ins = "insert into objectstate (id, `key`, val, whenchanged) values ($id, 'pos', '$str', now())";
    print "$ins <br>\n";

    insert($ins);
  }

}


