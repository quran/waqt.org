<?php
   header("Content-Type: text/xml");

   $param = "q=$location";
   print <<<RSSHEADER
<?xml version="1.0" encoding="utf-8"?>
   <rss version="2.0">
   <channel>
      <title>PrayerTimes for $location</title>
      <link>http://waqt.org</link>
      <description>prayertime feeds for the whole world, provided
                   by waqt.org</description>

RSSHEADER;

   $i=0;
   $pubdate = date('r');
   foreach ($data as $val => $time){
      $i++;
      print <<<RSSDATA
      <item>
         <title>$val - $time</title>
         <description>$val is at $time.</description>
         <link>http://waqt.org?$param&amp;time=$i</link>
         <pubDate>$pubdate</pubDate>
      </item>

RSSDATA;
   }
   print "</channel>\n</rss>";
