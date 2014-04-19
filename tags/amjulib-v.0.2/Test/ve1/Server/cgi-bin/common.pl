#!/usr/bin/perl -w

#####################################################################
# Common subs/DB connect code 
#####################################################################

use strict;
use CGI qw(:standard);
use DBI;
use CGI::Carp qw(fatalsToBrowser);
use HTML::Entities ();

our $dsn;
our $username;
our $pass;

require "credentials.pl";


# Global database connection
# OUR - not MY - so we can access it from files which "require" this one
our $dbh;

sub starthere()
{
  print "Content-type:text/html\n\n";
  print "<?xml version=\"1.0\">\n";

  # Clean user input
  # From: http://advosys.ca/papers/web/61-web-security.html
  #
  #my $ok_chars = 'a-zA-Z0-9 ,-';
  my $ok_chars = '/a-zA-Z0-9\.@_,-'; # comma is new
  foreach my $param_name ( param() ) 
  {
    $_ = HTML::Entities::decode( param($param_name) );
    #    print "Here is a param: \"$_\"....  ";
    $_ =~ s/[^$ok_chars]//go;

    #    print "Here is param now: \"$_\"\n";

    param($param_name,$_);
  }
}


#
# Useful DB functions
#
sub update_or_insert($)
{
  my $sql = shift;
  my $query = $dbh->prepare($sql) or die "Prepare failed for: '$sql'";
  $query->execute or die "Failed to execute '$sql'";
}

sub insert($)
{
  my $sql = shift;
  update_or_insert($sql);
  #print "Insert successful: '$sql'\n";
}

sub update($)
{
  my $sql = shift;
  update_or_insert($sql);
  #print "Update successful: '$sql'\n";
}

sub drop_table($)
{
  my $table_name = shift;

  my $sql = <<END;
DROP TABLE IF EXISTS `$table_name`;
END

  update_or_insert($sql);
}

sub my_connect2()
{
  $dbh = DBI->connect($dsn, $username, $pass);

  if (not $dbh)
  {
    my $t = time();
    print "<now>$t</now><error>BAD_DB_CONNECT $DBI::errstr </error>\n";
    exit(0);
#    die "Failed to connect to DB: $DBI::errstr\n";
  }
}

sub my_connect()
{  
  starthere(); # print header
  my_connect2();
}

sub disconnect()
{
  $dbh->disconnect;
}

sub check_session()
{
  my $session_id = param('session_id') or die "No session id";

  my $sql = "select id, UNIX_TIMESTAMP(now()) from session where id=$session_id and expires >= now()";
  my $query = $dbh->prepare($sql) or die "Prepare failed for: '$sql'";
  $query->execute or die "Failed to execute '$sql'";
  
  my $find_session_id = 0;
  my $time_now;

  if (($find_session_id, $time_now) = $query->fetchrow_array)
  {
    #print "Found session ID $find_session_id\n";
    print "<now>$time_now</now>\n";
    return 1;
  }
  print "BAD_SESSION: Session expired!\n";
  return 0;
}

1;

