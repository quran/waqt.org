<?php
   $param = "q=$location";

   print <<<SALATHTML_HEADER
   <div class="times-header">Prayer times for: $location
   <a href="/rss?$param"><img src="imgs/feedicon.png"></a>
   </div>
   <dl>
SALATHTML_HEADER;

   foreach ($data as $val => $time){
      print "<dt>$val: </dt> " .
         "<dd>$time</dd>\n";
   }

	print '<div class="clear"></div></dl>';