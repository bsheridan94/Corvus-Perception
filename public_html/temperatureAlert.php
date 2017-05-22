<?php
include('mysql_conn.php');

$Delta = 8;
$LowTemp = 73;
$HighTemp = 76;
$LowLimit = 0;
$HighLimit = 0;

$sql_count = "SELECT count(*) FROM corvus.temperature";
$rowCountSQLResult = mysqli_query($conn, $sql_count);

$row = mysqli_fetch_array($rowCountSQLResult);
$rowCount = (int)$row[0];



while ( $HighLimit < $rowCount) {
	$HighLimit = $LowLimit + $Delta;
	if ($HighLimit > $rowCount){
		$HighLimit = $rowCount;
	}

	$sql_check = "SELECT count(TempReading), TempReading  FROM corvus.temperature WHERE (TempReading<".$LowTemp." OR TempReading>".$HighTemp.")AND id BETWEEN ".$LowLimit." AND ".$HighLimit;
	
	$tempCheckSQLResult = mysqli_query($conn, $sql_check);

	$results_new = mysqli_fetch_array($tempCheckSQLResult);
	sleep(0.01);
	$tempCheckCount = $results_new[0];
	
	//echo "Checking temp values between ".$LowLimit." AND ".$HighLimit."\n\n";
	if ($tempCheckCount >0){
		echo $results_new[1];
		$sql = "UPDATE `corvus`.`alert_status` SET `Status`='1' WHERE `AlertType`='temperature'";
		mysqli_query($conn, $sql);
		break;
	}
	$LowLimit = $HighLimit;
}


?> 
