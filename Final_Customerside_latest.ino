adv code for esp32
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Keypad.h>
#include <U8g2lib.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "abc";
const char* password = "12345678";

// Server API endpoints
const char* serverURL = "http://192.168.210.220:3000/api/orders";
String orderStatusURL = "";

// OLED display setup
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Web server
WebServer server(80);

// Menu Data
String categories[] = {"Appetizers", "Main Course", "Desserts", "Beverages"};
String items[4][3] = {
  {"Spring Rolls", "Samosas", "Bruschetta"},
  {"Chicken Biryani", "Paneer Butter Masala", "Pasta Primavera"},
  {"Gulab Jamun", "Cheesecake", "Chocolate Lava Cake"},
  {"Coke", "Lemonade", "Iced Tea"}
};
float prices[4][3] = {
  {150, 120, 180},
  {300, 250, 350},
  {100, 200, 250},
  {50, 60, 70}
};

struct CartItem {
  String name;
  float price;
  int quantity;
};
CartItem cart[10];
int cartIndex = 0;

int currentLevel = 0;
int selectedCategory = -1;
String lastOrderId = "";
String lastStatus = "";
unsigned long lastPollTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(18, OUTPUT); // Green LED
  pinMode(19, OUTPUT); // Red LED

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  displayCategories();

  server.on("/led", HTTP_POST, handleLEDControl);
  server.begin();
  Serial.println("ESP32 HTTP Server started.");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    if (currentLevel == 0) handleCategorySelection(key);
    else if (currentLevel == 1) handleItemSelection(key);
    else if (currentLevel == 2) handleCartActions(key);
  }

  if (lastOrderId != "" && millis() - lastPollTime > 3000) {
    checkOrderStatus();
    lastPollTime = millis();
  }

  server.handleClient();
}

void displayCategories() {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print("Select Category:");
  for (int i = 0; i < 4; i++) {
    u8g2.setCursor(0, 22 + i * 12);
    u8g2.printf("%d. %s", i + 1, categories[i].c_str());
  }
  u8g2.sendBuffer();
}

void displayItems(int categoryIndex) {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.printf("Items: %s", categories[categoryIndex].c_str());
  for (int i = 0; i < 3; i++) {
    u8g2.setCursor(0, 22 + i * 12);
    u8g2.printf("%d. %s - ₹%.2f", i + 1, items[categoryIndex][i].c_str(), prices[categoryIndex][i]);
  }
  u8g2.sendBuffer();
}

void displayCart() {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print("Your Cart:");
  float total = 0;
  for (int i = 0; i < cartIndex; i++) {
    u8g2.setCursor(0, 22 + (i * 12));
    u8g2.printf("%s x%d - ₹%.2f", cart[i].name.c_str(), cart[i].quantity, cart[i].price);
    total += cart[i].price * cart[i].quantity;
  }
  u8g2.setCursor(0, 22 + (cartIndex * 12) + 10);
  u8g2.printf("Total: ₹%.2f", total);
  u8g2.sendBuffer();
}

void handleCategorySelection(char key) {
  if (key >= '1' && key <= '4') {
    selectedCategory = key - '1';
    currentLevel = 1;
    displayItems(selectedCategory);
  }
}

void handleItemSelection(char key) {
  if (key >= '1' && key <= '3') {
    int itemIndex = key - '1';
    addToCart(items[selectedCategory][itemIndex], prices[selectedCategory][itemIndex]);
  } else if (key == '*') {
    currentLevel = 0;
    displayCategories();
  } else if (key == '#') {
    currentLevel = 2;
    displayCart();
  }
  
}


void handleCartActions(char key) {
  if (key == '#') {
    sendOrderToServer();
  } else if (key == '*') {
    currentLevel = 1;
    displayItems(selectedCategory);
  }
}

void addToCart(String itemName, float itemPrice) {
  for (int i = 0; i < cartIndex; i++) {
    if (cart[i].name == itemName) {
      cart[i].quantity++;
      return;
    }
  }
  cart[cartIndex].name = itemName;
  cart[cartIndex].price = itemPrice;
  cart[cartIndex].quantity = 1;
  cartIndex++;
}

void sendOrderToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<500> doc;
    doc["tableNumber"] = 1;
    JsonArray itemsArray = doc.createNestedArray("items");
    for (int i = 0; i < cartIndex; i++) {
      JsonObject item = itemsArray.createNestedObject();
      item["name"] = cart[i].name;
      item["price"] = cart[i].price;
      item["quantity"] = cart[i].quantity;
    }

    String payload;
    serializeJson(doc, payload);
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
      StaticJsonDocument<300> resDoc;
      deserializeJson(resDoc, response);
      lastOrderId = resDoc["_id"].as<String>();
      orderStatusURL = "http://192.168.210.220:3000/api/orders/" + lastOrderId;
      Serial.println("Order ID: " + lastOrderId);
    } else {
      Serial.println("Error: " + String(httpResponseCode));
    }
    http.end();
  }
}

void checkOrderStatus() {
  if (WiFi.status() == WL_CONNECTED && orderStatusURL != "") {
    HTTPClient http;
    http.begin(orderStatusURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String response = http.getString();
      StaticJsonDocument<300> doc;
      deserializeJson(doc, response);
      String status = doc["status"].as<String>();

      if (status != lastStatus) {
        lastStatus = status;
        if (status == "accepted") {
          Serial.println("Order Accepted!");
          digitalWrite(18, HIGH);
          delay(2000);
          digitalWrite(18, LOW);
        } else if (status == "rejected") {
          Serial.println("Order Rejected!");
          digitalWrite(19, HIGH);
          delay(2000);
          digitalWrite(19, LOW);
        }
      }
    } else {
      Serial.println("Status Check Error: " + String(httpResponseCode));
    }
    http.end();
  }
}

void handleLEDControl() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (!error) {
      String status = doc["status"];
      if (status == "accepted") {
        digitalWrite(18, HIGH);
        delay(2000);
        digitalWrite(18, LOW);
        Serial.println("Green LED blinked (accepted)");
      } else if (status == "rejected") {
        digitalWrite(19, HIGH);
        delay(2000);
        digitalWrite(19, LOW);
        Serial.println("Red LED blinked (rejected)");
      }
    }
  }
  server.send(200, "text/plain", "LED control done");
}
