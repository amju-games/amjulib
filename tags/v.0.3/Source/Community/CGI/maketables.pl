#!/usr/bin/perl -w

require "common.pl";

my_connect();
print "Making tables...\n";
sub create_tables();
create_tables();

#foreach $key (sort(keys %ENV)) {
#    print "$key = $ENV{$key}<br>\n";
#}

print "That's it!\n";

##### end of main function

sub create_table_player()
{
  my $sql = <<END;

CREATE TABLE `player`
(`id` INT NOT NULL AUTO_INCREMENT,
`playername` VARCHAR(30) NOT NULL,
`sig` VARCHAR(20) NOT NULL,
`level` VARCHAR(30) NOT NULL,
`thumbnail` VARCHAR(30) NOT NULL,
`rating` INT NOT NULL,
`lasttime` TIMESTAMP,
PRIMARY KEY (`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created table! $sql\n";
}

sub create_tables()
{
  print "Creating tables for your community game!\n";

#  drop_table("apz_zoo");
#  create_table_zoo();

  drop_table("player");
  create_table_player();
}


