<?php
   header("Content-Type: application/json");
   $res = array();
   $times = array(); 

   $i=0;
   $pubdate = date('r');
   foreach ($data as $val => $time){
      $i++;
      $times[$val] = $time;
   }

   $res['query'] = $location;
   $res['times'] = $times;
   print json_encode($res);
