#!/usr/bin/perl -w

#############################################
## SHOW PLAYER MESSAGES RECV
## Creates a .csv file for each player with
##  message info 
#############################################

require "common.pl";

my_connect();

sub showsessions($$);
sub showresults();

showresults();


##### end of main function

sub showsessions($$)
{
  # Slightly complicated because chat table uses object IDs, not player IDs, sigh

  my $player_id = shift;
  my $obj_id = shift;

  # public_html/ve2/results
  my $dir = "../../ve2/results/";
  my $filename = $dir . "player-$player_id-messages-recv.csv";

  open (OUTFILE, ">$filename") or die "Failed to open output file $filename";
  print "Writing to file $filename<br>\n";

  print OUTFILE "Date, Yes, Sender(Player ID), Recip(Player ID), MsgId, MsgText\n";

  my $sql = "SELECT whensent, player.id, chat.id, msg FROM chat, player WHERE recip=$obj_id  AND whensent > DATE('2014-05-08') AND player.obj_id=chat.sender ORDER BY chat.id";

#  my $sql = "select s.id, player_id, start, expires, expires-start, playername, email from session as s, player as p where s.player_id=p.id and p.id=$this_player and start > DATE('2014-05-08')";

#  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  print "<p style=\"font-family:Arial\">";
  print "<b>Messages</b><br><br>\n<table border=1>";

  print "<b> <tr><td>Date</td> <td>Sender</td> <td>Recip</td> <td>MsgId</td> <td>MsgText</td> </tr> </b>\n";

  while (my ($whensent,  $sender, $msgid, $msgtext) = $query->fetchrow_array)
  {
    my $decoded = pack('H*', "$msgtext");

    print "<tr>";
    print "<td>$whensent</td> <td>$sender</td> <td>$player_id</td> <td>$msgid</td> <td>$decoded</td> ";
    print "</tr>\n";

    print OUTFILE "$whensent, 1, $sender, $player_id, $msgid, \"$decoded\"\n";
  }
  print "</table>\n";

  close(OUTFILE);
}

sub showresults()
{
  my $sql = "SELECT id, obj_id, playername, email from player";
  if (not $dbh) { print "Oh no, dbh is undefined!?!?!\n"; }
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";

  $query->execute;
  while (my ($player_id, $obj_id, $playername, $email) = $query->fetchrow_array)
  {
    print "Player ID: $player_id Obj ID: $obj_id Name: $playername Email: $email<br>\n";

    showsessions($player_id, $obj_id);
    print "\n<br><br>\n";
  }
}

