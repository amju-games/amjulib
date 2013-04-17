#!/usr/bin/perl -w

#############################################
## ADD NEW RESEARCH GROUP 
#############################################

require "common.pl";

my_connect();

sub add_new_rg();
add_new_rg();


##### end of main function

sub add_new_rg()
{
  # name of group, start date.

  my $groupname = param('groupname') or die "No group name";
  # Check group name is unique

  my $startdate = param('startdate') or die "No start date";
  # NB how to get dates from Html to here? Ideally as time stamp


  print "Hello! You would like to create group '$groupname' starting on '$startdate'.\n";


}


