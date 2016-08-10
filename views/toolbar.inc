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

   $host = "localhost";
   $username = "quran";
   $password = "qutubi786";
   $database = "quran";

   $link = mysqli_connect($host, $username, $password);
   if (!$link) return;
   if (!mysql_select_db($database)) return;

   $result = mysql_query($link, "SELECT * FROM quran.toolbar WHERE type='toolbar' AND active='1' ORDER BY weight");
   if (!$result) return;

   $i = 0;
   $string = '';
   while ($toolbarItem = mysql_fetch_assoc($result)){
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

      if ($i < mysql_num_rows($result))
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
