#!/usr/bin/perl -w

#############################################
## GET UPDATED FILES
#############################################

require "common.pl";

use POSIX();

# No need
#my_connect();

starthere();

sub showupdatedfiles();
showupdatedfiles();


##### end of main function

sub doOneDir($$)
{
  my $root = "../../ve2/assets/"; 
  my $thisDir = shift;
  my $dir = "$root/$thisDir/"; 

  my $ptime = shift;

  print "Root: $root  This: $thisDir  Full: $dir\n";

  if (not(opendir(DIR, $dir)))
  {
    print "Couldn't open dir $dir \n";
    return;
  }

  my @dir = grep { !/^\.+$/ } readdir(DIR);
  closedir(DIR);

  foreach (@dir)
  {  
    my $file = "$dir/$_";

    my $mtime = (stat($file))[9];            

    if ($mtime > $ptime)
    {

#    print "$file Last change:\t"
#      . POSIX::strftime("%Y%m%d", localtime($mtime))
#      . "\n";

      print "<file>$thisDir/$_</file>\n";
    }
  }

}

sub showupdatedfiles()
{
  # This now takes place at the start of the prog, so we are not logged in yet
  #if (check_session() == 0)
  #{
  #  return;
  #}

  my $t = time();
  print "<now>$t</now>\n";

  my $time = param('time') or die "Expected time since last check";

  my $dir = "../../ve2/assets/"; 
#~/public_html/ve2/assets/";

  print "<files>\n";
  {
    doOneDir(".", $time);
    doOneDir("rooms", $time);

  }
  print "</files>\n";

}


