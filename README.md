# Digi-Dine  
## Wireless E-Menu and Kitchen Feedback System

---

## 📌 Project Overview
Digi-Dine is a **wireless digital food ordering system** that replaces traditional manual order-taking with an **ESP32-based electronic menu**. The system enables customers to place orders digitally, which are transmitted over Wi-Fi to a centralized backend and displayed in real time on a **chef-facing web dashboard**.

The solution improves **order accuracy, operational efficiency, and customer experience** by establishing reliable end-to-end communication between the customer interface and the kitchen.

---

## 🧩 System Architecture
The project follows a **client–server model** consisting of three core layers:

### 1. Customer Side (Embedded System)
- ESP32 microcontroller acts as the client device
- OLED display shows menu items
- 4×4 matrix keypad enables user input
- Matrix scanning implemented using ESP32 GPIOs
- Orders are processed locally and sent via Wi-Fi

### 2. Backend Server
- Node.js server handles incoming API requests
- Token-based authentication ensures secure communication
- Orders are validated and stored in MongoDB
- RESTful APIs manage order lifecycle

### 3. Chef Side (Web Dashboard)
- Web interface displays incoming orders in real time
- JavaScript and EJS used for dynamic rendering
- Enables kitchen staff to monitor and manage orders efficiently

---

## 🚀 Key Features
- Wireless digital ordering using ESP32
- OLED-based menu visualization
- 4×4 keypad with matrix scanning logic
- Secure Wi-Fi communication with authentication tokens
- REST API-based order transmission
- MongoDB-backed persistent storage
- Live order dashboard for kitchen staff
- End-to-end system integration and testing

---

## 🛠️ Technology Stack

### Hardware
- ESP32 Microcontroller  
- OLED Display  
- 4×4 Matrix Keypad  

### Software
- Embedded C (ESP32 / Arduino IDE)
- Node.js (Backend Server)
- MongoDB (Database)
- JavaScript & EJS (Frontend)
- REST APIs (Client–Server Communication)

---

## 📂 Project Structure
```

TE MINI PROJECT - DIGIDINE
│
├── Chef_Side
│   ├── app_backend_server.js
│   ├── db.js
│   ├── order.js
│   ├── orderModel.js
│   ├── orderRoutes.js
│   ├── main.ejs
│   ├── navbar.ejs
│   ├── footer.ejs
│   ├── orders.ejs
│   ├── package.json
│   └── package-lock.json
│
├── Customer_Side
│   ├── HTTP_client
│   ├── OLED_logic
│   ├── Wifi_Connectivity
│   └── Final_Customerside_latest.ino
│
├── mini_project_synopsis.pdf
└── README.md

```

---

## 🔐 Communication Flow
1. Customer selects menu items using keypad
2. ESP32 processes input and displays feedback on OLED
3. Order data is sent to backend via Wi-Fi
4. Node.js server authenticates and stores order in MongoDB
5. Chef dashboard fetches and displays orders in real time

---

## ✅ Testing & Validation
- GPIO and keypad scanning tested for accuracy
- OLED rendering validated for menu clarity
- Wi-Fi connectivity tested under multiple conditions
- API calls verified for authentication and reliability
- End-to-end order flow tested from customer to kitchen

---

## 📈 Outcome
Digi-Dine successfully demonstrates a **real-world embedded + web integration system**, showcasing skills in:
- Embedded systems development
- Hardware–software integration
- Backend API design
- Database management
- Full-stack system testing

---

## 👤 Developer
**Shreya**  
Final Year B.E. – Electronics & Telecommunication Engineering  

---

## 📄 License
This project is developed for academic purposes.
```

---

### Bottom line

This README:

* Works for **college evaluation**
* Works for **placements**
* Works for **GitHub portfolio**
* Uses **industry-standard terminology**
* Clearly shows **your technical ownership**

If you want:

* a **shorter ATS-only version**
* a **GitHub “star-worthy” version**
* or a **resume-ready project description**

say the word.
