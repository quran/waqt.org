<head>
<title>waqt.org - muslim prayertimes</title>

<script type="text/javascript" src="prototype.lite.js"></script>
<script type="text/javascript" src="moo.ajax.js"></script>
<link rel="stylesheet" type="text/css" href="style.css"> 
</head>

<script type="text/javascript">
<!--
   function handleSubmit(){
      var input = document.ptform.location.value;
      if (input.length==0) return false;
      new ajax('calculate.php?q=' + input,
         { update: $('prayertimes'), method: 'get' });
   }

   function manualLocation(loc){
      document.ptform.location.value = loc;
      handleSubmit();
   }

-->
</script>

<body onload="javascript:$('location').focus();">

<div class="toplinks">
<a href="about.php">About</a> | <a href="http://github.com/ahmedre/waqt.org">Github</a> 
</div>

<center>
<img src="imgs/waqt.png">
<div id="all">
   <form name="ptform" action="javascript:void(0);"
         onsubmit="javascript:handleSubmit();">
      <input type="text" id="location">
   </form>
   <div id="prayertimes"></div>
</div>
</center>
</body>
