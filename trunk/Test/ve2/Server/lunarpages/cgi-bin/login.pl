#!/usr/bin/perl -w

#############################################
## LOGIN
#############################################

require "common.pl";
require "sendnotification.pl";
require "sendemail2.pl";

# This is the minimum client version which you must have to be able to log in.
my $latestMajor = "0";
my $latestMinor = "7";
my $startloc = 57;

my_connect();

sub login();
login();


##### end of main function

sub init_start_pos($)
{
  my $objid = shift;

  # Set location and pos - this relies on there being an empty square at the start location for new players
  #  to materialise in.
  my $x = rand(500) - 250; # TODO Be sure this square area exists in the location mesh!!!
  my $z = rand(500) - 250;

  my $val = "$x,0,$x,$startloc";
  $sql = "insert into objectstate (`id`, `key`, `val`, `whenchanged`) values ($objid, 'pos', '$val', now()) on duplicate key update val='$val', whenchanged=now()";
  insert($sql);
}


# For research project, send info about research session
sub add_research_element($$)
{
  my ($player_id, $new_session_id) = @_;

  my $period = 60 * 60 * 24 * 7;

  print "<research>";

  # SQL to get the last research session, if there has been one
  my $sql = "select now(), s.id, s.start, rs.session_num, rs.phase, rs.session_in_phase  from session as s, research_session as rs where s.player_id=$player_id and s.id=rs.session_id order by s.start desc limit 1";

  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute;
  if (my ($now, $session_id, $start, $r_session_num, $phase, $session_in_phase) = $query->fetchrow_array)
  {
    # There has been at least one session. Time for next research session ? All sessions done ?
    my $is_r_s = 0;
    if ($now > $start + $period)
    {
      $is_r_s = 1;
    }
    print "<is_research_session>$is_r_s</is_research_session>\n";
    print "<session_num>$r_session_num + 1</session_num>\n"; # 1-based
    print "<mode>0</mode>\n"; # No game, set baseline
    print "<cog_test>TODO</cog_test>\n";

    # Set game mode (phase): no game, single, or multi
    # Set session within this phase
  }
  else
  {
    # No previous sessions, this is the first.
    print "First research session!\n";
    print "<is_research_session>1</is_research_session>\n";
    print "<session_num>1</session_num>\n"; # 1-based
    # Mode: 0 = No game; 2 = Multi player mode. (No single player mode)
    print "<mode>0</mode>\n"; 
    print "<cog_test>TODO</cog_test>\n";
  }

  # Get calendar info
  print "<dates>";
  $sql = "select UNIX_TIMESTAMP(d.date), d.cog_test, d.play  from  player as p, research_group as g, research_date as d where p.research_group=g.research_group and d.date_id >= g.start_date and d.date_id <= g.end_date and p.id=$player_id";
  $query = $dbh->prepare($sql) or die "Query prepare failed for this query: $sql\n";
  $query->execute;
  while (my ($timestamp, $cog_test, $play) = $query->fetchrow_array)
  {
    print "<date>";
    print "<timestamp>$timestamp</timestamp><cogtest>$cog_test</cogtest><play>$play</play>";
    print "</date>";
  }
  print "</dates>";

  print "</research>";
}

sub cleanup_inactive_players()
{
  # Mark any inactive players as logged out
  # TODO Here inactive means no change in position for > 1 hour, and logged in over an hour ago
  # (TODO Tweak these)
  my $sql = "update objectstate set `val`='n' where `key`='loggedin' and id in ( select distinct a.obj_id from player as a, session as b, objectpos as c where a.obj_id=c.id and a.id=b.player_id and TIMESTAMPDIFF(SECOND, c.whenchanged, now()) > 3600 and  TIMESTAMPDIFF(SECOND, b.start, now()) > 3600 )";

  update($sql);

  # TODO - this could be broken. Odd start/expires timestamps in session table :-(

  # TODO Also end the session(s) - get session ID for each player(s) we are logging out
#  $sql = " update session set `expires`=now() where   TIMESTAMPDIFF(SECOND, start, now()) > 3600  and player_id in ( select distinct a.id from player as a, objectpos as c where a.obj_id=c.id and TIMESTAMPDIFF(SECOND, c.whenchanged, now()) > 3600)";
#  update($sql);

  # Make players drop any carried objects if they are inactive (less time, because hogging items is bad)
  $sql = "update objectstate set val='0' where `key`='pickup' and val in (select distinct a.obj_id from player as a, objectpos as c where a.obj_id=c.id and TIMESTAMPDIFF(SECOND, c.whenchanged, now()) > 60 * 5 )";
  update($sql);

}

