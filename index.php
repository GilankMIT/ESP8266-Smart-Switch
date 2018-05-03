<?php
date_default_timezone_set('Asia/Jakarta');
$host = "127.0.0.1";
$db   = "swiper";
$user = "root";
$pass = "itgilang";
$charset = "utf8mb4";
$dsn = "mysql:host=$host;dbname=$db;charset=$charset";
$opt = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false
];

$pdo = new PDO($dsn, $user, $pass, $opt);



//Check for HTTP request
$httpRequest = $_GET;
$param           = $httpRequest['param'];


if($param == "CHECKDATA"){
    $dl_start = "05:30:00";
    $dl_end   = "18:00:00";

    $stmt = $pdo->query("SELECT * FROM status");


    

    $data;
    while($row = $stmt->fetch()){

        if(time() > strtotime($dl_start) && time() < strtotime($dl_end)){
            $row['dl'] = 1;
        }else{
            $row['dl'] = 0;
        }

        $data = $row;
        
    }

    echo json_encode($data);

}else if($param == "PUSHDATA"){
    $action_1          = $httpRequest['action_1'];
    $mode_1            = $httpRequest['mode_1'];
    $action_2          = $httpRequest['action_2'];
    $mode_2            = $httpRequest['mode_2'];



    $stmt = $pdo->prepare("INSERT INTO status (m_1, a_1, ut_1, dt_1, m_2, a_2, ut_2, dt_2) 
                          VALUES (:mode_1, :action_1, :uptime_1, :downtime_1, :mode_2, :action_2, :uptime_2, :downtime_2 )");
    

    //For switchv1
    $stmt->bindParam(':mode_1', $mode_1, PDO::PARAM_INT);
    $stmt->bindParam(':action_1', $action_1, PDO::PARAM_INT);

    if($action_1 != 0){
        $ut = date("Y-m-d H:i:s");
        $stmt->bindParam(':uptime_1', $ut, PDO::PARAM_STR);
        $stmt->bindValue(':downtime_1', null, PDO::PARAM_INT);
    }else{
        $dt = date("Y-m-d H:i:s");
        $stmt->bindParam(':downtime_1', $dt, PDO::PARAM_STR);
        $stmt->bindValue(':uptime_1', null, PDO::PARAM_INT);
    }


    //For switch 2
    $stmt->bindParam(':mode_2', $mode_2, PDO::PARAM_INT);
    $stmt->bindParam(':action_2', $action_2, PDO::PARAM_INT);

    if($action_2 != 0){
        $ut = date("Y-m-d H:i:s");
        $stmt->bindParam(':uptime_2', $ut, PDO::PARAM_STR);
        $stmt->bindValue(':downtime_2', null, PDO::PARAM_INT);
        
    }else{
        $dt = date("Y-m-d H:i:s");
        $stmt->bindParam(':downtime_2', $dt, PDO::PARAM_STR);
        $stmt->bindValue(':uptime_2', null, PDO::PARAM_INT);
    }

    $stmt->execute();

    $message = "Success";
    
    echo json_encode($message);
    
}else{
    echo "NO ACTION; ACCESS UNIDENTIFED";
}


