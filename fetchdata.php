<?php
// buat koneksi ke database
$konek = mysqli_connect("localhost", "root", "", "db_earthweather");

// ambil halaman dari parameter GET
$page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
$limit = 20; // jumlah data per halaman
$offset = ($page - 1) * $limit;

// ambil data dari tabel tb_sensor dengan limit dan offset
$query = mysqli_query($konek, "SELECT * FROM tb_sensor ORDER BY id DESC LIMIT $limit OFFSET $offset");
$data = array();

while($row = mysqli_fetch_assoc($query)) {
    $data[] = $row;
}

// ambil total jumlah data
$totalQuery = mysqli_query($konek, "SELECT COUNT(*) as total FROM tb_sensor");
$totalData = mysqli_fetch_assoc($totalQuery)['total'];

$response = [
    'data' => $data,
    'total' => $totalData,
    'page' => $page,
    'totalPages' => ceil($totalData / $limit)
];

// kembalikan data dalam format JSON
echo json_encode($response);
?>
