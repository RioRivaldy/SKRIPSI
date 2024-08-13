<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EarthWeather</title>
    <link rel="stylesheet" href="css/page.css">

    <!-- load otomatis real time -->
    <script type="text/javascript">
        $(document).ready(function () {
            setInterval(function () {
                $("#ceksuhu").load("ceksuhu.php");
                $("#cekkelembaban").load("cekkelembaban.php");
                $("#cekmois").load("cekmois.php");
            }, 1000);
        });

    </script>

    <script type="text/javascript" src="jquery/jquery.min.js"></script>
</head>

<body>
    <!-- Navbar -->
    <nav class="navbar">
        <h1 class="logo">EarthWeather</h1>
        <div class="menu-toggle">
            <div></div>
            <div></div>
            <div></div>
        </div>
        <ul class="nav-links">
            <li><a href="#">Home</a></li>
            <li><a href="#">About</a></li>
            <li><a href="#">Services</a></li>
            <li><a href="#">Portfolio</a></li>
            <li><a href="#">Contact</a></li>
        </ul>
    </nav>

    <!-- Main Content -->
    <main>
        <section class="content">
            <h1>Real Time Data Sensors</h1>
            <div class="card-container">
                <div class="card">
                    <h2>Temperature</h2>
                    <img src="image1.jpg" alt="Image 1">
                    <h3> <span id="ceksuhu"> 0 </span> </h3>
                </div>
                <div class="card">
                    <h2>Humidity</h2>
                    <img src="image2.jpg" alt="Image 2">
                    <h3> <span id="cekkelembaban"> 0 </span> </h3>
                </div>
                <div class="card">
                    <h2>Soil Moisture</h2>
                    <img src="image3.jpg" alt="Image 3">
                    <h3> <span id="cekmois"> 0 </span> </h3>
                </div>
            </div>
        </section>
    </main>

    <!-- Footer -->
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
</body>

</html>