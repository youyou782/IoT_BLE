#pragma once
#include <BLE.h>
#include <mbed.h>

#include <memory>
#include <vector>

class CGattService : mbed::NonCopyable<CGattService> {
  protected:
	std::vector<std::vector<GattAttribute *>>
		_characteristics_user_descriptions; //!< The user description attributes of all the characteristics
	std::vector<GattCharacteristic *> _characteristics; //!< The characteristics of the service

	std::unique_ptr<GattService> _service; //!< The service encapsulated in unique pointer
  public:
	CGattService();
	~CGattService();
	bool addCharacteristic(const UUID &uuid,
						   const uint8_t properties = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
						   const char *user_description = nullptr,
						   uint8_t *value = nullptr,
						   size_t max_value_size = 0);

	template <typename T>
	bool addCharacteristic(const UUID &uuid,
						   const T &value,
						   const uint8_t properties = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
						   const char *user_description = nullptr) {

		// 1. check whether the service has not been created yet.
		if (_service) {
			return false;
		}
		// 2. Call the base class addCharacteristic for the value type

		const uint8_t *valueBuffer = reinterpret_cast<const uint8_t *>(&value);
		return addCharacteristic(
			uuid, properties, user_description, const_cast<uint8_t *>(valueBuffer), sizeof(T));
	}

	GattCharacteristic *getCharacteristicWithValueHandle(const GattAttribute::Handle_t &value_handle) const;

	const char *getCharacteristicUserDescription(const GattAttribute::Handle_t &value_handle) const;

	virtual void
		onDataWrittenHandler(GattCharacteristic *characteristic, const uint8_t *data, uint16_t size) = 0;

	ble_error_t createService(const UUID &uuid);

	/**
	 * @brief Get a shared pointer to the GattService of this object
	 *
	 * @return A pointer to the GattService of this object. nullptr if the is has been created.
	 */
	GattService *getService() const;
	/**
	 * @brief Get the Characteristic Count added to this service
	 *
	 * @return The number of characteristics
	 */
	unsigned int getCharacteristicCount() const;

	explicit operator bool() const {
		// use the bool operator of std::unique_ptr
		return bool(_service);
	}

	virtual void registerService(ble::BLE &ble);

	/**
	 * @brief On connection completed handler function of the service
	 *
	 */
	virtual void onConnected(void) = 0;

	/**
	 * @brief On disconnected from the client handler function of the service
	 *
	 */
	virtual void onDisconnected(void) = 0;

	template <typename T>
	static ble_error_t getCharacteristicValue(GattCharacteristic &characteristic, T &value) {
		ble::GattServer &server = ble::BLE::Instance().gattServer();

		uint16_t value_size = sizeof(T);
		auto error_code = server.read(characteristic.getValueHandle(), &value, &value_size);

		if (value_size > sizeof(T)) {
			return BLE_ERROR_BUFFER_OVERFLOW;
		}
		return error_code;
	}

	template <typename T>
	static ble_error_t setCharacteristicValue(GattCharacteristic &characteristic, T &value) {
		ble::GattServer &server = ble::BLE::Instance().gattServer();

		auto error_code = server.write(characteristic.getValueHandle(),
									   reinterpret_cast<const uint8_t *>(&value),
									   sizeof(T),
									   false);
		return error_code;
	}
};