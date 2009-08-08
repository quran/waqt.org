<head>
<title>waqt.org - muslim prayertimes</title>

<script type="text/javascript" src="prototype.lite.js"></script>
<script type="text/javascript" src="moo.ajax.js"></script>
<link rel="stylesheet" type="text/css" href="style.css"> 
</head>

<script type="text/javascript">
<!--
   function init(){
      $('location').focus();
      var input = document.ptform.location.value;
      if (input.length==0){
         if (navigator.geolocation){
            navigator.geolocation.getCurrentPosition(function(position) {
               document.ptform.location.value = 
                  "loc:" + position.coords.latitude + "," + 
                  position.coords.longitude;
               handleSubmit();
            });
         }
      }
   }

   function clearCalculating(){
      $('calculating').innerHTML = '';
   }

   function handleSubmit(){
      $('calculating').innerHTML = 'please wait...';
      var input = document.ptform.location.value;
      if (input.length==0){
         $('calculating').innerHTML = '';
         return false;
      }

      new ajax('calculate.php?ajax=1&q=' + input,
         { update: $('prayertimes'), 
           method: 'get', 
           onComplete: clearCalculating });
   }

   function manualLocation(loc){
      document.ptform.location.value = loc;
      handleSubmit();
   }

-->
</script>

<body onload="javascript:init();">

<div class="toplinks">
<a href="/about">About</a> | <a href="http://github.com/ahmedre/waqt.org">Github</a> 
</div>

<center>
<img src="imgs/waqt.png">
<div id="all">
   <form name="ptform" action="javascript:void(0);"
         onsubmit="javascript:handleSubmit();">
      <input type="text" id="location" value="<?php echo $q; ?>">
      <div class="searchdesc">to get prayertimes, type in a zipcode, postal code, city, city and state, country, or address.</div>
   </form>
   <div id="calculating"></div>
   <div id="prayertimes">
   <?php
      if (!is_null($search_results)) include 'views/locsearch.inc';
      else if (!is_null($data)) include 'views/salatimes.inc';
   ?>
   </div>
</div>
</center>

<?php include "settings.inc"; ?>
<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
try {
var pageTracker = _gat._getTracker("<?php echo $gaCode; ?>");
pageTracker._trackPageview();
} catch(err) {}</script>
</body>
