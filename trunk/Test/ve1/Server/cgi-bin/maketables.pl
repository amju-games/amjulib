#!/usr/bin/perl -w

require "common.pl";

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
PRIMARY KEY (`id`),
FOREIGN KEY (`obj_id`) REFERENCES object(`id`)
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created player table! $sql\n<br>\n";
}

sub create_table_session()
{
  my $sql = <<END;

CREATE TABLE `session`
(`id` INT NOT NULL AUTO_INCREMENT,
`player_id` INT NOT NULL,
`start` TIMESTAMP NOT NULL,
`expires` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`player_id`) REFERENCES player(`id`)
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
`assetfile` VARCHAR(30) NOT NULL,
`datafile` VARCHAR(30) NOT NULL,
`owner` INT NOT NULL,
`createtime` TIMESTAMP,
PRIMARY KEY (`id`)
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
`val` VARCHAR(30) NOT NULL,
`whenchanged` TIMESTAMP NOT NULL,
FOREIGN KEY (`id`) REFERENCES object(`id`) 
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
`whenchanged` TIMESTAMP NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`id`) REFERENCES object(`id`) 
) ENGINE = MYISAM;
END

  update_or_insert($sql);

  print "Created object table! $sql\n<br>\n";
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

  print "<br><br>Creating initial data...<br>\n";

  my $sql;
  
  $sql = "insert into object (type, assetfile, datafile, owner, createtime) values ('player', 'player-assets.txt', 'player-data.txt', 1, now());";
  insert($sql);

  $sql = "insert into object (type, assetfile, datafile, owner, createtime) values ('terrain', 'terrain-1-assets.txt', 'terrain-data.txt', 0, now());";
  insert($sql);

  $sql = "insert into player (playername, email, hashpw, thumbnail, lasttime, obj_id) values ('jay', 'jason\@amju.com', 'HASH', '', now(), 1);";
  insert($sql);
}


