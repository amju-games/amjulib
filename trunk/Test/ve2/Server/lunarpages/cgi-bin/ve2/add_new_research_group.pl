#!/usr/bin/perl -w

#############################################
## ADD NEW RESEARCH GROUP 
#############################################

require "common.pl";

my_connect();

sub add_new_rg();
add_new_rg();


##### end of main function

sub add_activity_week($$$$)
{
  my $id = shift;
  my $startdate = shift;
  my $firstday = shift;
  my $activity = shift;

  for (my $i = 0; $i < 4; $i++)
  {
    my $days = $firstday + $i;
    my $ins = "insert into research_date(date_id, cog_test, play, `date`) values ($id, 0, $activity, DATE_ADD(DATE('$startdate'), INTERVAL $days DAY))";
    print "\n<br>$ins\n";
    insert($ins);
    $id++;
  }
}

sub add_new_rg()
{
  # name of group, start date.

  my $groupname = param('groupname') or die "No group name";
  # Check group name is unique

  my $startdate = param('SnapHost_Calendar') or die "No start date";
  # NB how to get dates from Html to here? Ideally as time stamp


  print "Hello! You would like to create group '$groupname' starting on '$startdate'.\n";

  my $querysql = "select count(*) from research_group";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $numrows;
  ($numrows) = $query->fetchrow_array;
  my $id = $numrows * 100;
  print "..there are $numrows rows in research_group, so new date ID is $id";

  my $insgroup = "insert into research_group (research_group, start_date, end_date) values ('$groupname', $id, $id + 99)";
  insert($insgroup);

  # Add rows to research_date, one per day game should be played
  # Cog Test only - should be every Friday
  for (my $i = 0; $i < 7; $i++)
  {
    my $thisid = $id + 10 * $i + 9;
    my $ins = "insert into research_date(date_id, cog_test, play, `date`) values ($thisid, 1, 0, DATE_ADD(DATE('$startdate'), INTERVAL $i WEEK))";
    print "\n<br>$ins\n";
    insert($ins);
  }
  # Week 1 - no game

  # Week 2 - single player
  print "\n<br>Week 2 - single player\n";
  # 1 for single player, starts 10th day into expt
  add_activity_week($id + 20, $startdate, 10, 1); 

  # Week 3 - multi player
  print "\n<br>Week 3 - multi player\n";
  # 2 for multi player, starts 17th day into expt
  add_activity_week($id + 30, $startdate, 17, 2); 
 
  # Week 4 - no game
  
  # Week 5 - single player again
  print "\n<br>Week 5 - single player\n";
  # 1 for single player, starts 31st day into expt
  add_activity_week($id + 50, $startdate, 31, 1); 

  # Week 6 - multi player again
  print "\n<br>Week 6 - multi player\n";
  # 2 for multi player, starts 38th day into expt
  add_activity_week($id + 60, $startdate, 38, 2); 
}


