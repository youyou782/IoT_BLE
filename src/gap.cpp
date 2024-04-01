#include "gap.h"

#include <iostream>

#include "ble_utils.h"

void CGap::toggleLED() { _led = !_led; }

void CGap::updateAdvertisementData() { std::cout << "Advertisement data updated!" << std::endl; }
void CGap::onBLEInitCompleteHandler(BLE::InitializationCompleteCallbackContext *context) {
	std::cout << "BLE init completed" << std::endl;
	ble::own_address_type_t addrType;
	ble::address_t address;
	auto error = context->ble.gap().getAddress(addrType, address);
	std::cout << bleErrorToString(error, "GAP::getAddress()") << std::endl;
	if (error == BLE_ERROR_NONE) {
	}
}

void CGap::scheduleBLEEventsToProcess(BLE::OnEventsToProcessCallbackContext *context) {
	_event_queue.call(callback(&(context->ble), &BLE::processEvents));
}

CGap::CGap(ble::BLE &ble,
		   EventQueue &event_queue,
		   const std::string &device_name,
		   PinName led_pin,
		   const mbed::Callback<void(ble::BLE &)> &on_ble_init_callback,
		   const mbed::Callback<void(void)> &on_connected,
		   const mbed::Callback<void(void)> &on_disconnected)
	: _ble(ble), _event_queue(event_queue), _device_name(device_name), _led(led_pin),
	  _adv_data_builder(_adv_data_buffer),
	  // _on_ble_init_callback(on_ble_init_callback),
	  _led_event_id(0) {
	//   setOnBLEInitCompleteCallback(on_ble_init_callback);
}

void CGap::run() {
	_ble.onEventsToProcess(makeFunctionPointer(this, &CGap::scheduleBLEEventsToProcess));
	_ble.gap().setEventHandler(this);
	_ble.init(this, &CGap::onBLEInitCompleteHandler);
	std::cout << "Starting BLE Application " << std::endl;
	_event_queue.dispatch_forever();
}

void CGap::startAdvertising() {
	ble::AdvertisingParameters advParameters(ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
											 ble::adv_interval_t(ble::millisecond_t(100)));
	_adv_data_builder.setName(_device_name.c_str());
	ble_error_t error = _ble.gap().setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, advParameters);

	if (error != BLE_ERROR_NONE) {
		std::cout << ble::BLE::errorToString(error) << std::endl;
		return;
	}
	error = _ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE,
											 _adv_data_builder.getAdvertisingData());

	if (error != BLE_ERROR_NONE) {
		std::cout << ble::BLE::errorToString(error) << std::endl;
		return;
	}

	// Start advertising
	error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
	if (error != BLE_ERROR_NONE) {
		std::cout << ble::BLE::errorToString(error) << std::endl;
		return;
	}

	// Start blinking the Application LED
	if (!_led_event_id) {
		_led_event_id = _event_queue.call_every(500ms, mbed::callback(this, &CGap::toggleLED));
	}
	std::cout << "Device is advertising" << std::endl;
}

void CGap::onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
	// if on_connected callback is not nullptr, call it
	// keep the LED on
	if (_on_connected) {
		_on_connected();
	}

	std::cout << "Device is connected" << std::endl;
	if (_led_event_id) {
		_event_queue.cancel(_led_event_id);
		_led = 0;
		_led_event_id = 0;
	}
}
void CGap::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) {
	// if on_disconnected is not nullptr, call it
	// start advertising
	if (_on_disconnected) {
		_on_disconnected();
	}
	std::cout << "Device is disconnected" << std::endl;
	startAdvertising();
}

void CGap::setOnBLEInitCompleteCallback(const mbed::Callback<void(ble::BLE &)> &callback) {
	_on_ble_init_callback = callback;
}

void CGap::setOnConnectedCallback(const mbed::Callback<void(void)> &callback) { _on_connected = callback; }

void CGap::setOnDisconnectedCallback(const mbed::Callback<void(void)> &callback) {
	_on_disconnected = callback;
}
