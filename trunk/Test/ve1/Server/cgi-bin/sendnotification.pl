#!/usr/bin/perl -w

### Try sending a notification ###

use LWP::Simple;

require "credentials.pl";

# Send notification containing player name and ID
sub notifyLogin($$)
{
  my $playerName = shift;
  my $playerId = shift;

  $requestURL = sprintf("https://prowlapp.com/publicapi/add?apikey=%s&application=%s&event=%s&description=%s",
    $apikey,
    "My%20Game",
    "Player%20login",
    "Player%20$playerId%20($playerName)%20just%20logged%20in!" );

  my $content = get($requestURL) or die "get failed :-(";

  #print $content;
}

1;

