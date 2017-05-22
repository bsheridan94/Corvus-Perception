<?php
include('mysql_conn.php');

$json = array();
$sql_select = "SELECT Status FROM corvus.alert_status where AlertType = 'Sensitivity'";
$result = mysqli_query($conn, $sql_select);

while($row = mysqli_fetch_array ($result))     
{
    $bus = array(
        'Status' => (int)$row['Status']
    );
    array_push($json, $bus);
}

$jsonstring = json_encode($json);
echo $jsonstring;
?> 