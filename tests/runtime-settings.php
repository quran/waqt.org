<?php
// Run with php -n so the database calls below use stubs, without network access.
$expected = getenv('WAQT_SQL_PASSWORD');

try {
   require __DIR__ . '/../settings.inc';
} catch (RuntimeException $error) {
   if ($expected === false || $expected === '') {
      echo "PASS: missing database configuration rejected\n";
      exit(0);
   }
   throw $error;
}

if ($expected === false || $expected === '') {
   throw new RuntimeException('Missing database configuration was accepted');
}

class Memcache {
   public function connect($host, $port) { return false; }
}

$connections = 0;
function mysqli_connect($host, $user, $password) {
   global $expected, $connections;
   if ($password !== $expected) {
      throw new RuntimeException('Database connection ignored runtime configuration');
   }
   $connections++;
   return true;
}
function mysqli_select_db($connection, $database) { return false; }
function mysqli_query($connection, $query) { return false; }
function mysqli_real_escape_string($connection, $value) { return $value; }

require __DIR__ . '/../views/toolbar.inc';
require __DIR__ . '/../prayertimes.inc';
PrayerTimes::getPrayerTimes('loc:');
if ($connections !== 2) {
   throw new RuntimeException('Both database connection paths must be exercised');
}
echo "PASS: prayer queries and toolbar use runtime database configuration\n";
