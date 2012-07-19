#!/usr/bin/perl -w

#############################################
## SEND MSG REQUEST 
#############################################

require "common.pl";

my_connect();

sub sendmsg();
sendmsg();


##### end of main function

sub sendmsg()
{
  if (check_session() == 0)
  {
    return;
  }

  my $recip = param('recip') or die "No recipient";
  my $msg = param('msg') or die "No msg";
  my $sender = param('sender') or die "No sender";

  my $sql = "insert into chat (recip, sender, candelete, whensent, msg) values ($recip, $sender, 0, now(), '$msg') on duplicate key update id = id + 1";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

}


