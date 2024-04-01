#pragma once

#include <BLE.h>

#include <list>
#include <memory>

#include "gatt_service.h"

class CGattServer : public GattServer::EventHandler, public mbed::NonCopyable<CGattServer> {
  private:
	std::list<CGattService *> _services; //!< The services of this server.
	bool _started;						 //!< Indicates that the server has started when it is true.

  public:
	CGattServer();
	~CGattServer();
	bool addService(CGattService &service);
	void startServer(ble::BLE &ble);

	void onConnected(void);

	void onDisconnected(void);
	virtual void onDataWritten(const GattWriteCallbackParams &e) override;
	virtual void onDataRead(const GattReadCallbackParams &e) overr
	ide;
	virtual void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override;
	virtual void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) override;
	virtual void onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params) override;
};