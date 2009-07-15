<?php
   print "<span class=\"search-header\">multiple locations " .
      "matched your query...</span><br>\n";
   foreach ($search_results as $addr){
      print "<span class=\"search-result\"><a href=\"javascript" .
         ":manualLocation('" . $addr . "');\">" . $addr . "</a>" .
         "</span><br>\n";
   }
