<?php
// Configuration
$TOKEN = "#tkn25!@";  // Change to a strong secret
$DATA_FILE = "temperature.dat";

// Validate request
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    http_response_code(405);
    die("Method not allowed");
}

// Get JSON input
$input = json_decode(file_get_contents('php://input'), true);

// Validate token and temperature
if (!isset($input['token']) || $input['token'] !== $TOKEN) {
    http_response_code(401);
    die("Unauthorized");
}

if (!isset($input['temperature']) || !is_numeric($input['temperature'])) {
    http_response_code(400);
    die("Invalid temperature");
}

// Save temperature to file
$temperature = floatval($input['temperature']);
file_put_contents($DATA_FILE, $temperature);

// Send response
echo "Temperature updated: $temperature";
?>