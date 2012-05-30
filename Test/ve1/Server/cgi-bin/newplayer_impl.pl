#!/usr/bin/perl -w

#############################################
## NEW PLAYER IMPL
#############################################

require "common.pl";

sub new_player_impl($$$)
{
  my $playername = shift;
  my $email = shift;
  my $haspw = shift;

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

  $insertsql = "insert into object (type, assetfile, datafile, owner, createtime) values ('player', 'player-assets.txt', 'player-data.txt', $playerid, now())";
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

#  my $setnamesql = "insert into objectstate (id, 'key', val, whenchanged) values ($objid, 'name', '$playername', now())";

  my $setnamesql = "insert into objectstate values ($objid, 'name', '$playername', now()) on duplicate key update val='$playername', whenchanged=now()";
  insert($setnamesql);

  # TODO Set values for avatar
  my $sql = "insert into objectstate values ($objid, 'type', 1, now())"; # TODO TEMP TEST
  insert($sql);
  $sql = "insert into objectstate values ($objid, 'tex', 1, now())"; # TODO TEMP TEST
  insert($sql);

  # TODO Set location and pos
  $sql = "insert into objectpos (`id`, `x`, `y`, `z`, `loc`) values ($objid, 0, 0, 0, 1)";
  insert($sql);
}


