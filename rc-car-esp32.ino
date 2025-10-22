/*
 * ESP32 RC Car Control with L298N Motor Driver
 * 
 * This code creates a WiFi-controlled RC car using ESP32 and L298N motor driver.
 * The car can move forward, backward, turn left, turn right, and stop.
 * 
 * 
 * Author: MTS AUV Team
 * Date: 2025
 */

// Libraries for wifi and server setup
#include <WiFi.h>     
#include <WebServer.h> 

// WiFi credentials 
const char* ssid     = "Shiraz iPhone";
const char* password = "Shiraz1234";

// Web server object on port 80 (default HTTP port)
WebServer server(80);


// Left Motor (Motor A) - Connected to left side of L298N
int motor1Pin1 = 27;  // Connected to IN1 (Left motor direction control 1)
int motor1Pin2 = 26;  // Connected to IN2 (Left motor direction control 2)

// Right Motor (Motor B) - Connected to right side of L298N  
int motor2Pin1 = 33;  // Connected to IN3 (Right motor direction control 1)
int motor2Pin2 = 25;  // Connected to IN4 (Right motor direction control 2)



// WEB PAGE HTML
void handleRoot() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      /* CSS styling for the control interface */
      html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
      .button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; background-color: #4CAF50; border: none; color: white; padding: 12px 28px; text-decoration: none; font-size: 26px; margin: 1px; cursor: pointer; }
      .button2 {background-color: #555555;}
    </style>
    <script>
      /* JavaScript functions to send commands to ESP32 */
      function moveForward() { fetch('/forward'); }   // Send forward command
      function moveLeft() { fetch('/left'); }         // Send left turn command  
      function stopRobot() { fetch('/stop'); }        // Send stop command
      function moveRight() { fetch('/right'); }       // Send right turn command
      function moveReverse() { fetch('/reverse'); }   // Send reverse command
    </script>
  </head>
  <body>
    <h1>ESP32 Motor Control</h1>
    <!-- Control buttons layout -->
    <p><button class="button" onclick="moveForward()">FORWARD</button></p>
    <div style="clear: both;">
      <p>
        <button class="button" onclick="moveLeft()">LEFT</button>
        <button class="button button2" onclick="stopRobot()">STOP</button>
        <button class="button" onclick="moveRight()">RIGHT</button>
      </p>
    </div>
    <p><button class="button" onclick="moveReverse()">REVERSE</button></p>
  </body>
  </html>)rawliteral";
  
  // Send the HTML page to the client's browser
  server.send(200, "text/html", html);
}

// MOTOR CONTROL FUNCTIONS

// FORWARD
void handleForward() {
  Serial.println("Forward");  
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  
  server.send(200); // Send success response to browser
}

// LEFT TURN
void handleLeft() {
  Serial.println("Left");
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  
  server.send(200); 
}

// STOP
void handleStop() {
  Serial.println("Stop");
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, LOW);  
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);   
  
  server.send(200); 
}

// RIGHT TURN
void handleRight() {
  Serial.println("Right");
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);    
  
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);          
  
  server.send(200);
}

// SETUP FUNCTION
void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT); 
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT); 
  pinMode(motor2Pin2, OUTPUT); 
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection with status dots
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print connection success and IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  // Define web server routes (URL endpoints)
  server.on("/", handleRoot);           // Main control page
  server.on("/forward", handleForward); // Forward movement
  server.on("/left", handleLeft);       // Left turn
  server.on("/stop", handleStop);       // Stop motors
  server.on("/right", handleRight);     // Right turn  
  server.on("/reverse", handleReverse); // Reverse movement

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

// MAIN LOOP 
void loop() {
  server.handleClient();
}
