#!/usr/bin/perl -w

my $versionDotH = $ARGV[0];
open(VERSION_DOT_H, $versionDotH) or die "Please specify Version.h";

my $major = "0";
my $minor = "0";

while (<VERSION_DOT_H>)
{
  chomp;

  if ($_ =~ /MAJOR/)
  {
    my @arr = split(' ', $_);
    $major = $arr[2];
    #print "Found major: $major\n";
  }

  if ($_ =~ /MINOR/)
  {
    my @arr = split(' ', $_);
    $minor = $arr[2];
    #print "Found minor: $minor\n";
  }
}

print "v.$major.$minor\n";
