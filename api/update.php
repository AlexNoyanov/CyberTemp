<?php
// Configuration
$TOKEN = "#tkn25!@";  // Your token - MUST MATCH ESP32 TOKEN
$DATA_FILE = dirname(__FILE__) . '/temperature.dat';

// Only allow POST requests
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    http_response_code(405);
    header('Allow: POST');
    die(json_encode(["status" => "error", "message" => "Method not allowed. Use POST."]));
}

// Get JSON input
$json_data = file_get_contents('php://input');
$input = json_decode($json_data, true);

// Check if JSON was parsed successfully
if ($input === null) {
    http_response_code(400);
    die(json_encode(["status" => "error", "message" => "Invalid JSON: " . json_last_error_msg()]));
}

// Validate token
if (!isset($input['token']) || $input['token'] !== $TOKEN) {
    http_response_code(401);
    $received = isset($input['token']) ? $input['token'] : 'none';
    die(json_encode([
        "status" => "error",
        "message" => "Unauthorized",
        "expected" => $TOKEN,
        "received" => $received
    ]));
}

// Validate temperature
if (!isset($input['temperature']) || !is_numeric($input['temperature'])) {
    http_response_code(400);
    die(json_encode(["status" => "error", "message" => "Invalid temperature value"]));
}

// Save temperature to file
$temperature = floatval($input['temperature']);
if (file_put_contents($DATA_FILE, $temperature) === false) {
    http_response_code(500);
    die(json_encode(["status" => "error", "message" => "Failed to save temperature"]));
}

// Send success response
http_response_code(200);
header('Content-Type: application/json');
echo json_encode([
    "status" => "success",
    "message" => "Temperature updated: $temperature",
    "temperature" => $temperature
]);
?>