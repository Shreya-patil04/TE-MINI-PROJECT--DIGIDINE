const mongoose = require('mongoose');

const orderSchema = new mongoose.Schema({
  tableNumber: Number,
  items: [
    {
      name: String,
      price: Number,
      quantity: Number
    }
  ],
  status: {
    type: String,
    default: 'pending'
  }
});

module.exports = mongoose.model('Order', orderSchema);
