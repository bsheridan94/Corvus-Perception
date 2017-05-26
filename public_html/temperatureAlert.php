<?php
include('mysql_conn.php');

$Delta = 10;
$LowTemp = 0.0;
$HighTemp = 0.0;

$sqlAverage = "SELECT avg(TempReading) FROM corvus.temperature ORDER BY TempReading asc limit 10";
$tempCheckSQLResult = mysqli_query($conn, $sqlAverage);

$results_Avg = mysqli_fetch_array($tempCheckSQLResult);
sleep(0.01);
$results_Avg_Result = (float)$results_Avg[0];
echo $results_Avg_Result."\n\n";

$Sensitivity_sql = "SELECT status FROM corvus.alert_status where AlertType = 'Sensitivity'";
$SenQueryResult = mysqli_query($conn, $Sensitivity_sql);

$SenQueryResult = mysqli_fetch_array($SenQueryResult);
sleep(0.01);
$Sensitivity = (int)$SenQueryResult[0];


$LowLimit_sql = "SELECT min(id) FROM corvus.temperature";
$LowLimit_sqlResult = mysqli_query($conn, $LowLimit_sql);

$LowLimit_sqlResult = mysqli_fetch_array($LowLimit_sqlResult);
sleep(0.01);
$LowLimit = (int)$LowLimit_sqlResult[0];


$HighLimit_sql = "SELECT max(id) FROM corvus.temperature";
$HighLimit_sqlResult = mysqli_query($conn, $HighLimit_sql);

$HighLimit_sqlResult = mysqli_fetch_array($HighLimit_sqlResult);
sleep(0.01);
$MaxLimit = (int)$HighLimit_sqlResult[0];

echo $Sensitivity."\n\n";

if ($Sensitivity == 0 ){
	$LowTemp = 0.85*$results_Avg_Result;
	$HighTemp = 1.15*$results_Avg_Result; 
}
else if ($Sensitivity == 1 ){

	$LowTemp = 0.90*$results_Avg_Result;
	$HighTemp = 1.10*$results_Avg_Result; 
}
else if ($Sensitivity == 2 ){
	$LowTemp = 0.95*$results_Avg_Result;
	$HighTemp = 1.05*$results_Avg_Result; 
}

$sql_count = "SELECT count(*) FROM corvus.temperature";
$rowCountSQLResult = mysqli_query($conn, $sql_count);

$row = mysqli_fetch_array($rowCountSQLResult);
$rowCount = (int)$row[0];

echo $LowTemp."\n\n";
echo $HighTemp."\n\n";


while ( $HighLimit < $MaxLimit) {
	$HighLimit = $LowLimit + $Delta;
	if ($HighLimit > $MaxLimit){
		$HighLimit = $MaxLimit;
	}

	$sql_check = "SELECT count(TempReading), TempReading  FROM corvus.temperature WHERE (TempReading<".$LowTemp." OR TempReading>".$HighTemp.")AND id BETWEEN ".$LowLimit." AND ".$HighLimit;
	
	echo $sql_check."\n\n";
	
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
	
	$HighLimit_sql = "SELECT max(id) FROM corvus.temperature";
	$HighLimit_sqlResult = mysqli_query($conn, $HighLimit_sql);

	$HighLimit_sqlResult = mysqli_fetch_array($HighLimit_sqlResult);
	sleep(0.01);
	$MaxLimit = (int)$HighLimit_sqlResult[0];
	
}


?> 
