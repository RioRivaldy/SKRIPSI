<?php
     // buat koneksi ke database
     $konek = mysqli_connect("localhost", "root", "", "db_earthweather");

     // baca data yang dikirim dari ESP8266
     // nilai suhu sama yang Arduino
     $suhu = $_GET['suhu'];
     $kelembaban = $_GET['kelembaban'];
     $mois = $_GET['mois'];
   // $jarak = $_GET['jarak'];
   // $cuaca = $_GET['cuaca'];

     // auto-increment = 1
     mysqli_query($konek, "ALTER TABLE tb_sensor AUTO_INCREMENT=1");

     // simpan ke tabel tb_sensor
     $simpan = mysqli_query($konek, "insert into tb_sensor(suhu, kelembaban, mois)values('$suhu', '$kelembaban', '$mois')");

     if($simpan)
        echo "Berhasil Dikirim";
     else
        echo "Gagal Terkirim";
?>