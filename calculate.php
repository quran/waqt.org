<?php
include 'settings.inc';
$url = "http://local.yahooapis.com/MapsService/V1/geocode" .
       "?appid=$appid&location=";
$q = urlencode($_GET['q']);
$format = isset($_GET['rss'])? 1 : 0;

$baseurl = "http://waqt.org";
$url = $url . $q . "&output=php";

if (strlen($q) == 0) return; 
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$res = curl_exec($ch);
curl_close($ch);
$locations = unserialize($res);

if (isset($locations['ResultSet']['Result'][1])) 
   return showSearchResults($locations['ResultSet']['Result']);

$res = $locations['ResultSet']['Result'];
$lat = $res['Latitude'];
$long = $res['Longitude'];
$addr = calc_addr($res);

$url = "http://ws.geonames.org/timezoneJSON?lat=$lat&lng=$long";
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$res = curl_exec($ch);
curl_close($ch);
$tz_data = json_decode($res, true);
$gmt_offset = $tz_data['gmtOffset'];
$dst_offset = $tz_data['dstOffset'];
$dst = ($gmt_offset != $dst_offset);

/* methods
 || 1 || Egyptian General Authority of Survey ||
 || 2 || University of Islamic Sciences, Karachi (Shaf'i) ||
 || 3 || University of Islamic Sciences, Karachi (Hanafi) ||
 || 4 || Islamic Society of North America ||
 || 5 || Muslim World League (MWL) ||
 || 6 || Umm Al-Qurra (Saudi Arabia ||
 || 7 || Fixed Isha Interval (always 90) ||
 */
$method = 4;
$prayers = itl_get_prayer_times($long, $lat, $gmt_offset, $method,
                                date('j'), date('n'), date('Y'), $dst);

return showSalatTimes($addr, $prayers);

function calc_addr($res){
   $city = $res['City'];
   $state = $res['State'];
   $zip = $res['Zip'];
   $country = $res['Country'];

   $loc = '';
   if (!empty($city)) $loc = $city;
   if (!empty($state)) $loc .= (empty($loc)? $state : ", $state");
   if (!empty($zip)) $loc .= (empty($loc)? $zip : " $zip");
   return $loc;
}

function showSalatTimes($location, $pt){
   global $format;
   $result = array();
   $times = array(0 => "Fajr", 1 => "Shurooq", 2 => "Dhuhr",
                  3 => "3asr", 4 => "Maghrib", 5 => "3isha");
   foreach ($times as $key => $val){
      $min = $pt[$key]['minute'];
      $hour = $pt[$key]['hour'];
      $time_of_day = 'am';
      if ($hour >= 12) {
         $time_of_day = 'pm';
         if ($hour > 12) $hour -= 12;
      }
      
      if ($min < 10) $min = "0$min";
      $time = $hour . ":" . $min . " $time_of_day";
      $result[$val] = $time;
   }

   if ($format == 1)
      showRssSalatTimes($location, $result);
   else showHtmlSalatTimes($location, $result);

}

function showHtmlSalatTimes($location, $data){
   global $baseurl;
   $param = "q=$location";

   print <<<SALATHTML_HEADER
   <span class="times-header">Prayer times for: $location
   <a href="$baseurl/calculate.php?$param&rss">
   <img src="imgs/feedicon.png"></a>
   </span><br>
SALATHTML_HEADER;

   foreach ($data as $val => $time){
      print "<span class=\"salat-name\">$val: </span> " .
         "<span class=\"salat-time\">$time</span><br>\n";
   }
}

function showRssSalatTimes($location, $data){
   global $baseurl;
   header("Content-Type: text/xml");
   print <<<RSSHEADER
<?xml version="1.0" encoding="utf-8"?>
   <rss version="2.0"
        xmlns:content="http://purl.org/rss/1.0/modules/content/"
        xmlns:wfw="http://wellformedweb.org/CommentAPI/"
        xmlns:dc="http://purl.org/dc/elements/1.1/"
   >
   <channel>
      <title>PrayerTimes for $location</title>
      <link>$baseurl</link>
      <description>prayertime feeds for the whole world, provided
                   by waqt.org</description>
      <language>en-us</language>
RSSHEADER;

   $i=0;
   foreach ($data as $val => $time){
      $i++;
      print <<<RSSDATA
      <item>
      <guid isPermaLink="false">$baseurl/$i</guid>
      <title>$val</title>
      <description>$time</description>
      </item>
RSSDATA;
   }
   print "</channel></rss>";
}

function showSearchResults($results){
   global $format;
   if ($format == 0)
      showSearchResultsHtml($results);
}

function showSearchResultsHtml($results){
   print "<span class=\"search-header\">multiple locations " .
      "matched your query...</span><br>\n";
   foreach ($results as $r){
      $addr = calc_addr($r);
      print "<span class=\"search-result\"><a href=\"javascript" .
         ":manualLocation('" . $addr . "');\">" . $addr . "</a>" .
         "</span><br>\n";
   }
}

?>
