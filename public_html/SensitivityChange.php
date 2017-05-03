<?php
header("Location: http://cs.drexel.edu/~gpf27");
include('mysql_conn.php');

$sql = "UPDATE `corvus`.`alert_status` SET `Status`=".$_GET["SensitivitySlider"]." WHERE `AlertType`='Sensitivity'";
echo $sql;
if (mysqli_query($conn, $sql)) {
echo "Record updated successfully";
} else {
echo "Error updating record: " . mysqli_error($conn);
}
?>
