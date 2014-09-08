#!/usr/bin/perl -w

#############################################
## SEND DIGEST EMAIL 
#############################################

require "common.pl";
sub sendDigestEmail();
my $recent_msgs_string = "";

my_connect();
sendDigestEmail();


# Call this once to query the DB and make a big string.
# Then add the same string to all mails.
# Param 
sub get_recent_messages()
{
  my $resultString = "";

  my $sql = "SELECT id, sender, UNIX_TIMESTAMP(whensent), msg, recip FROM chat WHERE candelete = 0 order by whensent desc limit 10";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute or die "Failed to execute query!?!";

  while (my ($id, $sender, $whensent, $msg, $recip) = $query->fetchrow_array)
  {
    my $decoded = pack('H*', "$msg");
    # Strip tags for trades etc
    my $pos = index($decoded, '<');
    if ( $pos > -1 ) 
    {
      $decoded = substr($decoded, 0, $pos);
    }

# TODO
# 'You sent a message' or 'You recvd a message' depending on sender/recip ID
# Sender/recip name
# Nice time
# mugshot???
# Strip XML

    my $mug = $sender;
    if ($sender < 1 && $recip > 0)
    {
      $mug = $recip;
    } 
     
    if ($mug > 0)
    {
      $resultString = $resultString . "<img src='http://www.amju.com/ve2/mugshots/mugshot-$mug.png' width=32 height=32> ";
    }
    $resultString = $resultString . "$decoded <br>\n";
  }

  return $resultString;
}



# Show player stats, AND MAKE CHANGES TO THEM!
# Param: handle, player ID, obj ID, player name
sub steal($$$$)
{
  my $handle = shift;
  my $player_id = shift;
  my $obj_id = shift;
  my $player_name = shift;

  print "Player $player_name (ID: $player_id):<br>";

  # Get last time this player logged in.
  my $time_since = 0;
  my $sql = "select unix_timestamp(now()) - unix_timestamp(max(start)) from session where player_id=$player_id";
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  my $seconds_since_login = 0;
  if (($seconds_since_login) = $query->fetchrow_array)
  {
    $time_since = $seconds_since_login;
  }

  # If you don't log in for this long, you start to get stuff stolen
  my $stealTime = 60 * 60 * 24 * 3; # 3 days 
  my $doSteal = ($time_since > $stealTime);
  print "Time since last log in: $time_since seconds! Steal?: $doSteal <br>";
  
  # If Player has not logged in lately, steal their stuff!

  my $treasure = 0; # Get how much treasure this player has
  $sql = "select `val` from objectstate where `key`='num-treasure' and id=$obj_id";
  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  if (my($found_treasure) = $query->fetchrow_array)
  {
    $treasure = $found_treasure;
  }
  print $handle "You have got $treasure treasure!<br>\n";
  print "$player_name has got $treasure treasure!<br>\n";

  if ($doSteal)
  {
    # Random amount to be stolen (1 or 2?)
    # Make sure we don't steal more than player has
    my $max_rand = $treasure / 2; # wow, harsh
    my $stolen = int(rand($max_rand));

    print "Steal $stolen treasure from $player_name<br>\n";

    if ($stolen > 0)
    {
      # Message
      my $remaining = $treasure - $stolen;
      update ("update objectstate set `val`='$remaining' where `key`='num-treasure' and id=$obj_id");
      print $handle "Oh no, I am very sorry to tell you this, but $stolen of your treasure has been STOLEN by PIRATES! Arrrr!!";
      if ($remaining == 0)
      {
        print $handle " You now have no treasure!\n<br><br>";
      }
      else
      {
        print $handle " You now have only $remaining treasure!\n<br><br>";
      }
    }
    else
    {
      print $handle "You are lucky, no one has stolen any of your treasure!<br>\n";
    }
  }

  # Dec health
  my $health = 0;
  $sql = "select `val` from objectstate where `key`='health' and id=$obj_id";
  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  if (my($found_health) = $query->fetchrow_array)
  {
    $health = $found_health;
  }
  print $handle "You have got $health health!<br>\n";

  print "$player_name has got $health health!<br>\n";

  if ($doSteal)
  {
    $max_rand = $health / 2; # wow, harsh
    $stolen = int(rand($max_rand));

    print "Steal $stolen health from $player_name<br>\n";
    if ($stolen > 0)
    {
      print $handle "You are feeling unwell today and have lost $stolen health! I think you should log in to the game and eat some food to get more health!<br>\n";
      my $remHealth = $health - $stolen;
      update ("update objectstate set `val`='$remHealth' where `key`='health' and id=$obj_id");
    }
    else
    {
      ##print $handle "You are feeling OK today, with $health health points!<br>\n";
    }
  }

  # FOOD
  my $food = 0;
  $sql = "select `val` from objectstate where `key`='food-stored' and id=$obj_id";
  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  if (my($found_food) = $query->fetchrow_array)
  {
    $food = $found_food;
  }
  print $handle "You have got $food food!<br>\n";
  print "$player_name has got $food food!<br>\n";

  if ($doSteal)
  {
    $max_rand = $food / 2; # wow, harsh
    $stolen = int(rand($max_rand));

    print "Steal $stolen food from $player_name!<br>\n";

    if ($stolen > 0)
    {
      my $remFood = $food - $stolen;
      print $handle "Some of your food went off - now you have got only $remFood food!<br>\n";
      update ("update objectstate set `val`='$remFood' where `key`='food-stored' and id=$obj_id");
    }
  }
}

