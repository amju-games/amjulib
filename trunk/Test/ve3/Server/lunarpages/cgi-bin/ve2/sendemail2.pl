#!/usr/bin/perl -w

# Simple Email Function
# ($subject, $message)
# From http://perl.about.com/od/email/a/perlemailsub.htm

sub sendEmail
{
	my $to = 'jason.e.colman@gmail.com';
	my $from = 'no-reply@amju.com';

 	my ($subject, $message) = @_;
 	my $sendmail = '/usr/sbin/sendmail';
 	open(MAIL, "|$sendmail -oi -t ") or die "Failed to open to sendmail.";
	print MAIL "From: $from\n";
 	print MAIL "To: $to\n";
 	print MAIL "Subject: MY GAME: $subject\n\n";
 	print MAIL "$message\n";
 	close(MAIL);
} 

1;

# Example:
#  print "Content-type:text/html\n\n";
#  print "<html><body>";
#  sendEmail("Simple email.", "This is a test of the email function."); 
#  print "That got sent!</body></html>\n";

