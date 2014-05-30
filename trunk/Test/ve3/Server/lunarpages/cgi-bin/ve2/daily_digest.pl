#!/usr/bin/perl -w

#############################################
## SEND DIGEST EMAIL 
#############################################

require "common.pl";
sub sendDigestEmail();

my_connect();
sendDigestEmail();




sub new_player_impl($$$$)
{
  my $playername = shift;
  ###$playername = lc($playername); # why lc ??!??!

  my $email = shift;
  $email = lc($email);

  my $haspw = shift;

  my $resgroup = shift;

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

  my $insertsql = "insert into player (playername, email, hashpw, thumbnail, lasttime, research_group) values ('$playername', '$email', 'HASHPW', '', now(), '$resgroup')";
  insert($insertsql);

  my $querysql = "select id from player where email='$email'";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $playerid;
  ($playerid) = $query->fetchrow_array;
  
  print "<playerid>$playerid</playerid>";

  print "Player ID=$playerid<br>\n";

  $insertsql = "insert into object (type, assetfile, datafile, owner, createtime) values ('player', 'none', 'none', $playerid, now())";
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
#  my $sql = "insert into objectstate values ($objid, 'avatar', 'marge', now())"; 
#  insert($sql);

  # Set initial health
  my $foodsql = "insert into objectstate values($objid, 'health', '3', now())";
  insert($foodsql);
 
  # Set location and pos - this relies on there being an empty square at the start location for new players
  #  to materialise in.
  my $x = rand(500) - 250; # TODO Be sure this square area exists in the location mesh!!!
  my $z = rand(500) - 250;
  my $startLoc = 1;

  print "<br><br>Start location: ($x, $z)\n";

  $sql = "insert into objectstate values ($objid, 'pos', '$x,0,$z,$startLoc', now())";
  insert($sql);

  # Initially logged out
  $sql = "insert into objectstate (`id`, `key`, `val`) values($objid, 'loggedin', 'n') on duplicate key update val='n'";
  insert($sql);

  ###sendWelcomeEmail($email, $playername);
}

sub showmsgs($$)
{
  my $time = shift;
  my $recip = shift;

  #my $sql = "SELECT id, sender, UNIX_TIMESTAMP(whensent), msg, recip FROM chat WHERE candelete = 0 and (recip = $recip or recip = -2) and whensent >= FROM_UNIXTIME(GREATEST($time, UNIX_TIMESTAMP(DATE(NOW())))) ";

  # Any recipient - sort out in client
  my $sql = "SELECT id, sender, UNIX_TIMESTAMP(whensent), msg, recip FROM chat WHERE candelete = 0 and whensent >= FROM_UNIXTIME($time) ORDER BY whensent DESC LIMIT 50";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  print "<msgs>\n";
  while (my ($id, $sender, $whensent, $msg, $recip) = $query->fetchrow_array)
  {
    print "<msg>";
    print "<id>$id</id> <sender>$sender</sender> <whensent>$whensent</whensent> <msg>$msg</msg> <recip>$recip</recip>";
    print "</msg>\n";
  }
  print "</msgs>\n";

}



sub sendEmailToOnePlayer($$)
{
  my $email = shift;
  my $playername = shift;

  # Send email to this player. 
  # TODO Send html or text ??
  my $sendmail = '/usr/sbin/sendmail';
  if (!open(MAIL, "|$sendmail -t ") )
  {
    print "Failed to open to sendmail.";
    return;
  }

  print MAIL "From: jason.colman\@port.ac.uk\n";
  print MAIL "To: $email\n";
  print MAIL "Subject: What's been happening in Hungry People\n\n";
  print MAIL "Dear $playername,Thanks for participating in my experiment.Here is what has been happening in the game today.";

  # Get msgs relating to player.
  my $recip = 1;
  my $time = 1;
 
  # TODO Ideally show mugshots for players but this will be incredibly time consuming to do :-(

  my $sql = "SELECT id, sender, UNIX_TIMESTAMP(whensent), msg, recip FROM chat WHERE candelete = 0 and (recip = $recip or recip = -2) and whensent >= FROM_UNIXTIME(GREATEST($time, UNIX_TIMESTAMP(DATE(NOW())))) ";
  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  while (my ($id, $sender, $whensent, $msg, $recip) = $query->fetchrow_array)
  {
    print MAIL "<id>$id</id> <sender>$sender</sender> <whensent>$whensent</whensent> <msg>$msg</msg> <recip>$recip</recip>";
  }

  print MAIL "OK, bye!\n";

  close(MAIL);

  print "All done for $playername.\n"; 
}

sub digest()
{
  # Get msgs relating to player.
  my $playerid = 2;
  my $time = 1;
 
  # TODO Ideally show mugshots for players but this will be incredibly time consuming to do :-(

  my $sql = "SELECT id, sender, UNIX_TIMESTAMP(whensent), msg, recip FROM chat WHERE candelete = 0 and (recip = $playerid or recip = -2 or sender = $playerid) order by whensent desc limit 50";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  print "<table>";
  while (my ($id, $sender, $whensent, $msg, $recip) = $query->fetchrow_array)
  {
    my $decoded = pack('H*', "$msg");

# TODO
# 'You sent a message' or 'You recvd a message' depending on sender/recip ID
# Sender/recip name
# Nice time
# mugshot???
# Strip XML

    my $mug = $sender;
    if ($sender == $playerid)
    {
      $mug = $recip;
    } 
 
    print "<tr><td><img src='http://www.amju.com/ve2/assets/mugshot-$mug.png' width=64 height=64></td> <td>";
    print "MSG ID: $id SENDER: $sender SENT: $whensent MSG: $decoded RECIP: $recip\n<br>";
    print "</td></tr>";
  }

  print "</table>";
  print "OK, bye!\n";

}

sub sendDigestEmail()
{
  print "Hello!\n";
#  sendEmailToOnePlayer("jason.e.colman\@gmail.com", "jason"); # test
  digest();
}


