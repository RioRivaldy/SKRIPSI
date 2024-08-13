<?php
    // buat koneksi ke database
    $konek = mysqli_connect("localhost", "root", "", "db_earthweather");

    // baca data dari tabel tb_sensor
    $sql = mysqli_query($konek, "select * from tb_sensor order by id desc"); // data terakhir akan berada diatas 
    
    // baca data paling atas
    $data = mysqli_fetch_array($sql);
    $cuaca = $data['cuaca'];

    // uji, apabila nilai kelembaban belum ada, maka anggap suhu = 0
    if($cuaca == "") $cuaca = 0;

    // cetak nilai suhu
    echo $cuaca;

?>