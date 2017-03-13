<?php
include('mysql_conn.php');

$alertables = array("System","Location","Health","Temperature");
$arrlength = count($alertables);

for($i = 0; $i < $arrlength; $i++) {
	$sql_update = "UPDATE `corvus`.`alert_status` SET `Status`='1' WHERE `AlertType`='".$alertables[$i]."'";
	
	if (mysqli_query($conn, $sql_update)) {
		echo "Record updated successfully";
		sleep(0.1);
	} else {
		echo "Error updating record: " . mysqli_error($conn);
	}
}
	/*
$sql = "UPDATE `corvus`.`alert_status` SET `Status`='1' WHERE `AlertType`='Temperature'";
if (mysqli_query($conn, $sql)) {
echo "Record updated successfully";
} else {
echo "Error updating record: " . mysqli_error($conn);
}
*/
?>