#!/usr/bin/perl -w

#############################################
## LOGIN
#############################################

require "common.pl";

my_connect();

sub login();
login();


##### end of main function

sub login()
{
  my $email = param('email') or die "Expected email\n";

print "Email: \"$email\"\n";

  my $sql = "select id from player where email='$email'";

print "Query: $sql\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

  if (my ($player_id) = $query->fetchrow_array)
  {
    # Delete old session
    $sql = "delete from session where player_id=$player_id";
print "Query: $sql\n";
    $query = $dbh->prepare($sql) or die
      "Query prepare failed for this query: $sql\n";
    $query->execute;

    # Create new session
    # Session lasts 1 hour..?
    $sql = "insert into session (player_id, start, expires) values ($player_id, now(), FROM_UNIXTIME(UNIX_TIMESTAMP(now()) + 3600)) on duplicate key update id=id+1";
    insert($sql);

    # Get session ID for this player
    $sql = "select id from session where player_id=$player_id";

print "Query: $sql\n";


    $query = $dbh->prepare($sql) or die
      "Query prepare failed for this query: $sql\n";
    $query->execute;
    if (my ($session_id) = $query->fetchrow_array)
    {
print "Your new session ID: $session_id\n";

      print "<session>$session_id</session>\n";
    }
    else
    {
      print "Errr.. failed to create session?!\n";
    }
  }
  else
  {
    print "No such email\n";
  }
}


