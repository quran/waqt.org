<?php
$gaCode = 'UA-10364217-1';

define('GA_CODE', $gaCode);
define('USERNAME', 'ahmedre');
$waqtSqlPassword = getenv('WAQT_SQL_PASSWORD');
if ($waqtSqlPassword === false || $waqtSqlPassword === '') {
   throw new RuntimeException('WAQT_SQL_PASSWORD must be supplied at runtime');
}
define('WAQT_SQL_PASSWORD', $waqtSqlPassword);
unset($waqtSqlPassword);
