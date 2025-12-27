const mongoose = require("mongoose");

const orderSchema = new mongoose.Schema(
    {
        tableNumber: { type: Number, required: true },
        items: [{ name: String, price: Number, quantity: Number }],
        status: { type: String, default: "pending" } // pending, accepted, rejected, cooking, ready
    },
    { timestamps: true }
);

module.exports = mongoose.model("Order", orderSchema);
