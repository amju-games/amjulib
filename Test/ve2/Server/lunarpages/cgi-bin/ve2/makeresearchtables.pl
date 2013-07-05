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
  drop_table("research_objectpos");

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

  print "Created research_objectpos table! $sql\n<br>\n";
}
  
sub create_table_cogtest()
{
  drop_table("research_cogtest");
  drop_table("research_testresult");
  drop_table("research_group");
  drop_table("research_date");

  my $sql = <<END;

CREATE TABLE `research_cogtest`
(`id` INT NOT NULL AUTO_INCREMENT,
`test_type` INT NOT NULL,
`session_id` INT NOT NULL,
PRIMARY KEY (`id`),
UNIQUE KEY (`test_type`, `session_id`),
FOREIGN KEY (`session_id`) REFERENCES session(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created research_cogtest table! $sql\n<br>\n";

  $sql = <<END;

CREATE TABLE `research_testresult`
(`test_id` INT NOT NULL,
`key` VARCHAR(30) NOT NULL,
`val` VARCHAR(30) NOT NULL,
FOREIGN KEY (`test_id`) REFERENCES research_cogtest(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created research_testresult table! $sql\n<br>\n";

  $sql = <<END;
CREATE TABLE  `research_group` (
`research_group` VARCHAR( 10 ) NOT NULL ,
`start_date` INT NOT NULL ,
`end_date` INT NOT NULL
) ENGINE = MYISAM ;
END

  update_or_insert($sql);

  print "Created research_group table! $sql\n<br>\n";

  $sql = <<END;
CREATE TABLE  `research_date` (
`date_id` INT NOT NULL ,
`cog_test` INT NOT NULL ,
`play` INT NOT NULL ,
`date` DATE NOT NULL ,
PRIMARY KEY (  `date_id` ) 
) ENGINE = MYISAM ;
END

  update_or_insert($sql);

  print "Created research_date table! $sql\n<br>\n";

  $sql = <<END;
CREATE TABLE  `research_group_current` (
`research_group` VARCHAR( 10 ) NOT NULL,
PRIMARY KEY(`research_group`)
) ENGINE = MYISAM ;
END

  update_or_insert($sql);

  print "Created research_group_current table! $sql\n<br>\n";
}

sub create_table_action()
{
  # Tables to record all player actions - this may be overkill..

  drop_table("research_action"); # one row per player-initiated action
  drop_table("research_action_type"); # describes action types
  drop_table("research_action_info"); # rows for additional info for actions

  my $sql = <<END;

CREATE TABLE `research_action_type`
(`id` INT NOT NULL,
`name` VARCHAR(30) NOT NULL,
PRIMARY KEY (`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  $sql = <<END;

CREATE TABLE `research_action`
(`id` INT NOT NULL,
`actiontype` INT NOT NULL,
`playerid` INT NOT NULL,
`when` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`playerid`) REFERENCES player(`id`),
FOREIGN KEY (`actiontype`) REFERENCES research_action_type(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  $sql = <<END;

CREATE TABLE `research_action_info`
(`actionid` INT NOT NULL,
`key` VARCHAR(30) NOT NULL,
`val` VARCHAR(30) NOT NULL,
FOREIGN KEY (`actionid`) REFERENCES research_action(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);
}

sub create_table_session()
{
  # Research sessions
  drop_table("research_session"); # one row per research session (currently weekly) per player

  my $sql = <<END;

CREATE TABLE `research_session`
(`session_id` INT NOT NULL,
`session_num` INT NOT NULL,
`phase` INT NOT NULL,
`session_in_phase` INT NOT NULL,
FOREIGN KEY (`session_id`) REFERENCES session(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created research_testresult table! $sql\n<br>\n";
}

sub create_tables()
{
  print "Creating RESEARCH tables for this environment!\n";

  create_table_position();

  create_table_cogtest();

#  create_table_action();

#  create_table_session();
}


