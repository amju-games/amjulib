#!/usr/bin/perl -w

#############################################
## NEW PLAYER
#############################################

require "common.pl";
require "newplayer_impl.pl";

my_connect();

sub make_new_player();

make_new_player();

#print "That's it!\n";

##### end of main function

sub make_new_player()
{
  my $playername = param('playername') or die "Expected playername";
  my $email = param('email') or die "Expected email";
  my $hashpw = "123"; #param('hashpw') or die "Expected hashpw";
  my $resgroup = param('rg') or die "Expected research group";

  new_player_impl($playername, $email, $hashpw, $resgroup);
}


