#!/usr/bin/perl -w

#############################################
## SEND MSG REQUEST 
#############################################

require "common.pl";
require "publishchanges.pl";

my_connect();

sub sendmsg();
sendmsg();


##### end of main function

sub sendEmail($$$)
{
  my $recip = shift;
  my $sender = shift;
  my $msg = shift;

  my $querysql = "select email, playername from player where obj_id=$recip";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $email = "jason.e.colman\@gmail.com"; 
  my $playername = "Email look up failed for Recip ID: $recip";
  ($email, $playername) = $query->fetchrow_array;

  $querysql = "select playername from player where obj_id=$sender";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;
  my $sendername = "some guy";
  ($sendername) = $query->fetchrow_array;

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
  print MAIL "Dear $playername, thanks for participating in my research game! You got a message from $sendername! Please log in to the game to see it!";

}

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

  if ($recip >= 0 && $sender >= 0)
  {
    sendEmail($recip, $sender, $msg);
  }


  update_or_insert($sql);

  # Set (key, value) for this player so other players will know to
  #  retrieve new messages: if there's a valid recip id, set row for the recip.
  # If there's a valid sender ID, set for the sender.
  if ($recip > 0)
  {
    $sql = "insert into objectstate (id, `key`, val, whenchanged) values ($recip, 'last_msg_sent', LAST_INSERT_ID(), now() ) on duplicate key update val=LAST_INSERT_ID(), whenchanged=now()";

    insert($sql);
  }

  if ($sender > 0)
  {
    $sql = "insert into objectstate (id, `key`, val, whenchanged) values ($sender, 'last_msg_sent', LAST_INSERT_ID(), now() ) on duplicate key update val=LAST_INSERT_ID(), whenchanged=now()";

    insert($sql);
  }
  else
  {
    $sql = "update objectstate set val=LAST_INSERT_ID(), whenchanged=now() where `key`='last_msg_sent'";

    update($sql);
  }

  publishRecentChanges();
}


