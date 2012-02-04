#!/usr/bin/perl -w

require "common.pl";

my_connect();
print "Making tables to store research data...\n";
sub create_tables();
create_tables();

print "That's it!\n";

##### end of main function

sub create_table_position()
{
  my $sql = <<END;

CREATE TABLE `research_objectpos`
(`id` INT NOT NULL,
`x` FLOAT NOT NULL, 
`y` FLOAT NOT NULL, 
`z` FLOAT NOT NULL, 
`loc` INT NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
FOREIGN KEY (`id`) REFERENCES object(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
}

sub create_tables()
{
  print "Creating RESEARCH tables for VE1!\n";

  drop_table("research_objectpos");
  create_table_position();

}


