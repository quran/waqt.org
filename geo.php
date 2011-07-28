<?php
// implementation of geohasing functions
// based on http://github.com/davetroy/geohash-js/blob/master/geohash.js

class GeoHashUtils {
   private static $base32 = '0123456789bcdefghjkmnpqrstuvwxyz';

   private static $neighbors = array(
      'odd' => array('bottom' => '238967debc01fg45kmstqrwxuvhjyznp',
                     'top' => 'bc01fg45238967deuvhjyznpkmstqrwx',
                     'left' => '14365h7k9dcfesgujnmqp0r2twvyx8zb',
                     'right' => 'p0r21436x8zb9dcf5h7kjnmqesgutwvy'),
      'even' => array('right' => 'bc01fg45238967deuvhjyznpkmstqrwx',
                      'left' => '238967debc01fg45kmstqrwxuvhjyznp',
                      'top' => 'p0r21436x8zb9dcf5h7kjnmqesgutwvy',
                      'bottom' => '14365h7k9dcfesgujnmqp0r2twvyx8zb'));

   private static $borders = array(
      'odd' => array('bottom' => '0145hjnp', 'top' => 'bcfguvyz',
                     'left' => '028b', 'right' => 'prxz'),
      'even' => array('right' => 'bcfguvyz', 'left' => '0145hjnp',
                      'top' => 'prxz', 'bottom' => '028b'));

   private static $bits = array(16, 8, 4, 2, 1);
   private static $latRange = array(-90.0, 90.0);
   private static $lngRange = array(-180.0, 180.0);

   public static function getNeighbors($geohash){
      $neighbors = array();
      $neighbors[0] = GeoHashUtils::calcNeighbors($geohash, 'top');
      $neighbors[1] = GeoHashUtils::calcNeighbors($neighbors[0], 'right');
      $neighbors[2] = GeoHashUtils::calcNeighbors($geohash, 'right');
      $neighbors[3] = GeoHashUtils::calcNeighbors($neighbors[2], 'bottom');
      $neighbors[4] = GeoHashUtils::calcNeighbors($geohash, 'bottom');
      $neighbors[5] = GeoHashUtils::calcNeighbors($neighbors[4], 'left');
      $neighbors[6] = GeoHashUtils::calcNeighbors($geohash, 'left');
      $neighbors[7] = GeoHashUtils::calcNeighbors($neighbors[6], 'top');
      
      return $neighbors; 
   }

   public static function calcNeighbors($geohash, $direction){
      $geohash = strtolower($geohash);
      $last = $geohash[strlen($geohash)-1];
      $type = (strlen($geohash) % 2)? 'odd' : 'even';
      $base = substr($geohash, 0, strlen($geohash)-1);

      $b = GeoHashUtils::$borders[$type];
      $n = GeoHashUtils::$neighbors[$type];
      $val = strpos($b[$direction], $last);
      if (($val !== false) && ($val != -1))
         $base = GeoHashUtils::calcNeighbors($base, $direction);

      $ni = strpos($n[$direction], $last);
      return $base . GeoHashUtils::$base32[$ni];
   }

   public static function deHashisize($geohash){
      $isEven = true;
      $lat = GeoHashUtils::$latRange;
      $lng = GeoHashUtils::$lngRange;

      for ($i=0; $i<strlen($geohash); $i++){
         $c = $geohash[$i];
         $cd = strpos(GeoHashUtils::$base32, $c);
         for ($j=0; $j<5; $j++){
            $mask = GeoHashUtils::$bits[$j];
            $val = ($cd & $mask)? 0 : 1;
            if ($isEven)
               $lng[$val] = ($lng[0] + $lng[1]) / 2;
            else $lat[$val] = ($lat[0] + $lat[1]) / 2;
            $isEven = !$isEven;
         }
      }

      return array('latitude' => $lat, 'longitude' => $lng);
   }

   public static function geoHashize($latitude, $longitude){
      $ch = 0;
      $bit = 0;
      $geohash = "";
      $isEven = true;
      $precision = 12;
      $lat = GeoHashUtils::$latRange;
      $lng = GeoHashUtils::$lngRange;

      while (strlen($geohash) < $precision){
         if ($isEven){
            $mid = ($lng[0] + $lng[1]) / 2;
            if ($longitude > $mid){
               $ch |= GeoHashUtils::$bits[$bit];
               $lng[0] = $mid;
            }
            else $lng[1] = $mid;
         }
         else {
            $mid = ($lat[0] + $lat[1]) / 2;
            if ($latitude > $mid){
               $ch |= GeoHashUtils::$bits[$bit];
               $lat[0] = $mid;
            }
            else $lat[1] = $mid;
         }

         $isEven = !$isEven;
         if ($bit < 4) $bit++;
         else {
            $geohash .= GeoHashUtils::$base32[$ch];
            $bit = 0;
            $ch = 0;
         }
      }

      return $geohash;
   }
}
