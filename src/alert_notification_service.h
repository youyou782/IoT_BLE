#pragma once
#include <BLE.h>
#include <mbed.h>

#include <memory>
#include <vector>

#include "gatt_service.h"

class CAlertNotificationService : public CGattService {
  public:
#define ANS_ALERT_CATEGORY_COUNT (10)
	/**
	 * @brief Alert notification control point commands, as defined in the Alert Notification Specification.
	 * UUID: 0x2A44.
	 */
	enum CategoryId {
		ANS_TYPE_SIMPLE_ALERT = 0,			 /**< General text alert or non-text alert.*/
		ANS_TYPE_EMAIL = 1,					 /**< Email message arrives.*/
		ANS_TYPE_NEWS = 2,					 /**< News feeds such as RSS, Atom.*/
		ANS_TYPE_NOTIFICATION_CALL = 3,		 /**< Incoming call.*/
		ANS_TYPE_MISSED_CALL = 4,			 /**< Missed call.*/
		ANS_TYPE_SMS_MMS = 5,				 /**< SMS or MMS message arrives.*/
		ANS_TYPE_VOICE_MAIL = 6,			 /**< Voice mail.*/
		ANS_TYPE_SCHEDULE = 7,				 /**< Alert that occurs on calendar, planner.*/
		ANS_TYPE_HIGH_PRIORITIZED_ALERT = 8, /**< Alert to be handled as high priority.*/
		ANS_TYPE_INSTANT_MESSAGE = 9,		 /**< Alert for incoming instant messages.*/
		ANS_TYPE_ALL_ALERTS = 0xFF			 /**< Identifies all alerts. */
	};
	enum CategoryMaskId {
		ANS_TYPE_MASK_SIMPLE_ALERT = (1 << 0),			 /**< General text alert or non-text alert.*/
		ANS_TYPE_MASK_EMAIL = (1 << 1),					 /**< Email message arrives.*/
		ANS_TYPE_MASK_NEWS = (1 << 2),					 /**< News feeds such as RSS, Atom.*/
		ANS_TYPE_MASK_NOTIFICATION_CALL = (1 << 3),		 /**< Incoming call.*/
		ANS_TYPE_MASK_MISSED_CALL = (1 << 4),			 /**< Missed call.*/
		ANS_TYPE_MASK_SMS_MMS = (1 << 5),				 /**< SMS or MMS message arrives.*/
		ANS_TYPE_MASK_VOICE_MAIL = (1 << 6),			 /**< Voice mail.*/
		ANS_TYPE_MASK_SCHEDULE = (1 << 7),				 /**< Alert that occurs on calendar, planner.*/
		ANS_TYPE_MASK_HIGH_PRIORITIZED_ALERT = (1 << 8), /**< Alert to be handled as high priority.*/
		ANS_TYPE_MASK_INSTANT_MESSAGE = (1 << 9),		 /**< Alert for incoming instant messages.*/
		ANS_TYPE_MASK_ALL_ALERTS = 0x03FF				 /**< Identifies all alerts. */
	};
	/**
	 * @brief Alert notification control point commands, as defined in the Alert Notification Specification.
	 * UUID: 0x2A44
	 */
	enum CommandId {
		ANS_ENABLE_NEW_INCOMING_ALERT_NOTIFICATION = 0,		/**< Enable New Incoming Alert Notification.*/
		ANS_ENABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION = 1, /**< Enable Unread Category Status Notification.*/
		ANS_DISABLE_NEW_INCOMING_ALERT_NOTIFICATION = 2,	/**< Disable New Incoming Alert Notification.*/
		ANS_DISABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION =
			3,											   /**< Disable Unread Category Status Notification.*/
		ANS_NOTIFY_NEW_INCOMING_ALERT_IMMEDIATELY = 4,	   /**< Notify New Incoming Alert immediately.*/
		ANS_NOTIFY_UNREAD_CATEGORY_STATUS_IMMEDIATELY = 5, /**< Notify Unread Category Status immediately.*/
	};
	/**
	 * @brief The ANS control point commands
	 *
	 */
	union control_point_t {
		uint16_t value;
		struct control_point_fields {
			uint8_t command;
			uint8_t category;
		} fields;
	};
	/**
	 * @brief Alert status data structure
	 *
	 */
	union alert_status_t {
		uint16_t value;
		struct alert_status_fields {
			uint8_t category; //!< The alert category
			uint8_t count;	  //!< The number of unread alerts
		} fields;
	};

  private:
	uint32_t _button_press_count; //!< The number of button press counts since the system startup
	InterruptIn _button;		  //!< The system Button that will be used for capturing alerts

	int _supported_new_alert_category_index;	//!< The index of the supported new alert category in
												//!< _characteristics container
	int _supported_unread_alert_category_index; //!< The index of the supported unread alert category
												//!< characteristic in _characteristics container
	int _unread_alert_status_index;				//!< The index of the unread alert status characteristic in
												//!< _characteristics container
	int _new_alert_index; //!< The index of the new alert characteristic in _characteristics
						  //!< container
	int _alert_notification_control_point_index; //!< The index of the alert notification control point
												 //!< characteristic in _characteristics container
												 // NOTE: The values of the characteristics

	uint16_t _supported_new_alert_category;	   //!< supported new alerts configuration
	uint16_t _supported_unread_alert_category; //!< supported unread alert configuration
	uint16_t _enabled_new_alert_category;	   //!< Enabled new alert alerts by the current client
	uint16_t _enabled_unread_alert_category;   //!< Enabled unread alerts by the current client

	alert_status_t _alert_status[ANS_ALERT_CATEGORY_COUNT]; //!< Alert status for each supported alert

  protected:
	void buttonPressedHandler();

  public:
	CAlertNotificationService() = delete;
	CAlertNotificationService(const CAlertNotificationService &) = delete;
	CAlertNotificationService &operator=(const CAlertNotificationService &) = delete;

	CAlertNotificationService(PinName button_pin);
	~CAlertNotificationService();

	ble_error_t createService(const char *uuid) = delete;

	virtual void
		onDataWrittenHandler(GattCharacteristic *characteristic, const uint8_t *data, uint16_t size) final;

	virtual void onConnected(void) final;
	virtual void onDisconnected(void) final;

	virtual void registerService(ble::BLE &ble) final;

	bool addNewAlertToCategory(CAlertNotificationService::CategoryId category);

	bool setSupportedUnreadAlertsCategory(uint16_t supportedUnreadAlerts);
	bool addSupportedUnreadAlertsCategory(CAlertNotificationService::CategoryId category);

	bool removeSupportedUnreadAlertsCategory(CAlertNotificationService::CategoryId category);

	bool setSupportedNewAlertsCategory(uint16_t supportedUnreadAlerts);
	bool addSupportedNewAlertsCategory(CAlertNotificationService::CategoryId category);

	bool removeSupportedNewAlertsCategory(CAlertNotificationService::CategoryId category);

	void clearAlertsOfCategory(CategoryId category);
};