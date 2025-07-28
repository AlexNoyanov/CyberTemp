<?php
// Configuration
$DATA_FILE = "temperature.dat";

// Get temperature from file
if (!file_exists($DATA_FILE)) {
    http_response_code(503);
    die("Temperature not available");
}

$temperature = file_get_contents($DATA_FILE);

// Return as JSON
header('Content-Type: application/json');
echo json_encode([
    'temperature' => floatval($temperature),
    'timestamp' => date('c')  // ISO 8601 format
]);
?>