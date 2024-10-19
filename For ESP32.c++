#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "XXXXXXX"; // Replace X with your network SSID
const char* password = "XXXXXX"; // Replace X with your network password

WebServer server(80);

const char* html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Snake Game</title>
    <style>
        body { text-align: center; font-family: Arial; }
        canvas { border: 1px solid black; }
    </style>
</head>
<body>
    <h1>Snake Game</h1>
    <h2>Created by github.com/Pey-mma</h2> <!-- Add your name here -->
    <canvas id="gameCanvas" width="400" height="400"></canvas>
    <p>Score: <span id="score">0</span></p>
    <button onclick="startGame()">Start Game</button>
    
    <script>
        const canvas = document.getElementById('gameCanvas');
        const ctx = canvas.getContext('2d');
        let snake = [{x: 10, y: 10}];
        let direction = {x: 0, y: 0};
        let food = {};
        let score = 0;
        let gameInterval;

        function startGame() {
            snake = [{x: 10, y: 10}];
            direction = {x: 0, y: 0};
            score = 0;
            document.getElementById('score').innerText = score;
            placeFood();
            if (gameInterval) clearInterval(gameInterval);
            gameInterval = setInterval(gameLoop, 100);
            document.addEventListener('keydown', changeDirection);
        }

        function placeFood() {
            food.x = Math.floor(Math.random() * (canvas.width / 10)) * 10;
            food.y = Math.floor(Math.random() * (canvas.height / 10)) * 10;
        }

        function changeDirection(event) {
            switch(event.key) {
                case 'ArrowUp':
                    if (direction.y === 0) direction = {x: 0, y: -10};
                    break;
                case 'ArrowDown':
                    if (direction.y === 0) direction = {x: 0, y: 10};
                    break;
                case 'ArrowLeft':
                    if (direction.x === 0) direction = {x: -10, y: 0};
                    break;
                case 'ArrowRight':
                    if (direction.x === 0) direction = {x: 10, y: 0};
                    break;
            }
        }

        function gameLoop() {
            const head = {x: snake[0].x + direction.x, y: snake[0].y + direction.y};

            // Check for wall collisions
            if (head.x < 0 || head.x >= canvas.width || head.y < 0 || head.y >= canvas.height || checkCollision(head)) {
                clearInterval(gameInterval);
                alert('Game Over! Your score was: ' + score);
                return;
            }

            snake.unshift(head);

            // Check for food collision
            if (head.x === food.x && head.y === food.y) {
                score += 10;
                document.getElementById('score').innerText = score;
                placeFood();
            } else {
                snake.pop();
            }

            draw();
        }

        function checkCollision(head) {
            for (let i = 1; i < snake.length; i++) {
                if (head.x === snake[i].x && head.y === snake[i].y) return true;
            }
            return false;
        }

        function draw() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.fillStyle = 'green';
            for (let segment of snake) {
                ctx.fillRect(segment.x, segment.y, 10, 10);
            }
            ctx.fillStyle = 'red';
            ctx.fillRect(food.x, food.y, 10, 10);
        }
    </script>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    // Serve the HTML page
    server.on("/", []() {
        server.send(200, "text/html", html);
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
