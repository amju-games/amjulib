#!/usr/bin/perl -w

#############################################
## CHANGE VALUE 
#############################################

require "common.pl";

my_connect();

sub change_value();
change_value();


##### end of main function

sub change_value()
{
  if (check_session() == 0)
  {
    return;
  }

  my $player_obj_id = param('player_obj_id') or die "No player obj ID";

  my $key = param('key') or die "No key!!";
  # TODO verify ID, use it to work out what player collected

  my $change = param('change') or die "No change!!";

  my $sql = "insert into objectstate values ($player_obj_id, '$key', '0', now()) on duplicate key update val=val+$change, whenchanged=now()";

  update_or_insert($sql);

}


