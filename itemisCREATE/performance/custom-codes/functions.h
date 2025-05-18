/*
 * functions.h
 *
 *  Created on: Jan 29, 2025
 *      Author: Amin
 */

#ifndef IMPLEMENTATION_CUSTOM_CODES_FUNCTIONS_H_
#define IMPLEMENTATION_CUSTOM_CODES_FUNCTIONS_H_

#include <stdint.h>

class Functions{

public:
    Functions() noexcept;
	uint8_t ack_crc(uint8_t value, uint8_t polynomial);
	bool checkACK(uint8_t ack, uint8_t check);
	uint8_t crc4(uint8_t value, uint8_t t, uint8_t polynomial);
	uint16_t calculation(uint8_t input, uint8_t c_tag);
	uint8_t resetACK(uint8_t ca);
	bool checkPacket(uint16_t p, uint8_t t);
	uint8_t sendData(uint16_t p);
	uint8_t sendAck(uint8_t c);
	uint8_t nextCAck(uint8_t c);
};

#endif /* IMPLEMENTATION_CUSTOM_CODES_FUNCTIONS_H_ */
