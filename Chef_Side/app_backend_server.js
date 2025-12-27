const express = require("express");
const mongoose = require("mongoose");
const bodyParser = require("body-parser");
const cors = require("cors");
const path = require("path");
const Order = require("./models/orderModel");
const orderRoutes = require("./routes/orderRoutes");

const app = express();

// Load environment variables
require("dotenv").config();
const connectDB = require("./config/db");

// MongoDB connection
connectDB();

// Set view engine
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));

// Middleware
app.use(cors()); // Allow ESP32 to access API
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true })); // To parse application/x-www-form-urlencoded
app.use(express.static(path.join(__dirname, "public")));
app.use("/orders", orderRoutes); // Ensure this line is prese
// Routes
app.use("/api/orders", orderRoutes);

// Default homepage route to show all orders
app.get("/", async (req, res) => {
  try {
    const orders = await Order.find().sort({ createdAt: -1 });
    res.render("pages/orders", { orders });
  } catch (err) {
    res.status(500).send("Error loading orders");
  }
});

// POST route for handling order creation (API for ESP32)
app.post("/api/orders", async (req, res) => {
  try {
    const order = new Order(req.body); // Create a new order using the data from the request body
    await order.save(); // Save the order to the database

    // Log the order ID to serial monitor
    console.log("Order ID:", order._id); // This will show the order ID in the console/serial monitor

    res.status(200).json({ message: "Order received and saved to database", orderId: order._id });
  } catch (error) {
    console.error("Error saving order:", error);
    res.status(500).json({ message: "Error saving order", error: error.message });
  }
});

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
