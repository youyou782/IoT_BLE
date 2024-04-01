#include "gatt_server.h"

#include <iostream>

#include "ble_utils.h"

CGattServer::CGattServer() { _started = false; }

CGattServer::~CGattServer() { _services.clear(); }

bool CGattServer::addService(CGattService &service) {
	if (_started) {
		std::cout << "server already started, addService failed" << std::endl;
		return false;
	}
	if (service) {
		_services.push_back(&service);
		return true;
	}

	return false;
}

void CGattServer::startServer(ble::BLE &ble) {
	for (auto &i : _services) {
		i->registerService(ble);
		// std::cout << "SSService registered!" << std::endl;
	}

	ble.gattServer().setEventHandler(this);
	_started = true;
}

void CGattServer::onConnected(void) {
	// 1. Iterate over the _services
	// 2. call CGattService::onConnected function
	for (auto &service : _services) {
		service->onConnected();
	}
}

void CGattServer::onDisconnected(void) {
	for (auto &service : _services) {
		service->onDisconnected();
	}
}

void CGattServer::onDataWritten(const GattWriteCallbackParams &e) {
	GattCharacteristic *characteristic = nullptr;
	const char *characteristic_description = nullptr;
	const GattAttribute::Handle_t &value_handle = e.handle;
	std::cout << "onDataWritten() using Conn. Handle " << e.connHandle << " for Att. Handle " << value_handle
			  << std::endl;
	std::cout << "\twrite operation: " << e.writeOp << std::endl;
	std::cout << "\toffset: " << e.offset << std::endl;
	std::cout << "\tlength: " << e.len << std::endl;
	std::cout << "\tdata: ";
	std::cout << bufferToHex(e.data, e.len) << std::endl;

	for (auto &service : _services) {
		characteristic = service->getCharacteristicWithValueHandle(value_handle);
		if (characteristic) {
			characteristic_description = service->getCharacteristicUserDescription(value_handle);
			std::cout << "Characteristic with value handle" << characteristic_description << std::endl;
			service->onDataWrittenHandler(characteristic, e.data, e.len);
			break;
		}
	}

	if (characteristic == nullptr) {
		std::cout << "\tThe characteristic cannot be found" << std::endl;
	}
}

void CGattServer::onDataRead(const GattReadCallbackParams &e) {
	GattCharacteristic *characteristic = nullptr;
	const char *characteristic_description = nullptr;
	const GattAttribute::Handle_t &value_handle = e.handle;
	std::cout << "onDataRead() using Conn. Handle " << e.connHandle << " for Att. Handle " << value_handle
			  << std::endl;
	for (auto &service : _services) {
		characteristic = service->getCharacteristicWithValueHandle(value_handle);
		if (characteristic) {
			characteristic_description = service->getCharacteristicUserDescription(value_handle);
			if (characteristic_description) {
				std::cout << "Characteristic with value handle -> " << characteristic_description
						  << std::endl;
				// service->onDataWrittenHandler(characteristic, e.data, e.len);
			}
			break;
		}
	}

	if (characteristic == nullptr) {
		std::cout << "\tThe characteristic cannot be found" << std::endl;
	}
}

void CGattServer::onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) {
	GattCharacteristic *characteristic = nullptr;
	const char *characteristic_description = nullptr;
	const GattAttribute::Handle_t &value_handle = params.charHandle;
	std::cout << "onUpdatesEnabled() using Conn. Handle " << params.connHandle << " for Att. Handle "
			  << value_handle << std::endl;
	for (auto &service : _services) {
		characteristic = service->getCharacteristicWithValueHandle(value_handle);
		if (characteristic) {
			characteristic_description = service->getCharacteristicUserDescription(value_handle);
			if (characteristic_description) {
				std::cout << "Characteristic with value handle" << characteristic_description << std::endl;
				// service->onDataWrittenHandler(characteristic, e.data, e.len);
			}
			break;
		}
	}

	if (characteristic == nullptr) {
		std::cout << "\tThe characteristic cannot be found" << std::endl;
	}
}

void CGattServer::onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) {
	GattCharacteristic *characteristic = nullptr;
	const char *characteristic_description = nullptr;
	const GattAttribute::Handle_t &value_handle = params.charHandle;
	std::cout << "onUpdatesDisabled() using Conn. Handle " << params.connHandle << " for Att. Handle "
			  << value_handle << std::endl;

	for (auto &service : _services) {
		characteristic = service->getCharacteristicWithValueHandle(value_handle);
		if (characteristic) {
			characteristic_description = service->getCharacteristicUserDescription(value_handle);
			if (characteristic_description) {
				std::cout << "Characteristic with value handle" << characteristic_description << std::endl;
				// service->onDataWrittenHandler(characteristic, e.data, e.len);
			}
			break;
		}
	}

	if (characteristic == nullptr) {
		std::cout << "\tThe characteristic cannot be found" << std::endl;
	}
}

void CGattServer::onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params) {
	GattCharacteristic *characteristic = nullptr;
	const char *characteristic_description = nullptr;
	const GattAttribute::Handle_t &value_handle = params.attHandle;
	std::cout << "onUpdatesDisabled() using Conn. Handle " << params.connHandle << " for Att. Handle "
			  << params.attHandle << std::endl;
	for (auto &service : _services) {
		characteristic = service->getCharacteristicWithValueHandle(value_handle);
		if (characteristic) {
			characteristic_description = service->getCharacteristicUserDescription(value_handle);
			if (characteristic_description) {
				std::cout << "Characteristic with value handle" << characteristic_description << std::endl;
				// service->onDataWrittenHandler(characteristic, e.data, e.len);
			}
			break;
		}
	}

	if (characteristic == nullptr) {
		std::cout << "\tThe characteristic cannot be found" << std::endl;
	}
}
