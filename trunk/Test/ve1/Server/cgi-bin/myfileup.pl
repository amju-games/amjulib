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

  $CGI::POST_MAX = 1024 * 500; 
  my $safe_filename_characters = "a-zA-Z0-9_.-"; 
  my $upload_dir = "../assets/"; 
  my $query = new CGI; 
  my $filename = $query->param($name_in_form); 
  if (!$filename) 
  {  
    print $query->header(); 
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

  print $query->header(); 

  # Check if file already exists - if so, update DB table of updated files
  if (-e $pathfilename)
  {
    # Only need to connect to DB in this case -- assume connected ?
    my_connect();
    my $sql = "insert into fileupdate (filename, whenchanged) values ('$filename', now()) on duplicate key update whenchanged=now()";
    update_or_insert($sql);
  }

  my $upload_filehandle = $query->param('data'); 
  open ( UPLOADFILE, ">$pathfilename" ) or die "$!"; 
  binmode UPLOADFILE; 
  while ( <$upload_filehandle> ) 
  { 
    print UPLOADFILE; 
  } 
  close UPLOADFILE; 
}

myfileup_impl();


