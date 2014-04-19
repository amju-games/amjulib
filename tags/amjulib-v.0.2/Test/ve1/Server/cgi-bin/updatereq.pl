#!/usr/bin/perl -w

#############################################
## UPDATE OBJECT STATE REQUEST 
#############################################

require "common.pl";

my_connect();

sub updatestate();
updatestate();


##### end of main function

sub updatestate()
{
  if (check_session() == 0)
  {
    return;
  }

  my $key = param('key') or die "No key";
  my $val = param('val');
  my $obj_id = param('obj_id') or die "No ID";

  if ($key eq "pickup")
  {
    # Special case key:
    # We can only pick up (take ownership) of object if there is no current owner
    # TODO Check player strength here and any other restrictions
    if ($val ne "0")
    {
      # First make sure row exists. Then update it.
      my $sql = "insert ignore into objectstate values ($obj_id, '$key', 0, 1)";
      insert($sql);
      $sql = "update objectstate set val='$val', whenchanged=now() where `key`='$key' and id=$obj_id and val='0'";
      update($sql);
    }
    else
    {
      # We can only drop the object if we are the current owner.
      my $session_id = param('session_id') or die "No session id";

      my $sql = "update objectstate set val='0', whenchanged=now() where `key`='$key' and id=$obj_id and val in (select b.obj_id FROM session as a, player as b where a.player_id=b.id and a.id=$session_id) ";

      update($sql);
    }
    return;
  }

  my $sql = "insert into objectstate values ($obj_id, '$key', '$val', now()) on duplicate key update val='$val', whenchanged=now()";
  update_or_insert($sql);

}


