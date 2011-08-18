<?php include 'views/header.inc'; ?>
<script type="text/javascript" src="prototype_lite.js"></script>
<script type="text/javascript" src="moo.ajax.js"></script>

<form name="ptform" action="javascript:void(0);" onsubmit="javascript:handleSubmit();">
   <input class="zipcode" type="text" size="35" id="location" value="<?php echo $q; ?>">
   <input class="method" type="hidden" id="method">
	<input class="search" type="submit" value="Search">
</form>
<div class="searchdesc">to get prayertimes, type in a zipcode, postal code, city, city and state, country, or address.</div>
<div id="calculating"></div>
<div id="prayertimes">
<?php
if ((isset($search_results)) && (!is_null($search_results)))
   include 'views/locsearch.inc';
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

      methodParam = '';
      var method = document.ptform.method.value;
      if (input.length > 0)
         methodParam = '&method=' + method;

      new ajax('calculate.php?ajax=1' + methodParam + '&q=' + input,
         { update: $('prayertimes'), 
           method: 'get', 
           onComplete: clearCalculating });
   }

   function manualLocation(loc){
      document.ptform.location.value = loc;
      handleSubmit();
   }

   function toggleMethodBox(){
      $('methodbox').style.display = 'block';
   }

   function changeMethod(selector){
      method = selector.options[selector.options.selectedIndex];
      document.ptform.method.value = method.value;
      handleSubmit();
   }

	init();
-->
</script>
<?php include 'views/footer.inc'; ?>
