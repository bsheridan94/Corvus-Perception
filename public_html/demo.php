<?php
include('mysql_conn.php');

$json = array();
$sql_select = "SELECT * FROM corvus.alert_status";
$result = mysqli_query($conn, $sql_select);

while($row = mysqli_fetch_array ($result))     
{
    $bus = array(
        'AlertType' => $row['AlertType'],
        'Status' => (int)$row['Status']
    );
    array_push($json, $bus);
}

$jsonstring = json_encode($json);
echo $jsonstring;
?> 