<?php

include_once 'settings.inc';

class PrayerTimes {

   public static function getLocations($q){
      $url = "http://local.yahooapis.com/MapsService/V1/geocode" .
             "?appid=" . YAHOO_APPID . "&location=";
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

   public static function getTimezoneInfoFromCache($geohash){
      mysql_connect('localhost', 'waqt', WAQT_SQL_PASSWORD);
      mysql_select_db('waqt');

      $q = 'select dst_offset, gmt_offset, timezone from ' .
         "timezoneCache where geohash='$geohash'";
      $res = mysql_query($q) or null;

      $ret = null;
      if (($res != null) & ($row = mysql_fetch_assoc($res))){
         $ret = array();
         $ret['dstOffset'] = $row['dst_offset'];
         $ret['gmtOffset'] = $row['gmt_offset'];
         $ret['timezoneId'] = $row['timezone'];
      }

      mysql_close();
      if ($ret != null)
         return array('rescode' => 200, 'result' => $ret);
      return $ret;
   }

   public static function saveTimezoneInfoToCache($geohash, $json){
      $lat = $json['lat'];
      $lng = $json['lng'];
      $tz = mysql_real_escape_string($json['timezoneId']);
      $rawOffset = $json['rawOffset'];
      $dstOffset = $json['dstOffset'];
      $gmtOffset = $json['gmtOffset'];

      mysql_connect('localhost', 'waqt', WAQT_SQL_PASSWORD);
      mysql_select_db('waqt');
      $q = 'insert into timezoneCache(geohash, latitude, longitude, ' .
         'timezone, raw_offset, dst_offset, gmt_offset) values(' .
         "'$geohash', $lat, $lng, '$tz', $rawOffset, $dstOffset, " .
         "$gmtOffset)";
      mysql_query($q);
      mysql_close();
   }

   public static function getTimezone($lat, $long){
      require_once('geo.php');
      $geohash = GeoHashUtils::geoHashize($lat, $long);
      $ret = PrayerTimes::getTimezoneInfoFromCache($geohash);
      if ($ret != null) return $ret;
      
      $url = "http://ws.geonames.org/timezoneJSON?lat=$lat&lng=$long";

      $url .= "&username=" . USERNAME;
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      $res = curl_exec($ch);

      $info = curl_getinfo($ch);
      $ret = $info['http_code'];
      curl_close($ch);

      $json = json_decode($res, true);
      PrayerTimes::saveTimezoneInfoToCache($geohash, $json);
      return array('rescode' => $ret, 'result' => $json);
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
      $timezone_id = $tz_data['timezoneId'];
      
      // unfortunately, the time offsets returned by geonames
      // are not necessarily accurate (ex dst with Egypt, gmt
      // with Sydney).  we consequently ignore geonames' offsets
      // and figure out the current offset and whether or not we
      // are in dst, since that's all we need.  all this means is
      // that we have to keep the zoneinfo package on the system
      // updated.
      try {
         $tz = new DateTimeZone($timezone_id);
         $date = new DateTime();
         $date->setTimezone($tz);
         $cur_offset = $date->getOffset()/3600;
         
         $i = 1;
         $now = time();
         $transitions = $tz->getTransitions();
         $max = count($transitions);
         while (true){
            // i make no guarantees about this code after october 2037.
            // if it gets to that point, fallback to geonames.

            // means we reach the end with no applicable switch rule.
            // in this case, use the cur offset and assume no dst. 
            if (!isset($transitions[$i+1])){
               $dst = 0;
               $gmt_offset = $cur_offset;
               break;
            }

            if (($now >= $transitions[$i]['ts']) &&
                ($now < $transitions[$i+1]['ts'])){
               $dst = $transitions[$i]['isdst'];
               $gmt_offset = $cur_offset - ($dst? 1 : 0);
               break;
            }
            else $i++;
         }
      }
      catch (Exception $e){
         $cur_offset = $tz_data['dstOffset'];
         $gmt_offset = $tz_data['gmtOffset'];
         $dst = ($gmt_offset != $cur_offset);

         if ($dst && (($gmt_offset + 1) != $cur_offset)){
            // handle reverse dst case
            $dst = false;
            $gmt_offset = $cur_offset;
         }
      }
      
      $method = $method + 0;
      /* methods
       || 1 || Egyptian General Authority of Survey ||
       || 2 || University of Islamic Sciences, Karachi (Shaf'i) ||
       || 3 || University of Islamic Sciences, Karachi (Hanafi) ||
       || 4 || Islamic Society of North America ||
       || 5 || Muslim World League (MWL) ||
       || 6 || Umm Al-Qurra (Saudi Arabia ||
       || 7 || Fixed Isha Interval (always 90) ||
       */
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
