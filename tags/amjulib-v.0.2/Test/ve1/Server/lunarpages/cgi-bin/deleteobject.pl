#!/usr/bin/perl -w

#############################################
## DELETEOBJECT 
#############################################

require "common.pl";

my_connect();

sub do_delete();
do_delete();


##### end of main function

sub do_delete()
{
  if (check_session() == 0)
  {
    return;
  }

  my $obj_id = param('obj_id') or die "No ID";

  my $sql = "delete from objectpos where id=$obj_id";
  update($sql);

  $sql = "delete from objectstate where id=$obj_id";
  update($sql);

  $sql = "delete from object where id=$obj_id";
  update($sql);

  $sql = "delete from location where obj_id=$obj_id";
  update($sql);

  # TODO Mark player as deleted
#  $sql = "delete from player where obj_id=$obj_id";
#  update($sql);

  
}