sub login()
{
  my $email = param('email') or die "Expected email\n";
  $email = lc($email);  # make lower case

  # Buggy - logs out people who have just logged in, argh!
  #cleanup_inactive_players();

  my $clientver = param('clientver');
  if (!$clientver)
  {
    $clientver = "v.0.2";
  }

print "Email: \"$email\"\n";
print "Client ver: $clientver\n";

  # Check version against the current version (TODO have latest version number in a file)
  if ($clientver =~ /v\.(\d+)\.(\d+)/)
  {
    my $major = "$1";
    my $minor = "$2"; 

    print "Major: $major, Minor: $minor\n";

    # Check major/minor version numbers, error if this client version is too old
    print "LATEST Major: $latestMajor, Minor: $latestMinor\n";

    if ($major < $latestMajor || $major == $latestMajor && $minor < $latestMinor)
    {
      # Client too old
      my $t = time();
      print "<now>$t</now><error>BAD_CLIENT: Sorry, it looks like you need to download the latest version of the game. There are some spiffy new features which unfortunately won't work with the version you have.<error>\n";

      sendEmail("Login: bad client", "Hi, login.pl here. $email just tried to log in but has an old client (version: $clientver).\nServer time is: $t\nBye!");
      
      notifyProwl("BAD client", "Email: $email Version: $clientver");
      return 1;
    }
    else
    {
      print "Your client version is up to date.\n";
    }
  }
  else
  {
    print "Don't understand your client version.\n";
  }

  my $sql = "select id from player where email='$email'";

print "Query: $sql\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  if (my ($player_id) = $query->fetchrow_array)
  {
    # DON'T Delete any old sessions,  but DO make sure they are marked as expired
    $sql = "update session set expires=now() where player_id=$player_id and expires > now()";
    update($sql);

    # Create new session
    # Session lasts 24 hours..?
    $sql = "insert into session (player_id, start, expires) values ($player_id, now(), FROM_UNIXTIME(UNIX_TIMESTAMP(now()) + 3600*24)) on duplicate key update id=id+1";
    insert($sql);

    # Get session ID for this player and start location
    $sql = "select now(), a.id, b.playername, b.obj_id, c.loc from session as a, player as b, objectpos as c where a.player_id=$player_id and a.player_id=b.id and b.obj_id=c.id and a.id=LAST_INSERT_ID()";

print "Query: $sql\n";


    $query = $dbh->prepare($sql) or die
      "Query prepare failed for this query: $sql\n";
    $query->execute;
    if (my ($now, $session_id, $playername, $objid, $loc) = $query->fetchrow_array)
    {
print "Your new session ID: $session_id\n";

      # Reset to start loc
      print "<now>$now</now> <session>$session_id</session><playername>$playername</playername><objid>$objid</objid><loc>$startloc</loc>\n";
      init_start_pos($objid);

      # Add info for research project: is this a research session, which game mode, etc.
      add_research_element($player_id, $session_id);
   
      # Set logged in flag as object value 
      $sql = "insert into objectstate (`id`, `key`, `val`) select player.obj_id, 'loggedin', 'y' from player, session where player.id = session.player_id and session.id=$session_id on duplicate key update val='y'";
      insert($sql);

      # Notify admin of login
      notifyProwl("Successful login", "Player $playername ($player_id) just logged in!");
    }
    else
    {
      print "Errr.. failed to create session?!\n";
      my $t = time();
      print "<now>$t</now><error>BAD_SESSION: Well, this was not expected. Not at all. This could perhaps be due to the database being hammered too much. </error>\n";
    
      sendEmail("Login: bad session", "Hi, login.pl here. $email just tried to log in got a BAD_SESSION error! Well, bye!");
      notifyProwl("Bad session error", "BAD_SESSION error for $email!");
    }
  }
  else
  {
    my $t = time();
    print "<now>$t</now><error>BAD_EMAIL: Sorry, I didn't recognise your email address. Please could you check for spelling mistakes. Maybe you have more than one email address, and I have a different one in my database to the one you just used. If it's any consolation, Jason gets texted and emailed when this happens, so he will be on the case soon.</error>\n";

    sendEmail("Login: bad email", "Hi, login.pl here. Someone just tried to log in with this email address and it wasn't found in the player table.\n$email\nServer time is: $t\nBye!");
      
    notifyProwl("BAD login", "Bad email: $email");
  }
}


