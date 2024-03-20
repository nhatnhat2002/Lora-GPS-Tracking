const map = L.map('map'); 

// Set initial view coordinates and zoom level
map.setView([51.505, -0.09], 10); 

// Add OpenStreetMap tile layer
L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '© OpenStreetMap'
}).addTo(map); 

// Define variables for marker, circle, and zoomed state
let marker, circle, zoomed;

// Array to store passed positions
const passedPositions = [];

// Function to fetch data from an API endpoint
async function getDataFromEndpoint(endpoint) {
    try {
        const response = await fetch(endpoint);

        if (!response.ok) {
            throw new Error('Network response was not ok');
        }

        const data = await response.json();
        return data[data.length - 1]; // Return the last document from the data array
    } catch (error) {
        console.error('Error fetching data:', error);
        return null;
    }
}

// Success callback function for geolocation
function success(pos) {
    // Define the endpoint URL for fetching data
    const endpointUrl = 'https://us-east-1.aws.data.mongodb-api.com/app/application-0-vqinw/endpoint/getdata';
    
    // Fetch data from the endpoint
    getDataFromEndpoint(endpointUrl)
        .then(data => {
            if (data) {
                console.log('Data from endpoint:', data);

                // Parse latitude and longitude from the fetched data
                const lat = parseFloat(data.lat);
                const lng = parseFloat(data.lng);

                // Update the content of the coordinates div with the new latitude and longitude values
                document.getElementById('lat').textContent = lat.toFixed(6);
                document.getElementById('lng').textContent = lng.toFixed(6);

                // Get accuracy of the geolocation
                const accuracy = 5;

                // Remove existing marker and circle layers from the map
                if (marker) {
                    map.removeLayer(marker);
                    map.removeLayer(circle);
                }

                // Clear passedPositions array
                passedPositions.splice(0, passedPositions.length);

                // Add marker and circle layers to the map
                marker = L.marker([lat, lng]).addTo(map);
                circle = L.circle([lat, lng], { radius: accuracy }).addTo(map);

                // Create markers for passed positions
                for (const position of passedPositions) {
                    L.marker(position).addTo(map);
                }

                // Fit map view to the circle bounds
                if (!zoomed) {
                    zoomed = map.fitBounds(circle.getBounds());
                }

                // Set map view to the current location
                map.setView([lat, lng]);
            } else {
                console.log('No data received from endpoint.');
            }
        })
        .catch(error => console.error('Error:', error));
}

// Error callback function for geolocation
function error(err) {
    if (err.code === 1) {
        alert("Please allow geolocation access");
    } else {
        alert("Cannot get current location");
    }
}

// Watch for changes in geolocation and call the success or error functions accordingly
function updatePosition() {
    navigator.geolocation.getCurrentPosition(success, error);
}

// Update position every 5 seconds
setInterval(updatePosition, 5000);