#include <BLE.h>
#include <mbed.h>

#include <iostream>

#include "alert_notification_service.h"
#include "gatt_server.h"
#include "immediate_alert_service.h"
#include "nrf52832_app_protect.h"
#include "secure_gap.h"

int main() {

	nrf52_disable_approtect();

	std::cout << "Starting GattServer application" << std::endl;
	
	// Create EventQueue object for the application
    EventQueue event_queue;
    BLE &ble = BLE::Instance();
	// Instantiate CSecureGap object with the required parameters
    CSecureGap csecurgap(ble, event_queue, "MTC-BLE", LED1);
	// Instantiate a CImmediateAlertService object and a CAlertNotificationService object
    CImmediateAlertService ias(LED2);
    CAlertNotificationService ans(BUTTON1);

	// instantiate CGattServer object
    CGattServer cgattserver;
	// Add service objects to CGattServer
    cgattserver.addService(ias);
    
    cgattserver.addService(ans);
    

    csecurgap.setOnBLEInitCompleteCallback(mbed::callback(&cgattserver, &CGattServer::startServer));

    csecurgap.setOnConnectedCallback(mbed::callback(&cgattserver, &CGattServer::onConnected));

    csecurgap.setOnDisconnectedCallback(mbed::callback(&cgattserver, &CGattServer::onDisconnected));

    csecurgap.run();

	return 0;
}