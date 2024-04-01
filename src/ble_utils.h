#ifndef _BLE_UTILS_H_
#define _BLE_UTILS_H_

#include <BLE.h>

#include <iomanip>
#include <sstream>

std::string bluetoothAddressToString(const ble::address_t &address);
std::string bluetoothAddressToString(const ble::own_address_type_t type, const ble::address_t &address);
std::string bluetoothAddressToString(const ble::peer_address_type_t type, const ble::address_t &address);
std::string bufferToHex(const uint8_t *buffer, size_t size);
std::string passKeyToString(const SecurityManager::Passkey_t passkey);
std::string bleErrorToString(ble_error_t error, const char *message = nullptr);
#endif //! _BLE_UTILS_H_