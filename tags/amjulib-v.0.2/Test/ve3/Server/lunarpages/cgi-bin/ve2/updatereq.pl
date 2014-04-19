#!/usr/bin/perl -w

#############################################
## UPDATE OBJECT STATE REQUEST 
#############################################

require "common.pl";
require "publishchanges.pl";

sub updatestate();

my_connect();
updatestate();


##### end of main function

sub updatestate()
{
  if (check_session() == 0)
  {
    return;
  }

  my $key = param('key') or die "No key";
  my $val = param('val'); # could be zero!
  my $obj_id = param('obj_id') or die "No ID";

  my $sql = "insert into objectstate values ($obj_id, '$key', '$val', now()) on duplicate key update val='$val', whenchanged=now()";
  update_or_insert($sql);

  publishRecentChanges();
}

