let currentPage = 1;
let totalPages = 1;

document.addEventListener('DOMContentLoaded', function () {
    fetchData(currentPage);
});

function fetchData(page) {
    fetch(`fetchdata.php?page=${page}`)
        .then(response => response.json())
        .then(response => {
            const tableBody = document.querySelector('#sensorTable tbody');
            tableBody.innerHTML = ''; // Clear existing data

            response.data.forEach((row, index) => {
                const tr = document.createElement('tr');
                tr.innerHTML = `
                    <td>${index + 1 + (page - 1) * 20}</td>
                    <td>${row.id}</td>
                    <td>${row.suhu}</td>
                    <td>${row.kelembaban}</td>
                    <td>${row.mois}</td>
                    <td>${row.jarak}</td>
                    <td>${row.timestamp}</td>
                `;
                tableBody.appendChild(tr);
            });

            totalPages = response.totalPages;
            updatePagination(page, totalPages);
        })
        .catch(error => console.error('Error fetching data:', error));
}

function updatePagination(currentPage, totalPages) {
    const pageNumbers = document.getElementById('pageNumbers');
    pageNumbers.innerHTML = '';

    const maxVisiblePages = 5;
    let startPage = Math.max(currentPage - Math.floor(maxVisiblePages / 2), 1);
    let endPage = Math.min(startPage + maxVisiblePages - 1, totalPages);

    if (endPage - startPage < maxVisiblePages - 1) {
        startPage = Math.max(endPage - maxVisiblePages + 1, 1);
    }

    for (let i = startPage; i <= endPage; i++) {
        const pageNumber = document.createElement('span');
        pageNumber.textContent = i;
        pageNumber.classList.add('page-number');
        if (i === currentPage) {
            pageNumber.classList.add('active');
        }
        pageNumber.addEventListener('click', () => fetchPage(i));
        pageNumbers.appendChild(pageNumber);
    }

    document.getElementById('pageInput').max = totalPages;
    document.getElementById('pageInput').value = currentPage;

    document.getElementById('prevPage').disabled = currentPage <= 1;
    document.getElementById('nextPage').disabled = currentPage >= totalPages;

    // Tampilkan jumlah total halaman
    const totalPagesText = document.getElementById('totalPages');
    totalPagesText.textContent = `Total Pages: ${totalPages}`;
}

function fetchPage(page) {
    currentPage = page;
    fetchData(page);
}

function prevPage() {
    if (currentPage > 1) {
        currentPage--;
        fetchData(currentPage);
    }
}

function nextPage() {
    if (currentPage < totalPages) {
        currentPage++;
        fetchData(currentPage);
    }
}

function goToPage() {
    const pageInput = document.getElementById('pageInput');
    const page = parseInt(pageInput.value);
    if (page >= 1 && page <= totalPages) {
        fetchPage(page);
    } else {
        alert('Page number out of range');
    }
}