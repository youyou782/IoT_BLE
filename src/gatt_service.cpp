#include "gatt_service.h"
#include <iostream>
#include <memory>

CGattService::CGattService() {}

CGattService::~CGattService() {

	_service.reset();

	for (auto i : _characteristics) {
		delete i;
	}

	for (auto i : _characteristics_user_descriptions) {
		for (auto j : i) {
			delete j;
		}
	}
}

bool CGattService::addCharacteristic(
	const UUID &uuid,
	const uint8_t properties /*= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE*/,
	const char *user_description /*= nullptr*/,
	uint8_t *value /*= nullptr*/,
	size_t max_value_size /*= 0*/) {

	if (_service) {
		// if it is, we cannot add characteristics to it.
		return false;
	}

	std::vector<GattAttribute *> characteristicDescriptors;

	GattAttribute *userDescriptionAtt = new GattAttribute(
		BLE_UUID_DESCRIPTOR_CHAR_USER_DESC, /*Bluetooth defined UUID for CHARACTERISTIC_DESCRIPTOR*/
		(uint8_t *)user_description,		/*Value of this attribute*/
		strlen(user_description) + 1,		/*size of the initial value*/
		strlen(user_description) + 1,		/*maximum size the attribute value might have*/
		false /*this attribute has fixed size */);

	assert(userDescriptionAtt != nullptr && "Memory allocation for userDescriptionAtt failed");
	characteristicDescriptors.emplace_back(userDescriptionAtt);

	_characteristics_user_descriptions.emplace_back(characteristicDescriptors);

	GattCharacteristic *gattCharacteristic = new GattCharacteristic(
		uuid,						  /*UUID of the characteristic*/
		value,						  /*Memory buffer holding the initial value. */
		(value ? max_value_size : 0), /*The size in bytes of the characteristic's value*/
		max_value_size,				  /*The capacity in bytes of the characteristic value buffer*/
		properties,					  /*An 8-bit field that contains the characteristic's properties*/
		_characteristics_user_descriptions.back()
			.data(), /*A pointer to an array of descriptors to be included within this characteristic.*/
		1,			 /*The number of descriptors presents in descriptors array*/
		false /*Flag that indicates if the attribute's value has variable length*/);

	assert(gattCharacteristic != nullptr && "Memory allocation for gattCharacteristic failed");
	_characteristics.emplace_back(gattCharacteristic);
	return true;
}

GattCharacteristic *
	CGattService::getCharacteristicWithValueHandle(const GattAttribute::Handle_t &value_handle) const {

	for (auto &characteristic : _characteristics) {
		if (characteristic->getValueHandle() == value_handle) {
			return characteristic;
		}
	}
	return nullptr;
}

const char *
	CGattService::getCharacteristicUserDescription(const GattAttribute::Handle_t &value_handle) const {

	int characteristic_index = 0;

	for (size_t i = 0; i < _characteristics.size(); i++) {
		if (_characteristics[i]->getValueHandle() == value_handle) {
			return reinterpret_cast<const char *>(_characteristics_user_descriptions[i][0]->getValuePtr());
			// return temp;
		}
	}
	return nullptr;
}

ble_error_t CGattService::createService(const UUID &uuid) {

	assert(_service == nullptr && "Service created multiple times");

	if (_characteristics.empty()) {
		return BLE_ERROR_INVALID_STATE;
	}

	_service = std::make_unique<GattService>(
		uuid, /*UUID of the service*/
		_characteristics.data() /*A pointer to the array of characteristics that belongs to the service.*/,
		_characteristics.size() /*The number of characteristics*/);

	assert(_service != nullptr && "Service creation failed");
	return BLE_ERROR_NONE;
}

GattService *CGattService::getService() const {
	if (_service != nullptr) {
		return nullptr;
	} else {
		return nullptr;
	}
}

unsigned int CGattService::getCharacteristicCount() const { return _characteristics.size(); }

void CGattService::registerService(ble::BLE &ble) {
	std::cout << "Registering services" << std::endl;
	ble.gattServer().addService(*_service);
	std::cout << "Register successful" << std::endl;
}