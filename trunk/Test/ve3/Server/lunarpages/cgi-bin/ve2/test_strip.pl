#!/usr/bin/perl -w

  my $string = "Some test<some markup><a<b>>";
  print $string;
  print "\n";

  my $pos = index($string, '<');
  if ( $pos > -1 ) 
  {
    $string = substr($string, 0, $pos);
  }

  print $string;
  print "\n";