# Send game info to player, as an incentive to log in and play more.
# Param: player ID, player name, obj ID, email
sub digest($$$$)
{
  my $playerid = shift;
  my $playername = shift;
  my $obj_id = shift;
  my $email = shift;

  # Send email to this player. 
  my $sendmail = '/usr/sbin/sendmail';
  if (!open(MAIL, "|$sendmail -t ") )
  {
    print "Failed to open to sendmail.";
    return;
  }

  print MAIL "From: jason.colman\@port.ac.uk\n";
  print MAIL "To: $email\n";
  print MAIL "Subject: What's been happening in Hungry People\n";
  print MAIL "Content-type: text/html\r\n\r\n";
  print MAIL "<html><head/><body>";

  print MAIL "<img src='http://www.amju.com/ve2/mugshots/mugshot-$obj_id.png' width=64 height=64><br>";
  print MAIL "Hello $playername! Thanks for participating in my research project! Here is what has been happening in the game today.<br>";
    
  my $time = 1;

  steal(MAIL, $playerid, $obj_id, $playername);

  # Add most recent messages
  # TODO
  print MAIL "<br><br>Most recent activity:<br><br>\n"; 
  print MAIL "$recent_msgs_string<br>\n"; 

  print MAIL "<br>Thanks for participating in the research project! Bye!\n</body></html>\n";
  close(MAIL);

  print "All done for $playername.\n"; 
}

sub getBest()
{
  # Get player with most treasure
  my $sql = "select id, playername, obj_id from objectstate where `key`='num-treasure'";
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  if (my($player_id, $player_name, $obj_id) = $query->fetchrow_array)
  {
  } 
}

# Get max treasure:
# SELECT id, val, max(CONVERT(`val`, SIGNED INTEGER)) FROM `objectstate` where `key`='num-treasure'

sub sendDigestEmail()
{
  $recent_msgs_string = get_recent_messages();

  print "Here is the recent msgs string:<br>\n$recent_msgs_string<br>\n";

  my $sql = "select id, playername, obj_id, email from player";
  my $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute or die "Failed to execute query: $sql";
  while (my($player_id, $player_name, $obj_id, $email) = $query->fetchrow_array)
  {
    print "Send email to: $email....<br>\n";

    digest($player_id, $player_name, $obj_id, $email);
  }
}


