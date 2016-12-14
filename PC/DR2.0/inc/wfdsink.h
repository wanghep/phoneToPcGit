#ifndef _WIFIDISPLAY_SINK_API_H_
#define _WIFIDISPLAY_SINK_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <winsock2.h>
#include <windot11.h>

typedef struct _WFD_DISPLAY_SINK_OBJECT_HEADER {
    UCHAR       Type;
    UCHAR       Revision;
    USHORT      Length;
} WFD_DISPLAY_SINK_OBJECT_HEADER, *PWFD_DISPLAY_SINK_OBJECT_HEADER;

// Values to be used in header for WFD_DISPLAY_SINK_NOTIFICATION
#define WFD_DISPLAY_SINK_NOTIFICATION_OBJECT_TYPE_DEFAULT                    1
#define WFD_DISPLAY_SINK_NOTIFICATION_OBJECT_REVISION_VERSION_1              1

// Values to be used in header for WFD_DISPLAY_SINK_NOTIFICATION_RESULT
#define WFD_DISPLAY_SINK_NOTIFICATION_RESULT_OBJECT_TYPE_DEFAULT             1
#define WFD_DISPLAY_SINK_NOTIFICATION_RESULT_OBJECT_REVISION_VERSION_1       1

#define WFD_SINK_MAX_DEVICE_NAME_LENGTH                                      32
#define WFD_SINK_WPS_INFO_MAX_PASSKEY_LENGTH                                 8

typedef enum _WFD_DISPLAY_SINK_NOTIFICATION_TYPE {
    ProvisioningRequestNotification,
    ReconnectRequestNotification,
    ConnectCompleteNotification,
    ConnectFailedNotification,
    DisconnectedNotification
} WFD_DISPLAY_SINK_NOTIFICATION_TYPE;

typedef struct _WFD_DISPLAY_SINK_NOTIFICATION {
    WFD_DISPLAY_SINK_OBJECT_HEADER Header;
    WFD_DISPLAY_SINK_NOTIFICATION_TYPE type;
    WCHAR strRemoteDeviceName[WFD_SINK_MAX_DEVICE_NAME_LENGTH + 1];
    DOT11_MAC_ADDRESS RemoteDeviceAddress;

    union
    {
        // Information about provisiong for the incoming connection request
        struct _ProvisioningRequestInfo
        {
            HANDLE hSessionHandle;                         // Handle to the session
            DOT11_WPS_CONFIG_METHOD PossibleConfigMethods; // Bitmask of possible configuration methods
        } ProvisioningRequestInfo;

        // Information about the incoming reconnect request
	    struct _ReconnectRequestInfo
        {
            HANDLE hSessionHandle;             // Handle to the session
            DOT11_WFD_GROUP_ID GroupID;        // Group ID of the group being reinvoked for the reconnect
        } ReconnectRequestInfo;

        // Information about connection setup successfully
	    struct _ConnectCompleteInfo
        {
            HANDLE hSessionHandle;             // Handle to the session
            GUID guidSessionInterface;         // GUID of the Wi Fi Direct network interface
            DOT11_WFD_GROUP_ID GroupID;        // Group ID of the group use for the connection.
            PWSTR strProfile;                  // If a new profile was generated, this pointer is set to a null terminated string. When the callback returns
                                               // this pointer will be freed and hence can not be accessed.
            SOCKADDR_STORAGE LocalAddress;     // Local IP address
            SOCKADDR_STORAGE RemoteAddress;    // Remote IP address
            USHORT uRTSPPort;                  // RTSP port to be used to setup RTSP session (in host byte order)
        } ConnectCompleteInfo;

        // Information about failed connection attempt
	    struct _ConnectFailedInfo
        {
            HANDLE hSessionHandle;             // Handle to the session
            DWORD dwError;                     // Set to Win32 error code indicating reason for failure
            DWORD dwReasonCode;                // Set to a WLAN specific reason code when a detailed reason for failure is known
        } ConnectFailedInfo;
        
        struct _DisconnectedInfo
        {
            HANDLE hSessionHandle;             // Handle to the session just disconnected
        } DisconnectedInfo;
    };
} WFD_DISPLAY_SINK_NOTIFICATION, *PWFD_DISPLAY_SINK_NOTIFICATION;

typedef struct _WFD_DISPLAY_SINK_NOTIFICATION_RESULT {
    WFD_DISPLAY_SINK_OBJECT_HEADER Header;
    WFD_DISPLAY_SINK_NOTIFICATION_TYPE type;
    union
    {
        struct _ProvisioningData
        {
            DOT11_WPS_CONFIG_METHOD ConfigMethod;                 // Configuration method to be used for the connection              
            UINT32 uPassKeyLength;                                // Length of the pass key
            UCHAR Passkey[WFD_SINK_WPS_INFO_MAX_PASSKEY_LENGTH];  // Pass key string (not null terminated)
        } ProvisioningData;

        struct _ReconnectData
        {
            PWSTR strProfile;                                     // The profile string to be used for the connection, allocated using WlanAllocateMemory.
                                                                  // The receiver of the notification is assumed to release ownership of the pointer.
                                                                  // The string bufer will be freed by the API using WlanFreeMemory.     
        } ReconnectData;
    };

} WFD_DISPLAY_SINK_NOTIFICATION_RESULT, *PWFD_DISPLAY_SINK_NOTIFICATION_RESULT;

typedef DWORD (WINAPI *WFD_DISPLAY_SINK_NOTIFICATION_CALLBACK) (
    _In_opt_ PVOID Context,
    _In_ const PWFD_DISPLAY_SINK_NOTIFICATION pNotification,
    _Inout_opt_ PWFD_DISPLAY_SINK_NOTIFICATION_RESULT pNotificationResult
    );

// Returns ERROR_NOT_SUPPORTED if DISPLAY sink is not supported on 
// this h/w
DWORD 
WINAPI
WFDDisplaySinkStart(
    _In_ USHORT uDeviceCategory,
    _In_ USHORT uSubCategory,
    _In_ BOOL fLCPSupported,
    _In_opt_ PVOID pCallbackContext,
    _In_ WFD_DISPLAY_SINK_NOTIFICATION_CALLBACK pfnCallback
    );

DWORD
WINAPI
WFDDisplaySinkStop();

DWORD
WINAPI
WFDDisplaySinkCloseSession(
    _In_ HANDLE hSessionHandle
    );

DWORD
WINAPI
WFDDisplaySinkSetPersistedGroupIDList(
    _In_ UINT32 cGroupIDList,
    _In_opt_count_(cGroupIDList) DOT11_WFD_GROUP_ID* pGroupIDList 
    );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _WIFIDISPLAY_SINK_H_