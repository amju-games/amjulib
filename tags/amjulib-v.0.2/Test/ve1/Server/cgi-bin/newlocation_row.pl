#!/usr/bin/perl -w

require "common.pl";

my_connect();

sub doit();
doit();

sub doit()
{
  if (check_session() == 0)
  {
    return;
  }

  my $loc_id = param('loc_id') or die "Expected location ID";
  my $assetFile = param('asset_file') or die "Expected asset file name for location";
  my $dataFile = param('data_file') or die "Expected data file name for location";

  # Add row to object table and objectpos
  $sql = "insert into object (type, assetfile, datafile, owner, createtime) values ('terrain', '$assetFile', '$dataFile', 0, now() ) ";
  insert($sql);

  $sql = "insert into objectpos (id, x, y, z, loc, whenchanged) values (LAST_INSERT_ID(), 0, 0, 0, $loc_id, now())";
  insert($sql);

  $sql = "update location set obj_id = LAST_INSERT_ID(), whenchanged = now() where id=$loc_id";
  update($sql);

  print "OK!\n";
}

