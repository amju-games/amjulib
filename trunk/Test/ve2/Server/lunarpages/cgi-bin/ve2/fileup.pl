#!/usr/bin/perl -w

require "fileup_impl.pl";

use strict;
use CGI qw(:standard);
use CGI;
use CGI::Carp qw(fatalsToBrowser);
use HTML::Entities ();
use File::Basename;

fileup_impl("filename");
print "NICE ONE THANKS\n";

