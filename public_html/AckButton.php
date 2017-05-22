<?php
include('mysql_conn.php');

$sql = "UPDATE `corvus`.`alert_status` SET `Status`='0' WHERE `AlertType`='".$_POST["a_type"]."'";
if (mysqli_query($conn, $sql)) {
echo "Record updated successfully";
} else {
echo "Error updating record: " . mysqli_error($conn);
}
?>