#!/usr/bin/perl -w

#############################################
## NEW OBJECT 
## Updates new object so it can be seen in game.
## Use newobject_id.pl first to create placeholder row.
#############################################

require "common.pl";

my_connect();

sub newobject();
newobject();


##### end of main function

sub newobject()
{
  if (check_session() == 0)
  {
    return;
  }

  my $obj_id = param('obj_id');
  my $assetfile = param('asset_file');
  my $datafile = param('data_file');

  # Get owner
  my $sql = "update object set assetfile='$assetfile', datafile='$datafile', createtime=now() where id=$obj_id limit 1";
  update($sql);

  print "<ok>OK!</ok>";
}


