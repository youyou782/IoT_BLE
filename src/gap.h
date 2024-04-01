#pragma once
#include <BLE.h>
#include <mbed.h>

#include <string>
class CGap : private mbed::NonCopyable<CGap>, public ble::Gap::EventHandler {
   protected:
	ble::BLE &_ble;			   //!< A reference to the BLE system's BLE object instance
	EventQueue &_event_queue;  //!< A reference to the application's EventQueue
	std::string _device_name;  //!< The name of the device.

	DigitalOut _led;  //!< The system LED that will be used for indicating advertisement and connection status
	int _led_event_id;	//!< The Event identifier (returned by EventQueue::call_every function)

	ble::AdvertisingDataBuilder _adv_data_builder;	//!< Advertisement data builder that will be used for
													//!< creating, storing and manipulating advertisement data
	uint8_t _adv_data_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];	 //!< The buffer that is used by the
																 //!< advertisement data builder
	mbed::Callback<void(ble::BLE &)> _on_ble_init_callback;

	mbed::Callback<void(void)> _on_connected;

	mbed::Callback<void(void)> _on_disconnected;
   protected:
	void toggleLED();
	void updateAdvertisementData();
	virtual void onBLEInitCompleteHandler(BLE::InitializationCompleteCallbackContext *context);
	void scheduleBLEEventsToProcess(BLE::OnEventsToProcessCallbackContext *context);
	virtual void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;
	virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override;

   public:
	CGap() = delete;
	CGap(const CGap &) = delete;
	CGap(ble::BLE &ble,
		 EventQueue &event_queue,
		 const std::string &device_name,
		 PinName led_pin,
		 const mbed::Callback<void(ble::BLE &)> &on_ble_init_callback = nullptr,
		 const mbed::Callback<void(void)> &on_connected = nullptr,
		 const mbed::Callback<void(void)> &on_disconnected = nullptr);
	void run();
	void startAdvertising();
	/**
	 * @brief Set the OnBLEInitComplete Callback object
	 *
	 * @param callback The callback function to be set
	 */
	void setOnBLEInitCompleteCallback(const mbed::Callback<void(ble::BLE &)> &callback);

	/**
	 * @brief Set the On Connected Callback function
	 *
	 * @param callback The callback function
	 */
	void setOnConnectedCallback(const mbed::Callback<void(void)> &callback);

	/**
	 * @brief Set the On Disconnected Callback function
	 *
	 * @param callback The callback function
	 */
	void setOnDisconnectedCallback(const mbed::Callback<void(void)> &callback);
};