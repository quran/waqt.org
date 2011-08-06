create table timezoneCache(
   geohash varchar(15) not null,
   latitude double not null,
   longitude double not null,
   timezone varchar(50) not null,
   raw_offset int not null,
   dst_offset int not null,
   gmt_offset int not null,
   primary key(geohash)
) default character set 'utf8';

create table geocodeCache(
   query varchar(255) not null,
   latitude double not null,
   longitude double not null,
   address varchar(255) not null,
   source int not null default 0,
   primary key(query)
) default character set 'utf8';
