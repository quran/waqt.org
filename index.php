<?php
if (isset($_GET['about'])){
   include 'views/about.inc';
   return;
}

// $ajax is set automagically from calculate.php for legacy purposes
// $_GET['ajax'] should be set by the javascript in an ajax call.
$ajax = isset($ajax)? true : (isset($_GET['ajax'])? true : false);
$format = (isset($_GET['rss'])? 'rss' : 'html');

$q = "";
$data = null;
if (isset($_GET['q'])){
   include 'prayertimes.inc';
   $q = urlencode($_GET['q']);
   if (strlen($q) > 0)
      $data = PrayerTimes::getPrayerTimes($q);
}

if (!is_null($data)){
   if ($data['type']=='search_results'){
      $search_results = $data['data'];
      if ($ajax) include 'views/locsearch.inc';
      else include 'views/main.inc';
   }
   else showSalatTimes($data['location'], $data['data'], $format, $ajax);
   return;
}
else if ($ajax) return;
else include 'views/main.inc';

function showSalatTimes($location, $pt, $format, $ajax = true){
   global $q;  // don't like this, but...

   $data = array();
   $times = array(0 => "Fajr", 1 => "Shurooq", 2 => "Dhuhr",
                  3 => "'Asr", 4 => "Maghrib", 5 => "'Isha");
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
      $data[$val] = $time;
   }

   if ($format == 'rss')
      include 'views/salatrss.inc';
   else if (!$ajax) include 'views/main.inc';
   else include 'views/salatimes.inc';
}
