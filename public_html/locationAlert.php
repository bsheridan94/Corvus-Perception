<?php
include('mysql_conn.php');

$Delta = 10;
$LowTemp = 0.0;
$HighTemp = 0.0;

$Sensitivity_sql = "SELECT status FROM corvus.alert_status where AlertType = 'Sensitivity'";
$SenQueryResult = mysqli_query($conn, $Sensitivity_sql);

$SenQueryResult = mysqli_fetch_array($SenQueryResult);
sleep(0.01);
$Sensitivity = (int)$SenQueryResult[0];


$LowLimit_sql = "SELECT min(id) FROM corvus.location";
$LowLimit_sqlResult = mysqli_query($conn, $LowLimit_sql);

$LowLimit_sqlResult = mysqli_fetch_array($LowLimit_sqlResult);
sleep(0.01);
$LowLimit = (int)$LowLimit_sqlResult[0];


$HighLimit_sql = "SELECT max(id) FROM corvus.temperature";
$HighLimit_sqlResult = mysqli_query($conn, $HighLimit_sql);

$HighLimit_sqlResult = mysqli_fetch_array($HighLimit_sqlResult);
sleep(0.01);
$MaxLimit = (int)$HighLimit_sqlResult[0];

if ($Sensitivity == 0 ){
	$HighTemp = 5;
}
else if ($Sensitivity == 1 ){
	$HighTemp = 3;
}
else if ($Sensitivity == 2 ){
	$HighTemp = 1;
}

$sql_count = "SELECT count(*) FROM corvus.temperature";
$rowCountSQLResult = mysqli_query($conn, $sql_count);

$row = mysqli_fetch_array($rowCountSQLResult);
$rowCount = (int)$row[0];

while ( $HighLimit < $MaxLimit) {
	$HighLimit = $LowLimit + $Delta;
	if ($HighLimit > $MaxLimit){
		$HighLimit = $MaxLimit;
	}

	$sql_check = "SELECT count(*) FROM corvus.location WHERE (location = 0)AND id BETWEEN ".$LowLimit." AND ".$HighLimit;
	
	$tempCheckSQLResult = mysqli_query($conn, $sql_check);

	$results_new = mysqli_fetch_array($tempCheckSQLResult);
	sleep(0.01);
	$tempCheckCount = $results_new[0];
	
	//echo "Checking temp values between ".$LowLimit." AND ".$HighLimit."\n\n";
	if ($tempCheckCount >= $HighTemp){
		echo $results_new[1];
		$sql = "UPDATE `corvus`.`alert_status` SET `Status`='1' WHERE `AlertType`='Location'";
		mysqli_query($conn, $sql);
		break;
	}
	
	$HighLimit_sql = "SELECT max(id) FROM corvus.temperature";
	$HighLimit_sqlResult = mysqli_query($conn, $HighLimit_sql);

	$HighLimit_sqlResult = mysqli_fetch_array($HighLimit_sqlResult);
	sleep(0.01);
	$MaxLimit = (int)$HighLimit_sqlResult[0];
	
	$LowLimit = $HighLimit;
}


?> 
