#!/usr/bin/perl -w

#####################################################################
# Common subs/DB connect code 
#####################################################################

use strict;
use CGI qw(:standard);
use DBI;
use CGI::Carp qw(fatalsToBrowser);

our $dsn;
our $username;
our $pass;

require "credentials.pl";


# Global database connection
# OUR - not MY - so we can access it from files which "require" this one
our $dbh;

print "Content-type:text/html\n\n";
print "<?xml version=\"1.0\">\n";

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
  print "Insert successful: '$sql'\n";
}

sub update($)
{
  my $sql = shift;
  update_or_insert($sql);
  print "Update successful: '$sql'\n";
}

sub drop_table($)
{
  my $table_name = shift;

  my $sql = <<END;
DROP TABLE IF EXISTS `$table_name`;
END

  update_or_insert($sql);
}

sub my_connect()
{  
  $dbh = DBI->connect($dsn, $username, $pass);

  if (not $dbh)
  {
    die "Failed to connect to DB.\n";
  }
#  print "Connected to DB!\n";
}

1;

