<?php
header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');

$SYSTEMnonAlert = '<td class="not-alerting"><i class="fa fa-server fa-2x link-icon" alt="System"></i></td><td>All devices are operational</td>';

$SYSTEMAlert = '<td class="alerting"><i class="fa fa-server fa-2x link-icon" alt="System"></i></td><td>There is an error with one of the devices</td><td><button type="button" onclick="onSysAck()">Acknowledge</button></td>';

$TEMPAlert = '<td class="alerting"><i class="fa fa-thermometer-three-quarters fa-2x link-icon" alt="Temperature"></i></td><td>Temperature outside of preset range</td><td><button type="button" onclick="onTempAck()">Acknowledge</button></td>';

$TEMPAlertHigh = '<td class="alerting"><i class="fa fa-thermometer-three-quarters fa-2x link-icon" alt="Temperature"></i></td><td>Temperature too high</td><td><button type="button" onclick="onTempAck()">Acknowledge</button></td>';

$TEMPAlertLow = '<td class="alerting"><i class="fa fa-thermometer-three-quarters fa-2x link-icon" alt="Temperature"></i></td><td>Temperature too low</td><td><button type="button" onclick="onTempAck()">Acknowledge</button></td>';


echo "data: {$SYSTEMAlert}\n\n";
flush();
?> 