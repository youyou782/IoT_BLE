#pragma once
#include <BLE.h>
#include <mbed.h>

#include <memory>
#include <vector>

#include "gatt_service.h"

class CImmediateAlertService : public CGattService {
  public:
	enum AlertLevel { IAS_ALERT_LEVEL_NO_ALERT = 0, IAS_ALERT_LEVEL_MEDIUM = 1, IAS_ALERT_LEVEL_HIGH = 2 };

  private:
	PwmOut _led;

	AlertLevel _alert_level;
	uint8_t _alert_level_value;

  public:
	CImmediateAlertService() = delete;
	CImmediateAlertService(const CImmediateAlertService &) = delete;
	const CImmediateAlertService &operator=(const CImmediateAlertService &) = delete;
	/**
	 * @brief Construct a new CImmediateAlertService object
	 *
	 * 
	 * @param led_pin The LED pin that will be used by the service
	 */
	CImmediateAlertService(PinName led_pin);

	~CImmediateAlertService();

	ble_error_t createService(const char *uuid) = delete;

	virtual void
		onDataWrittenHandler(GattCharacteristic *characteristic, const uint8_t *data, uint16_t size) final;

	virtual void onConnected(void) final;

	virtual void onDisconnected(void) final;

	virtual void registerService(ble::BLE &ble) final;
};