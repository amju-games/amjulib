#!/usr/bin/perl -w

#############################################
## ADD NEW RESEARCH GROUP - v2, more flexible
#############################################

require "common.pl";

my_connect();

sub add_new_rg();
add_new_rg();


##### end of main function

sub add_activity_week($$$$$$)
{
  my $id = shift;
  my $startdate = shift;
  my $firstday = shift;
  my $activity = shift;
  my $numDaysInWeek = shift;
  my $isPreview = shift;

  for (my $i = 0; $i < $numDaysInWeek; $i++)
  {
    my $days = $firstday + $i;
    my $ins;

    $ins = "insert into research_date(date_id, cog_test, play, `date`) values ($id, 0, $activity, DATE_ADD(DATE('$startdate'), INTERVAL $days DAY))";
    
    print "\n<br>$ins\n";

    if ($isPreview == 0)
    {
      insert($ins);
    }
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

  my $numReps = param('numreps');
  my $numWeeksPattern = param('patternlength');
  my $numWeeksTotal = $numReps * $numWeeksPattern;
  my $weekSingle = param('singleweek');
  my $weekMulti = param('multiweek');
  my $numDaysInWeek = param('daysinweek');
  my $isCogTests = param('cogtests');
  if (!defined($isCogTests)) 
  { 
    $isCogTests = 0;
  }
  my $isPreview = param('preview');
  if (!defined($isPreview)) 
  { 
    $isPreview = 0;
  }

  print "Hello! You would like to create group '$groupname' starting on '$startdate'.\n";
  print "<br>Num weeks in pattern: $numWeeksPattern\n";
  print "<br>Num reps: $numReps\n";
  print "<br>Num weeks total: $numWeeksTotal\n";
  print "<br>Week in pattern for Single player: $weekSingle\n";
  print "<br>Week in pattern for Multi player: $weekMulti\n";
  print "<br>Num days in week: $numDaysInWeek\n";
  print "<br>Do cog tests every Friday: $isCogTests\n";
  print "<br>Is preview only: $isPreview\n";

  my $querysql = "select count(*) from research_group";
  $query = $dbh->prepare($querysql) or die "Prepare failed for: '$querysql'";
  $query->execute;

  my $numrows;
  ($numrows) = $query->fetchrow_array;
  my $id = $numrows * 1000;
  print "<br>There are $numrows rows in research_group, so new date ID is $id\n";

  my $insgroup = "insert into research_group (research_group, start_date, end_date) values ('$groupname', $id, $id + 999)";
  print "<br>$insgroup\n";

  if ($isPreview == 0)
  {
    insert($insgroup);
  }

  if ($isCogTests == 1)
  {
    print "<br>Adding cog test dates...\n";
    # Cog Test only - should be every Friday
    #  (<= because we want a final cog test at the end of the period.)
    for (my $i = 0; $i <= $numWeeksTotal; $i++)
    {
      my $thisid = $id + 10 * $i + 9;
      my $ins;
   
      my $days = $i * 7; 
      $ins = "insert into research_date(date_id, cog_test, play, `date`) values ($thisid, 1, 0, DATE_ADD(DATE('$startdate'), INTERVAL $days DAY))";
   
      print "\n<br>$ins\n";
  
      if ($isPreview == 0)
      {
        insert($ins);
      }
    }
  }

  my $week = 0;
  for (my $rep = 0; $rep < $numReps; $rep++)
  {
    print "<br>Repetition $rep...\n";

    for (my $weekInRep = 1; $weekInRep <= $numWeeksPattern; $weekInRep++)
    {
      $week++;
      print "<br>Week $week: ";

      if ($weekInRep == $weekSingle)
      {
        print "This is a Single player week.\n";
        add_activity_week($id + 100 + $week * 10, $startdate, 3 + 7 * ($week - 1), 1, $numDaysInWeek, $isPreview); 
      }
      elsif ($weekInRep == $weekMulti)
      {
        print "This is a Multi player week.\n";
        add_activity_week($id + 100 + $week * 10, $startdate, 3 + 7 * ($week - 1), 2, $numDaysInWeek, $isPreview); 
      }
      else
      {
        print "This is a no-game week.\n";
      }
    } 
  }
}


