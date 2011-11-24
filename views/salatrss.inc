<?php
   header("Content-Type: text/xml");

   $param = "q=$location";
   //<pubDate>$pubdate</pubDate>
   print <<<RSSHEADER
<?xml version="1.0" encoding="utf-8"?>
   <rss version="2.0">
   <channel>
      <title>PrayerTimes for $location</title>
      <link>http://salah.com</link>
      <description>provided by salah.com</description>

RSSHEADER;

   $i=0;
   $pubdate = date('r');
   foreach ($data as $val => $time){
      $i++;
      print <<<RSSDATA
      <item>
         <title>$val</title>
         <description>$time</description>
         <link>http://salah.com?$param&amp;time=$i</link>
      </item>

RSSDATA;
   }
   print "</channel>\n</rss>";
