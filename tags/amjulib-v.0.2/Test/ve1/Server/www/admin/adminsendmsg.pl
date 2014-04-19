#!/usr/bin/perl -w

#############################################
## ADMIN SEND MSG  
#############################################

require "common.pl";

my_connect();

sub sendmsg();
sendmsg();


##### end of main function

sub sendmsg()
{
  # No session required

  my $recip = param('recip') or die "No recipient";
print "Recip: $recip\n";

  my $msg = param('msg') or die "No msg";
  # TODO Convert text to special format. Each char is represented by its 2-digit hex ascii code. This is 
  #  so we can store punctuation etc. and still sanitise script params.
  

  # Need special ID for admin user. Must be set when DB created.
  my $sender = 1; #'Admin'; #param('sender') or die "No sender";

  my $hex = unpack('H*', "$msg");

print "Msg: $msg, unpacked: $hex\n";

  my $sql = "insert into chat (recip, sender, candelete, whensent, msg) values ($recip, $sender, 0, now(), '$hex') on duplicate key update id = id + 1";

  print "Query: $sql\n\n";

  my $query = $dbh->prepare($sql) or die
    "Query prepare failed for this query: $sql\n";

  $query->execute;

print "OK, check for msg sent.\n";

}


