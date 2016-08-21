#!/usr/bin/perl -w

### Try sending a notification to iOS devices with Prowl ###

use LWP::Simple;

require "credentials.pl";

# Send notification: $subject $message
sub notifyProwl($$)
{
  return; # 14 April 2016 this gives Internal Server Error

  # TODO timeout 

  my ($subject, $message) = @_;

  $requestURL = sprintf("https://prowlapp.com/publicapi/add?apikey=%s&application=%s&event=%s&description=%s",
    $apikey,
    "My%20Game",
    $subject,
    $message );

  my $content = get($requestURL); # or die "get failed :-(";
}

1;

