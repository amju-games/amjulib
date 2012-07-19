#!/usr/bin/perl -w

require "common.pl";

my_connect();
print "Getting ID...\n";
sub request_id();
request_id();

#foreach $key (sort(keys %ENV)) {
#    print "$key = $ENV{$key}<br>\n";
#}

print "That's it!\n";

##### end of main function

sub request_id()
{
  my $playername = param('playername');
  my $sig = param('sig');

  # See if the player/ip address already exists
  my $findplayersql = "select id from player where playername='$playername' and sig='$sig'";

print "Query: $findplayersql\n\n";
  if (not $dbh) { print "Oh shit, dbh is undefined!?!?!\n"; }

  my $query = $dbh->prepare($findplayersql) or die
    "Query prepare failed for this query: $findplayersql\n";

  $query->execute;
  my $player_id = 0;
  if (($player_id) = $query->fetchrow_array)
  {
    print "Found player in player table, ID:$player_id\n";
    print "<id>$player_id</id>";
    return;
  }

  my $insertsql = "insert into player (playername, sig, level, thumbnail, rating, lasttime) values ('$playername', '$sig', '', '', 0, now())";
  insert($insertsql);

  my $querysql = "select id from player where playername='$playername' and sig='$sig'";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  while (my ($id) = $query->fetchrow_array)
  {
    print "<id>$id</id>";
  }
}


