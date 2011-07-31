<?php
   $param = "q=$location&method=$method";

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

   /* methods
    || 1 || Egyptian General Authority of Survey ||
    || 2 || University of Islamic Sciences, Karachi (Shaf'i) ||
    || 3 || University of Islamic Sciences, Karachi (Hanafi) ||
    || 4 || Islamic Society of North America ||
    || 5 || Muslim World League (MWL) ||
    || 6 || Umm Al-Qurra (Saudi Arabia ||
    || 7 || Fixed Isha Interval (always 90) ||
    */

   $methods = array(1 => 'Egyptian General Authority of Survey',
      2 => 'University of Islamic Sciences, Karachi (Shaf\'i)',
      3 => 'University of Islamic Sciences, Karachi (Hanafi)',
      4 => 'Islamic Society of North America',
      5 => 'Muslim World League (MWL)',
      6 => 'Umm Al-Qurra (Saudi Arabia)',
      7 => 'Fixed Isha Interval (always 90)');

   $methname = $methods[$method];
   print '<div class="clear"></div></dl>';
   print "<div class=\"searchdesc\"> <b>Calculation Method:</b> $methname (<a class=\"changelink\" href=\"javascript:toggleMethodBox();\">change</a>)</div>";
?>

<div id="methodbox">
<select onchange="return changeMethod(this)">
<?php
   foreach ($methods as $id => $name){
      $sel = ($id == $method)? "selected" : "";
      print "<option $sel value=\"$id\">$name</option>";
   }
?>
</select>
</div>
