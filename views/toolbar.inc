<?php
function buildToolbar(){
   $toolbar_key = 'quranicaudio.com.toolbar';
   
   $mc = null;
   $m = new Memcache;
   if (@$m->connect('localhost', 11211)){
      $mc = $m;
      $cached = $mc->get($toolbar_key);
      if ($cached != false) return $cached;
   }

   $host = "waqt-db";
   $username = "waqt";
   $password = "qurtubi786";
   $database = "waqt";

   $link = mysqli_connect($host, $username, $password);
   if (!$link) return;
   if (!mysqli_select_db($link, $database)) return;

   $result = mysqli_query($link, "SELECT * FROM toolbar WHERE type='toolbar' AND active='1' ORDER BY weight");
   if (!$result) return;

   $i = 0;
   $string = '';
   while ($toolbarItem = mysqli_fetch_assoc($link, $result)){
      $i++;
      $reps = array('http://');
      if (strpos($_SERVER['SERVER_NAME'], 'www.')===false)
         $reps[] = 'www.';
      $tl_link = str_replace($reps, '', $toolbarItem['itemLink']);

      if ($_SERVER['SERVER_NAME'] == $tl_link)
          $string .= '<span>'.$toolbarItem['itemLabel'].'</span>';
      else {
          $string .= '<a href="'.$toolbarItem['itemLink'].'"';
          if ($toolbarItem['popup'] == 1)
               $string .= ' target="_blank"';
          $string .= '>'.$toolbarItem['itemLabel'].'</a>';
      }

      if ($i < mysqli_num_rows($link, $result))
           $string .= ' | ';
   }

   $retval = '<div id="toolbar"><div id="toolbarRight">' .
         $string.'</div></div>';

   if (!is_null($mc))
      // cache for an hour
      $mc->set($toolbar_key, $retval, 0, 3600);
   return $retval;
}

echo buildToolbar();
?>
