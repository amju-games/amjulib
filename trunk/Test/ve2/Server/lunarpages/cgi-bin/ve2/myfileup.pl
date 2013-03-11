#!/usr/bin/perl -w
# From http://www.sitepoint.com/uploading-files-cgi-perl

require "common.pl";

use strict;
use CGI qw(:standard);
use CGI;
use CGI::Carp qw(fatalsToBrowser);
use HTML::Entities ();
use File::Basename;

sub myfileup_impl()
{
  my $name_in_form = "filename";
  my $directory = param('dir');

  # NB This path is new for lunarpages. 
  #  public_html/ve2/assets/
  my $upload_dir = "../../ve2/assets/$directory/";
 
  #unless(-e $upload_dir or mkdir $upload_dir) 
  #{
  #  die "Unable to create dir $directory ($upload_dir)\n";
  #}

  # Will fail after first time, but that's ok 
  mkdir $upload_dir;

  $CGI::POST_MAX = 1024 * 500; 
  my $safe_filename_characters = "a-zA-Z0-9_.-"; 
  my $query = new CGI; 
  my $filename = $query->param($name_in_form); 
  if (!$filename) 
  {  
    print "There was a problem uploading your file, no filename"; 
    exit; 
  } 

  my ($name, $path, $extension) = fileparse ($filename, '\..*'); 
  $filename = $name . $extension; 
  $filename =~ tr/ /_/; 
  $filename =~ s/[^$safe_filename_characters]//g; 
  if ( $filename =~ /^([$safe_filename_characters]+)$/ ) 
  { 
    $filename = $1; 
  } 
  else 
  { 
    die "Filename contains invalid characters"; 
  } 

  my $pathfilename = $upload_dir . $filename;

  # Check if file already exists - if so, update DB table of updated files
  if (-e $pathfilename)
  {
    # Only need to connect to DB in this case -- assume connected ?
    my_connect2();
    my $sql = "insert into fileupdate (filename, whenchanged) values ('$directory/$filename', now()) on duplicate key update whenchanged=now()";
    update_or_insert($sql);
  }

  #my $upload_filehandle = $query->param('data'); 
  #open ( UPLOADFILE, ">$pathfilename" ) or die "$!"; 
  #binmode UPLOADFILE; 
  #while ( <$upload_filehandle> ) 
  #{ 
  #  print UPLOADFILE; 
  #} 
  #close UPLOADFILE; 

  my $data = param('data') || die ("Expected file data!!");

  open (MYFILE, ">$pathfilename") || die ("Can't open file $pathfilename");
  print "Got image data: ", length($data), " bytes. Writing to file $pathfilename..\n";
  print MYFILE $data;
  close(MYFILE);

}

#my_connect();
# Can't clean input using starthere(), it will clean the file data!
# TODO Use form 
# http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1

print "Content-type:text/html\n\n";
print "<?xml version=\"1.0\">\n";

#Don't connect unless we have to.
#my_connect2();

myfileup_impl();
print "<now>1</now> Successfully uploaded!";
# Seems a bit unnecessary, what with the limited number of connections
#check_session();

