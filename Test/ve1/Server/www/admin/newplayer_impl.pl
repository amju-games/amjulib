#!/usr/bin/perl -w

#############################################
## NEW PLAYER IMPL
#############################################

require "common.pl";

sub new_player_impl($$$)
{
  my $playername = shift;
  $playername = lc($playername);

  my $email = shift;
  $email = lc($email);

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

  my $insertsql = "insert into player (playername, email, hashpw, thumbnail, lasttime, research_group) values ('$playername', '$email', 'HASHPW', '', now(), 'a')";
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

  # Set values for avatar appearance
  my $sql = "insert into objectstate values ($objid, 'avatar', 'marge', now())"; 
  insert($sql);

  # Set location and pos - this relies on there being an empty square at the start location for new players
  #  to materialise in.
  my $x = rand(500) - 250; # TODO Be sure this square area exists in the location mesh!!!
  my $z = rand(500) - 250;
  my $startLoc = 57;

  print "<br><br>Start location: ($x, $z)\n";

  $sql = "insert into objectpos (`id`, `x`, `y`, `z`, `loc`) values ($objid, $x, 0, $z, $startLoc)";
  insert($sql);

  # Initially logged out
  $sql = "insert into objectstate (`id`, `key`, `val`) values($objid, 'loggedin', 'n') on duplicate key update val='n'";
  insert($sql);

  # Send email to new player. TODO Use a template so we can easily edit this.
  # TODO Send html or text ??
  my $sendmail = '/usr/sbin/sendmail';
  if (!open(MAIL, "|$sendmail -oi -t ") )
  {
    print "Added user ok, except at the end, where I failed to open to sendmail.";
    return;
  }

  print MAIL "From: jason\@amju.com\n";
  print MAIL "To: $email\n";
  print MAIL "Subject: Welcome to MY GAME!\n\n";
  print MAIL "Dear $playername,\nThanks for volunteering to participate in my experiment.\n\nTo get started, please go to www.amju.com/ve1/www\n\nIf you don't want to play this game, you can just ignore this email.\n\nBest wishes,\nJason Colman \n";
  close(MAIL);

  print "All OK, and sent email to new player.\n"; 
}


