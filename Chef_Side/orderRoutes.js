const express = require('express');
const router = express.Router();
const Order = require('../models/orderModel');
const axios = require('axios');

// Create order
router.post('/', async (req, res) => {
  try {
    const newOrder = new Order(req.body);
    await newOrder.save();
    res.status(201).json(newOrder);
  } catch (err) {
    res.status(400).json({ error: err.message });
  }
});

// Get order by ID
router.get('/:id', async (req, res) => {
  try {
    const order = await Order.findById(req.params.id);
    res.json(order);
  } catch (err) {
    res.status(404).json({ error: 'Order not found' });
  }
});

// Update order status
router.put('/:id/status', async (req, res) => {
  try {
    const order = await Order.findByIdAndUpdate(req.params.id, {
      status: req.body.status,
    }, { new: true });

    // Send to ESP32
    await axios.post('http://192.168.210.220/led', { status: req.body.status });

    res.json(order);
  } catch (err) {
    res.status(400).json({ error: err.message });
  }
});

// Update order status (Accept or Reject)
router.post("/update/:id", async (req, res) => {
  try {
    const orderId = req.params.id;
    const newStatus = req.body.status; // 'accepted' or 'rejected'

    // Find the order by ID and update the status
    const order = await Order.findByIdAndUpdate(
      orderId,
      { status: newStatus },
      { new: true } // Return the updated document
    );

    if (!order) {
      return res.status(404).send("Order not found");
    }

    // Send the updated order back as a response
    res.status(200).json(order);
  } catch (error) {
    console.error("Error updating order status:", error);
    res.status(500).json({ message: "Error updating order status", error: error.message });
  }
});

// In your update order API route:
router.post('/orders/update/:id', async (req, res) => {
  try {
    const order = await Order.findById(req.params.id);

    if (!order) {
      return res.status(404).json({ message: "Order not found" });
    }

    // Update the order status based on the request
    if (req.body.status === 'accepted') {
      order.status = 'accepted';
    } else if (req.body.status === 'rejected') {
      order.status = 'rejected';
    }

    await order.save();

    // Send the updated order status back as a response
    res.json(order);  // This will send back the status of the order
  } catch (error) {
    console.error('Error updating order:', error);
    res.status(500).json({ message: 'Error updating order', error: error.message });
  }
});


module.exports = router;
