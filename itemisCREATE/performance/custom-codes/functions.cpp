/*
 * functions.cpp
 *
 *  Created on: Jan 29, 2025
 *      Author: Amin
 */

#include "functions.h"
//#include <QDebug>

Functions::Functions() noexcept {}

uint8_t Functions::ack_crc(uint8_t value, uint8_t polynomial) {
    uint8_t crc = 0; // Initialize the CRC register to 0

    // Loop over each bit in the input byte (MSB first)
    for (int i = 7; i >= -4; --i) {
        // XOR the MSB of the input with the MSB of the CRC register
        crc ^= (value >> i) & 1; // Shift input and XOR

        // If the MSB of the CRC is 1, perform the polynomial division (XOR)
        if ((crc & 16) != 0) { // Check if bit 4 (fifth bit from LSB in an 8-bit number) is set
            crc ^= polynomial; // Perform division by XOR with polynomial
        }

        if (i != -4) {
            // Shift CRC register left by 1 bit
            crc <<= 1;
        }
    }

    // Final CRC value is the remainder in the lowest 4 bits
    return crc & 15; // Mask to get the lower 4 bits as the CRC result
}

uint8_t Functions::crc4(uint8_t value, uint8_t t, uint8_t polynomial) {
    uint16_t crc = 0; // Initialize the CRC register to 0
    uint16_t input = (static_cast<uint16_t>(value) | (static_cast<uint16_t>(t) << 8));

    // Loop over each bit in the input value (MSB first, 16 bits)
    for (int i = 15; i >= -4; --i) {
        // XOR the MSB of the input with the MSB of the CRC register
        crc ^= (input >> i) & 1; // Shift input right and XOR with the least significant bit of crc

        // If the MSB of the CRC is 1, perform the polynomial division (XOR)
        if ((crc & 16) != 0) { // Check if the fifth bit is set
            crc ^= polynomial; // Perform division by XOR with polynomial
        }

        if (i != -4) {
            // Shift CRC register left by 1 bit
            crc <<= 1;
        }
    }

    // Final CRC value is the remainder in the lowest 4 bits
    return static_cast<uint8_t>(crc & 15); // Mask to get the lower 4 bits as the CRC result
}

uint16_t Functions::calculation(uint8_t input, uint8_t c_tag) {
    uint16_t output = static_cast<uint16_t>(input) | // Cast and OR the input
                      (static_cast<uint16_t>(c_tag) << 8) | // Shift c_tag left by 8 bits and OR
                      (static_cast<uint16_t>(crc4(input, c_tag, 19)) << 9); // Compute CRC, shift it left by 9 bits and OR

    return output;
}

bool Functions::checkACK(uint8_t ack, uint8_t check) {
    uint8_t header = ack >> 4; // Right shift by 4 bits
    uint8_t ack_value = ack & 0x0F; // Bitwise AND with 0x0F to get the lower 4 bits

    if (ack_crc(ack_value, 19) == header && ack_value == check) {
        return true;
    } else {
        return false;
    }
}

uint8_t Functions::resetACK(uint8_t ca) {
    uint8_t new_ca;
    if (ca == 15) {
        new_ca = 0; // Reset the counter when it reaches 15
    } else {
        new_ca = ca + 1; // Increment the counter otherwise
    }
    return new_ca;
}

bool Functions::checkPacket(uint16_t p, uint8_t t) {
    uint8_t header = static_cast<uint8_t>(p >> 9); // Extract header by shifting right
    uint8_t raw_data = static_cast<uint8_t>(p & 0x00FF); // Extract raw data using bitwise AND
    uint8_t checktag = (p & (1 << 8)) != 0; // Extract checktag from bit 9

//    qDebug() << crc4(raw_data, t, 19);
    return (crc4(raw_data, t, 19) == header) && (checktag == t);
}

uint8_t Functions::sendData(uint16_t p) {
    return static_cast<uint8_t>(p & 0x00FF);
}

uint8_t Functions::sendAck(uint8_t c) {
    uint8_t crc_value = ack_crc(c, 19); // Assuming 19 is the polynomial
    return c | (crc_value << 4);
}

uint8_t Functions::nextCAck(uint8_t c) {
    return (c == 15) ? 0 : (c + 1);
}

