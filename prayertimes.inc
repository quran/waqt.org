<?php

class PrayerTimes {
   public static function getLocations($q){
      include 'settings.inc';  // todo: make these defines and use them
      $url = "http://local.yahooapis.com/MapsService/V1/geocode" .
             "?appid=$appid&location=";
      $q = urlencode($_GET['q']);
      $format = isset($_GET['rss'])? 1 : 0;

      $url = $url . $q . "&output=php";

      if (strlen($q) == 0) return; 
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      $res = curl_exec($ch);
      curl_close($ch);
      return unserialize($res);
   }

   public static function getTimezone($lat, $long){
      // geonames' json api is currently broken - switching to xml api
      // $url = "http://ws.geonames.org/timezoneJSON?lat=$lat&lng=$long";

      $url = "http://ws.geonames.org/timezone?lat=$lat&lng=$long";
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      $res = curl_exec($ch);

      $info = curl_getinfo($ch);
      $ret = $info['http_code'];
      curl_close($ch);

      // specifically for xml interface
      $result = array();
      if (preg_match('/\<timezoneId>(.*)<\/timezoneId>/', $res, $matches))
         $result['timezoneId'] = $matches[1];
      if (preg_match('/\<gmtOffset>(.*)<\/gmtOffset>/', $res, $matches))
         $result['gmtOffset'] = $matches[1];
      if (preg_match('/\<dstOffset>(.*)</dstOffset>/', $res, $matches))
         $result['dstOffset'] = $matches[1];

      // return array('rescode' => $ret, 'result' => json_decode($res, true));
      return array('rescode' => $ret, 'result' => $result);
   }

   public static function getPrayerTimes($q, $method = 4){
      $pos = strpos($q, "loc:");
      if ($pos!==false){
         $q = substr($q, $pos+4);
         if (empty($q)) return array('type' => 'error', 
            'err' => 'invalid query string', 'data' => array());
         
         list($lat, $long) = split(',', $q);
         $addr = "loc:$lat,$long";
      }
      else {
         $locations = PrayerTimes::getLocations($q);
         if ($locations == false){
            $errmsg = 'one of the apis which we depend on is ' .
              'currently broken, please try again later.';
            return array('type' => 'error',
               'msg' => $errmsg,  'data' => array());
         }
      
         if (isset($locations['ResultSet']['Result'][1])){
            $locs = $locations['ResultSet']['Result'];
            $vals = array();
            foreach ($locs as $loc)
               $vals[] = PrayerTimes::calc_addr($loc);
            return array('type' => 'search_results', 'data' => $vals);
         }
        
         $res = $locations['ResultSet']['Result'];
         $lat = $res['Latitude'];
         $long = $res['Longitude'];
         $addr = PrayerTimes::calc_addr($res);
      }

      $tz_arr = PrayerTimes::getTimezone($lat, $long);
      if ($tz_arr['rescode']!=200){
         $errmsg = 'the geonames api which we depend on is ' .
           'currently broken, please try again later.';
         return array('type' => 'error',
            'msg' => $errmsg,  'data' => array());
      }

      $tz_data = $tz_arr['result'];
      $gmt_offset = $tz_data['gmtOffset'];
      $timezone_id = $tz_data['timezoneId'];

      // unfortunately, the dstOffset returned by geonames
      // is not necessarily accurate (ex with Egypt).  so
      // have to keep update zoneinfo packages and compare
      // to whatever php says.
      try {
         $tz = new DateTimeZone($timezone_id);
         $date = new DateTime();
         $date->setTimezone($tz);
         $dst_offset = $date->getOffset()/3600;
      }
      catch (Exception $e){
         $dst_offset = $tz_data['dstOffset'];
      }

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
      $method = 4;  // todo: make method and dates options
      $prayers = itl_get_prayer_times($long, $lat, $gmt_offset, $method,
                                      date('j'), date('n'), date('Y'), $dst);
      return array('type' => 'prayertimes', 
         'data' => $prayers, 'location' => $addr);
   }
   
   public static function calc_addr($res){
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
}
