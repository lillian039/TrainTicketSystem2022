<?php
    header("content-type:text/html;charset=utf-8");
    error_reporting(E_ALL &~E_NOTICE);
    $start_oneway = $_POST["start_oneway"];
    $arrive_oneway = $_POST["arrive_oneway"];
    $date_oneway = $_POST["date_oneway"];
    $priority = $_POST["priority"];
    $command = "query_ticket -s ".$start_oneway." -t ".$arrive_oneway." -p ".$priority." -d ".$date_oneway;
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,"81.68.199.253",443);
    socket_write($socket,$command);
    $tmp_res=socket_read($socket,100000);
    echo $tmp_res;
?>