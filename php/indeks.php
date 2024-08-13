<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EarthWeather</title>
    <link rel="stylesheet" href="css/index.css">
    <!-- <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Bebas+Neue&family=Poppins&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.11.0/css/all.css"> -->


    <script type="text/javascript" src="jquery/jquery-3.7.1.min.js"></script>

    <!-- load otomatis real time -->
    <script type="text/javascript">
        $(document).ready(function () {
            setInterval(function () {
                $("#ceksuhu").load("ceksuhu.php");
                $("#cekkelembaban").load("cekkelembaban.php");
                $("#cekmois").load("cekmois.php");
                $("#cekjarak").load("cekjarak.php");
            }, 1000);
        });

    </script>

</head>

<body>
    <!-- Navbar -->
    <nav class="navbar">
        <h1 class="logo"><a href="landingpage.html" style="text-decoration: none; color: white;">EarthWeather</a></h1>
        <div class="menu-toggle">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <ul class="nav-links">
            <li><a href="homepage.html">Home</a></li>
            <li><a href="index.php">Data Sensors</a></li>
            <li><a href="#">Data Weather</a></li>
            <li><a href="#">Portfolio</a></li>
            <li><a href="#">Contact</a></li>
        </ul>
    </nav>

    <main>
        <section class="content">
            <h1>Real Time Data Sensors</h1>
            <div class="card-container">
                <div class="card">
                    <h2>Temperature</h2>
                    <img src="images/suhu.png" alt="Image 1" style="width: 120px; height: 140px;">
                    <h3> <span id="ceksuhu"> 0 </span> °C </h3>
                </div>
                <div class="card">
                    <h2>Humidity</h2>
                    <img src="images/humidity.png" alt="Image 2" style="width: 120px; height: 120px;">
                    <h3> <span id="cekkelembaban"> 0 </span> % </h3>
                </div>
                <div class="card">
                    <h2>Soil Moistu re</h2>
                    <img src="images/mois.png" alt="Image 3" style="width: 120px; height: 120px;">
                    <h3> <span id="cekmois"> 0 </span> % </h3>
                </div>
                <div class="card">
                    <h2>Distance Tank</h2>
                    <img src="images/jarak.png" alt="Image 4" style="width: 120px; height: 120px;">
                    <h3> <span id="cekjarak"> 0 </span> cm </h3>
                </div>
            </div>
        </section>
    </main>

    <footer>
        <p>&copy; 2023 EarthWeather. All rights reserved.</p>
    </footer>

    <script>
        document.addEventListener("DOMContentLoaded", function () {
            const menuToggle = document.querySelector('.menu-toggle');
            const navLinks = document.querySelector('.nav-links');

            menuToggle.addEventListener('click', function () {
                navLinks.classList.toggle('show');
            });
        });
    </script>

  <!-- <script src="jquery/weather.js"></script> -->
  <!-- <script src="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.11.0/js/all.js"></script> -->

</body>

</html>