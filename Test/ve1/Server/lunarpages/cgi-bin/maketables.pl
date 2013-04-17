#!/usr/bin/perl -w

require "common.pl";
require "newplayer_impl.pl";

my_connect();
print "Making tables...\n";
sub create_tables();
create_tables();

print "That's it!\n";

##### end of main function

sub create_table_player()
{
  my $sql = <<END;

CREATE TABLE `player`
(`id` INT NOT NULL AUTO_INCREMENT,
`playername` VARCHAR(30) NOT NULL,
`email` VARCHAR(100) NOT NULL,
`hashpw` VARCHAR(20) NOT NULL,
`thumbnail` VARCHAR(30) NOT NULL,
`lasttime` TIMESTAMP,
`obj_id` INT NOT NULL,
`research_group` VARCHAR(10) NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`obj_id`) REFERENCES object(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created player table! $sql\n<br>\n";
}

# TODO I just found out that 'start' was set to update to the current timestamp on every update!!
# We don't want this! -- but how is this Create Table setting such an attribute ???
sub create_table_session()
{
  my $sql = <<END;

CREATE TABLE `session`
(`id` INT NOT NULL AUTO_INCREMENT,
`player_id` INT NOT NULL,
`start` TIMESTAMP NOT NULL,
`expires` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`player_id`) REFERENCES player(`id`),

INDEX (`id`, `expires`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created player session! $sql\n<br>\n";
}

sub create_table_object()
{
  my $sql = <<END;

CREATE TABLE `object`
(`id` INT NOT NULL AUTO_INCREMENT,
`type` VARCHAR(30) NOT NULL,
`assetfile` VARCHAR(100) NOT NULL,
`datafile` VARCHAR(100) NOT NULL,
`owner` INT NOT NULL,
`createtime` TIMESTAMP,
PRIMARY KEY (`id`),

INDEX (`createtime`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
}

sub create_table_objectstate()
{
  my $sql = <<END;

CREATE TABLE `objectstate`
(`id` INT NOT NULL,
`key` VARCHAR(30) NOT NULL,
`val` VARCHAR(50) NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`, `key`),
FOREIGN KEY (`id`) REFERENCES object(`id`),

INDEX (`whenchanged`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
}

sub create_table_objectpos()
{
  my $sql = <<END;

CREATE TABLE `objectpos`
(`id` INT NOT NULL,
`x` FLOAT NOT NULL, 
`y` FLOAT NOT NULL, 
`z` FLOAT NOT NULL, 
`loc` INT NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`id`) REFERENCES object(`id`),

INDEX (`whenchanged`, `loc`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
}

sub create_table_chat()
{
  my $sql = <<END;

CREATE TABLE `chat`
(`id` INT NOT NULL AUTO_INCREMENT,
`recip` INT NOT NULL, 
`sender` INT NOT NULL, 
`candelete` TINYINT NOT NULL, 
`whensent` TIMESTAMP NOT NULL,
`msg` TEXT NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`recip`) REFERENCES object(`id`),
FOREIGN KEY (`sender`) REFERENCES object(`id`),

INDEX (`recip`, `whensent`) 
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
}

sub create_table_fileupdate()
{
  my $sql = <<END;

CREATE TABLE `fileupdate`
(`filename` VARCHAR(100) NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
PRIMARY KEY (`filename`),
INDEX (`whenchanged`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created fileupdate table! $sql\n<br>\n";
}

sub create_table_location()
{
  my $sql = <<END;

CREATE TABLE `location`
(`id` INT NOT NULL AUTO_INCREMENT,
`obj_id` INT NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
`name` VARCHAR(100),
PRIMARY KEY (`id`),
FOREIGN KEY (`obj_id`) REFERENCES object(`id`),
INDEX (`whenchanged`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created location table! $sql\n<br>\n";
}

sub create_tables()
{
  print "Creating tables for VE1!\n";

  drop_table("player");
  create_table_player();

  drop_table("session");
  create_table_session();

  drop_table("object");
  create_table_object();

  drop_table("objectstate");
  create_table_objectstate();

  drop_table("objectpos");
  create_table_objectpos();

  drop_table("chat");
  create_table_chat();

  drop_table("fileupdate");
  create_table_fileupdate();

  drop_table("location");
  create_table_location();

  print "<br><br>Creating initial data...<br>\n";

  my $sql;
  

  $sql = "insert into object (type, assetfile, datafile, owner, createtime) values ('terrain', 'terrain-1-assets.txt', 'terrain-data.txt', 0, now());";
  insert($sql);

  # TODO Add admin player character

  new_player_impl('jay', 'jason@amju.com', '123');
  new_player_impl('ella', 'ella@amju.com', '123');
  new_player_impl('josh', 'josh@amju.com', '123');

}


