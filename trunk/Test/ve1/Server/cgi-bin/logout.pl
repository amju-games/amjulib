#!/usr/bin/perl -w

#############################################
## LOGOUT
## Well-behaved clients call this when user
##  wants to end session.
#############################################

require "logout_impl.pl";

my_connect();

logout();

