#!/usr/bin/perl -w

#############################################
## NEW PLAYER
#############################################

require "common.pl";

my_connect();

sub request_id();
request_id();

print "That's it!\n";

##### end of main function

sub request_id()
{
  my $playername = param('playername') or die "Expected playername";
  my $email = param('email') or die "Expected email";
  my $hashpw = param('hashpw') or die "Expected hashpw";

  # See if the player/ip address already exists
  my $findplayersql = "select id from player where email='$email'";

  print "Query: $findplayersql\n\n";
  if (not $dbh) { print "dbh is undefined!?!?!\n"; }

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

  my $insertsql = "insert into player (playername, email, hashpw, thumbnail, lasttime) values ('$playername', '$email', 'HASHPW', '', now())";
  insert($insertsql);

  my $querysql = "select id from player where email='$email'";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $playerid;
  ($playerid) = $query->fetchrow_array;
  
  print "<playerid>$playerid</playerid>";

  print "Player ID=$playerid<br>\n";

  $insertsql = "insert into object (type, assetfile, owner, createtime) values ('player', 'player.txt', $playerid, now())";
  print "<br>$insertsql<br>\n";


  insert($insertsql);
  $querysql = "select id from object where owner=$playerid";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $objid;
  ($objid) = $query->fetchrow_array;
  print "<objectid>$objid</objectid>";
  print "Object ID=$objid<br>\n";
 
  my $updatesql = "update player set obj_id=$objid where id=$playerid";
  update($updatesql); 
}


