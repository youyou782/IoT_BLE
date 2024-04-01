#include "immediate_alert_service.h"

CImmediateAlertService::CImmediateAlertService(PinName led_pin)
	: _led(led_pin), _alert_level(IAS_ALERT_LEVEL_NO_ALERT), _alert_level_value(IAS_ALERT_LEVEL_NO_ALERT) {

	// 1. set the PWM period to 1 seconds
	_led.period(1.0f);

	//    assert that the characteristic is successfully created
	const char *description = "IAS";

	assert(
		this->addCharacteristic(
			GattCharacteristic::UUID_ALERT_LEVEL_CHAR,
			GattCharacteristic::
				BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE /*=
																   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE*/
			,
			description /*= nullptr*/,
			&_alert_level_value /*= nullptr*/,
			sizeof(_alert_level_value) /*= 0*/) &&
		"error adding characteristic");

	for (auto i : _characteristics) {
		i->setSecurityRequirements(ble::att_security_requirement_t::UNAUTHENTICATED,
								   ble::att_security_requirement_t::UNAUTHENTICATED,
								   ble::att_security_requirement_t::UNAUTHENTICATED);
	}

	//    assert that the service is successfully created
	assert(!(this->CGattService::createService(GattService::UUID_IMMEDIATE_ALERT_SERVICE)) &&
		   "error creating service");
}

CImmediateAlertService::~CImmediateAlertService() {}

void CImmediateAlertService::onDataWrittenHandler(GattCharacteristic *characteristic,
												  const uint8_t *data,
												  uint16_t size) {

	if (size != sizeof(AlertLevel))
		return;

	_alert_level = static_cast<AlertLevel>(*data);
	_alert_level_value = *data;

	setCharacteristicValue(*_characteristics[0], _alert_level_value);

	switch (_alert_level) {

	case IAS_ALERT_LEVEL_NO_ALERT:
		// 4. 根据警报级别设置PWM占空比
		_led.write(1.0f); // NO_ALERT -> OFF (100%)
		break;
	case IAS_ALERT_LEVEL_MEDIUM:
		_led.write(0.5f); // MEDIUM -> 约50%
		break;
	case IAS_ALERT_LEVEL_HIGH:
		_led.write(0.0f); // HIGH -> ON (0%)
		break;
	default:
		// 处理未知警报级别
		break;
	}
}

void CImmediateAlertService::onConnected(void) {

	_alert_level = IAS_ALERT_LEVEL_NO_ALERT;
	_alert_level_value = IAS_ALERT_LEVEL_NO_ALERT;
	// set the characteristic value
	// ble::GattServer &server = ble::BLE::Instance().gattServer();

	setCharacteristicValue(*_characteristics[0], _alert_level_value);

	switch (_alert_level) {

	case IAS_ALERT_LEVEL_NO_ALERT:
		// 4. 根据警报级别设置PWM占空比
		_led.write(1.0f); // NO_ALERT -> OFF (100%)
		break;
	case IAS_ALERT_LEVEL_MEDIUM:
		_led.write(0.5f); // MEDIUM -> 约50%
		break;
	case IAS_ALERT_LEVEL_HIGH:
		_led.write(0.0f); // HIGH -> ON (0%)
		break;
	default:
		break;
	}
}

void CImmediateAlertService::onDisconnected(void) {

	_alert_level = IAS_ALERT_LEVEL_MEDIUM;
	_alert_level_value = IAS_ALERT_LEVEL_MEDIUM;

	setCharacteristicValue(*_characteristics[0], _alert_level_value);

	switch (_alert_level) {

	case IAS_ALERT_LEVEL_NO_ALERT:
		// 4. 根据警报级别设置PWM占空比
		_led.write(1.0f); // NO_ALERT -> OFF (100%)
		break;
	case IAS_ALERT_LEVEL_MEDIUM:
		_led.write(0.5f); // MEDIUM -> 约50%
		break;
	case IAS_ALERT_LEVEL_HIGH:
		_led.write(0.0f); // HIGH -> ON (0%)
		break;
	default:

		break;
	}
}

void CImmediateAlertService::registerService(ble::BLE &ble) { CGattService::registerService(ble); }
