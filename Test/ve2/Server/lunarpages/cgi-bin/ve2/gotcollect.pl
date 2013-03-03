#!/usr/bin/perl -w

#############################################
## GOT COLLECT 
#############################################

require "common.pl";

my_connect();

sub got_collect();
got_collect();


##### end of main function

sub got_collect()
{
  if (check_session() == 0)
  {
    return;
  }

  my $player_obj_id = param('player_obj_id') or die "No player obj ID";

  my $special_id = param('special_id') or die "No special ID";
  # TODO verify ID, use it to work out what player collected

  my $sql = "insert into objectstate values ($player_obj_id, 'stamina', '1', now()) on duplicate key update val=val+1, whenchanged=now()";

  update_or_insert($sql);

}


