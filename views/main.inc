<?php include 'views/header.inc'; ?>
<script type="text/javascript" src="prototype.lite.js"></script>
<script type="text/javascript" src="moo.ajax.js"></script>
<form name="ptform" action="javascript:void(0);" onsubmit="javascript:handleSubmit();">
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
      $('calculating').innerHTML = '<img src="/imgs/loading.gif">';
      $('prayertimes').innerHTML = '';
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
   
	init();
-->
</script>
<?php include 'views/footer.inc'; ?>