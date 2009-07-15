<?php
   $param = "q=$location";

   print <<<SALATHTML_HEADER
   <span class="times-header">Prayer times for: $location
   <a href="/rss?$param">
   <img src="imgs/feedicon.png"></a>
   </span><br>
SALATHTML_HEADER;

   foreach ($data as $val => $time){
      print "<span class=\"salat-name\">$val: </span> " .
         "<span class=\"salat-time\">$time</span><br>\n";
   }
