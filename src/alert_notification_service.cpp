#include "alert_notification_service.h"

void CAlertNotificationService::buttonPressedHandler() {
	_button_press_count++;
	// add a new alert to the ANS_TYPE_SIMPLE_ALERT category
	if (_alert_status[ANS_TYPE_SIMPLE_ALERT].fields.count < 255) { // 防止溢出
		_alert_status[ANS_TYPE_SIMPLE_ALERT].fields.count++;
	}
	setCharacteristicValue(*_characteristics[4], _alert_status);
	int temp = 1;
	setCharacteristicValue(*_characteristics[3], temp);
	setCharacteristicValue(*_characteristics[2], _button_press_count);
}

CAlertNotificationService::CAlertNotificationService(PinName button_pin)
	: _button_press_count(0), _button(button_pin) {
	_supported_new_alert_category_index = 0;
	_supported_unread_alert_category_index = 1;
	_unread_alert_status_index = 2;
	_new_alert_index = 3;
	_alert_notification_control_point_index = 4;

	_supported_new_alert_category = 0;
	_supported_unread_alert_category = 0;
	_enabled_new_alert_category = 0;
	_enabled_unread_alert_category = 0;
	for (int i = 0; i < ANS_ALERT_CATEGORY_COUNT; i++) {
		_alert_status[i].value = 0;
		_alert_status[i].fields.category = i;
	}

	assert(this->addCharacteristic(GattCharacteristic::UUID_SUPPORTED_NEW_ALERT_CATEGORY_CHAR,
								   &_supported_new_alert_category,
								   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
								   nullptr) &&
		   "add service 1 fail");

	assert(this->addCharacteristic(GattCharacteristic::UUID_SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR,
								   &_supported_unread_alert_category,
								   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
								   nullptr) &&
		   "add service 2 fail");

	assert(this->addCharacteristic(GattCharacteristic::UUID_UNREAD_ALERT_CHAR,
								   &_enabled_unread_alert_category,
								   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
								   nullptr) &&
		   "add service 3 fail");

	assert(this->addCharacteristic(GattCharacteristic::UUID_NEW_ALERT_CHAR,
								   &_enabled_new_alert_category,
								   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
								   nullptr) &&
		   "add service 4 fail");

	assert(this->addCharacteristic(GattCharacteristic::UUID_ALERT_NOTIFICATION_CONTROL_POINT_CHAR,
								   &_alert_status,
								   GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
								   nullptr) &&
		   "add service 5 fail");

	for (auto i : _characteristics) {
		i->setSecurityRequirements(ble::att_security_requirement_t::UNAUTHENTICATED,
								   ble::att_security_requirement_t::UNAUTHENTICATED,
								   ble::att_security_requirement_t::UNAUTHENTICATED);
	}
	assert(!(this->CGattService::createService(GattService::UUID_ALERT_NOTIFICATION_SERVICE)) &&
		   "service creation failed");
	_button.fall(mbed::callback(this, &CAlertNotificationService::buttonPressedHandler));
}

CAlertNotificationService::~CAlertNotificationService() {}

void CAlertNotificationService::onDataWrittenHandler(GattCharacteristic *characteristic,
													 const uint8_t *data,
													 uint16_t size) {}

void CAlertNotificationService::onConnected(void) {}

void CAlertNotificationService::onDisconnected(void) {}

void CAlertNotificationService::registerService(ble::BLE &ble) { CGattService::registerService(ble); }

bool CAlertNotificationService::addNewAlertToCategory(CAlertNotificationService::CategoryId category) {
	return false;
}

bool CAlertNotificationService::setSupportedUnreadAlertsCategory(uint16_t supportedUnreadAlerts) {
	return false;
}

bool CAlertNotificationService::addSupportedUnreadAlertsCategory(
	CAlertNotificationService::CategoryId category) {
	return false;
}

bool CAlertNotificationService::removeSupportedUnreadAlertsCategory(
	CAlertNotificationService::CategoryId category) {
	return false;
}

bool CAlertNotificationService::setSupportedNewAlertsCategory(uint16_t supportedUnreadAlerts) {
	return false;
}

bool CAlertNotificationService::addSupportedNewAlertsCategory(
	CAlertNotificationService::CategoryId category) {
	return false;
}

bool CAlertNotificationService::removeSupportedNewAlertsCategory(
	CAlertNotificationService::CategoryId category) {
	return false;
}

void CAlertNotificationService::clearAlertsOfCategory(CategoryId category) {}
