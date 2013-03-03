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

  update_or_insert($sql);

  # Set (key, value) for this player so other players will know to
  #  retrieve new messages
  $sql = "insert into objectstate (id, `key`, val, whenchanged) values ($sender, 'last_msg_sent', LAST_INSERT_ID(), now() ) on duplicate key update val=LAST_INSERT_ID(), whenchanged=now()";

  update_or_insert($sql);
}


