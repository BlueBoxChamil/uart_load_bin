#ifndef _GATT_API_H
#define _GATT_API_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "gatt_defines.h"
#include "gatt_sig_uuid.h"
#include "ble_api_error.h"

/* Exported types ------------------------------------------------------------*/
enum gatt_msg_evt {
    GATTS_MSG_READ_REQ,         //!< GATT service received read request
    GATTS_MSG_WRITE_REQ,        //!< GATT service received write request
    GATTS_MSG_ATT_INFO_REQ,     //!< GATT service received get att information request
    GATTC_MSG_NTF_REQ,          //!< GATT client notification received
    GATTC_MSG_IND_REQ,          //!< GATT client indication received
    GATTC_MSG_READ_IND,         //!< GATT client read response received
    GATTC_MSG_CMP_EVT,          //!< GATT cmd message complete event
    GATTC_MSG_LINK_CREATE,      //!< A Link is established
    GATTC_MSG_LINK_LOST,        //!< A Link is lost
    GATTC_MSG_SVC_REPORT,       //!< GATT client received report of service discovery
    GATTC_MSG_CHAR_REPORT,      //!< GATT client received report of characteristic discovery
    GATTC_MSG_CHAR_DESC_REPORT, //!< GATT client received report of characteristic describtion discovery
};

enum gatt_event_type {
    ENABLE_TYPE_NTF,                 //!< notification type
    ENABLE_TYPE_IND,                 //!< indication type
};

enum gatt_svc_change_type
{
    SVC_CHANGE_UUID,   //!< change uuid 
    SVC_CHANGE_PERM,   //!< change property 
};

#define UUID_SIZE_2         2                   //!< 16-bits UUID, usually SIG assigned UUID.
#define UUID_SIZE_4         4                   //!< 32-bits UUID, usually assigned by users. 
#define UUID_SIZE_16        16                  //!< 128-bits UUID, usually assigned by users. 

/** @defgroup GATT_PROP_BITMAPS_DEFINES GATT Attribute Access Permissions Bit Fields
 * @{
 */
#define GATT_PROP_BROADCAST             (1<<0)  //!< Attribute is able to broadcast
#define GATT_PROP_READ                  (1<<1)  //!< Attribute is Readable
#define GATT_PROP_WRITE_CMD             (1<<2)  //!< Attribute supports write with no response
#define GATT_PROP_WRITE_REQ             (1<<3)  //!< Attribute supports write request
#define GATT_PROP_NOTI                  (1<<4)  //!< Attribute is able to send notification
#define GATT_PROP_INDI                  (1<<5)  //!< Attribute is able to send indication
#define GATT_PROP_SIG_WRTIE             (1<<6)  //!< Attribute supports authenticated signed write
#define GATT_PROP_EXTEND_PROP           (1<<7)  //!< Attribute supports extended properities
#define GATT_PROP_READ_UNAUTH           (1<<8)  //!< Attribute read requires Unauthentication(Just work pairring method)
#define GATT_PROP_READ_AUTHEN           (2<<8)  //!< Attribute read requires Authentication(Legacy mode. Pass key or OOB pairring method)
#define GATT_PROP_READ_SECURE_CONN      (3<<8)  //!< Attribute read requires Secure Connection(Secure connect mode. Pass key\OOB\Numbric Comparision pairing method)
#define GATT_PROP_WRITE_UNAUTH          (1<<10) //!< Attribute write requires Unauthentication
#define GATT_PROP_WRITE_AUTHEN          (2<<10) //!< Attribute write requires Authentication
#define GATT_PROP_WRITE_SECURE_CONN     (3<<10) //!< Attribute write requires Secure Connection
#define GATT_PROP_NI_UNAUTH             (1<<12) //!< Attribute notification or indication requires Unauthentication
#define GATT_PROP_NI_AUTHEN             (2<<12) //!< Attribute notification or indication requires Authentication
#define GATT_PROP_NI_SECURE_CONN        (3<<12) //!< Attribute notification or indication requires Secure Connection
#define GATT_PROP_READ_AUTHOR           (1<<14) //!< Attribute read requires Authorization
#define GATT_PROP_WRITE_AUTHOR          (2<<14) //!< Attribute write requires Authorization
/** @} End GATT_PERMIT_BITMAPS_DEFINES */

/** @defgroup GATT_OPERATION_NAME_DEFINES GATT operation name define, used with GATTC_MSG_CMP_EVT
 * @{
 */
#define GATT_OP_NOTIFY                  0x01    //!< GATT notification operation
#define GATT_OP_INDICA                  0x02    //!< GATT indication operation
#define GATT_OP_PEER_SVC_REGISTERED     0x03    //!< Used with GATTC_CMP_EVT, GATT peer device service registered
#define GATT_OP_WRITE_REQ               0x05    //!< GATT wirte request operation
#define GATT_OP_WRITE_CMD               0x06    //!< GATT wirte command operation, write without response
#define GATT_OP_READ                    0x07    //!< GATT read operation
#define GATT_OP_PEER_SVC_DISC_END       0x08    //!< Used with GATTC_CMP_EVT, GATT peer device service discovery is ended  
#define GATT_OP_PEER_CHAR_DISC_END      0x09    //!< Used with GATTC_CMP_EVT, GATT peer device characteristic discovery is ended 
#define GATT_OP_PEER_DESC_DISC_END      0x0A    //!< Used with GATTC_CMP_EVT, GATT peer device characteristic describtion discovery is ended
#define GATT_OP_CLI_EVENT_REGISTERED    0x0B    //!< Used with GATTC_CMP_EVT, gatt_client_event_register() completed
#define GATT_OP_CLI_EVENT_UNREGISTERED  0x0C    //!< Used with GATTC_CMP_EVT, gatt_client_event_unregister() completed
/** @} End GATT_OPERATION_NAME_DEFINES */

struct gatt_msg {
    enum gatt_msg_evt msg_evt;  //!< The event that message come with
    uint8_t conn_idx;           //!< Connection index
    uint8_t svc_id;             //!< service id of this message
    uint16_t att_idx;           //!< Attribute index of in the service table or client uuid table
    uint16_t handle;            //!< Attribute handle number in peer service
    union {
        struct {
            uint8_t operation;  //!< GATT request type, see @GATT_OPERATION_NAME_DEFINES
            uint8_t status;     //!< Status of the request
            uint16_t arg_len;   //!< Parameter length
            void * arg;         //!< Parameter pointer
        } gatt_op_cmp;          //!< Value of GATTC_MSG_CMP_EVT

        struct {
            uint16_t msg_len;   //!< GATT message length
            uint16_t offset;    //!< GATT message data offset
            uint8_t *p_msg_data;//!< GATT message data pointer
        } gatt_data;
    } param;
};

typedef uint16_t (*gatt_msg_handler_t)(struct gatt_msg *p_msg);

typedef struct gatt_uuid {
    uint8_t size;           //!< Length of UUID (2 or 16 bytes UUIDs: UUID_SIZE_2 or UUID_SIZE_16). 
    uint8_t p_uuid[16];     //!< Pointer to uuid, could be 2 or 16 bytes array. 
}gatt_uuid_t;

typedef struct gatt_attribute {
    struct gatt_uuid uuid;  //!< Attribute UUID
    uint16_t prop;          //!< Attribute properties, see @GATT_PROP_BITMAPS_DEFINES
    uint16_t max_size;      //!< Attribute data maximum size
    uint8_t *p_data;        //!< Attribute data pointer
}gatt_attribute_t;

typedef struct gatt_service {
    const struct gatt_attribute *p_att_tb;  //!< Service's attributes table to add to system attribute database.
    uint8_t att_nb;                         //!< Service's attributes number.
    gatt_msg_handler_t gatt_msg_handler;    //!< Read request callback function.
}gatt_service_t;

typedef struct gatt_client {
    const struct gatt_uuid *p_att_tb;     //!< Characteristics UUID.
    uint8_t att_nb;                       //!< Characteristic number. 
    uint8_t att_tb_initialized;           //!< 
    gatt_msg_handler_t gatt_msg_handler;  //!< Read request callback function. 
}gatt_client_t;

typedef struct gatt_service_change {
    uint8_t svc_id;                     //!< Server index, this is return value after calling gatt_add_server.
    uint8_t att_idx;                    //!< Attribute index in service attribute table registed when calling gatt_add_service.
    bool send_svc_chg_ind;              //!< true: a service changed indication will be automatically sent to all clients who have written the CCC of the local service changed characteristics(uuid: 0x2A05)
                                        //!< false: not send
    enum gatt_svc_change_type type;     //!< ref @enum gatt_svc_change_type
    union {
        struct gatt_uuid uuid;          //!< for type SVC_CHANGE_UUID
        uint16_t new_prop;              //!< for type SVC_CHANGE_PERM
    } p;
}gatt_service_change_t;

typedef struct gatt_multi_att {
    uint8_t  length;        //The length of each attribute to read, cannot to be 0. The total length should be less than (MTU - 1).
    uint16_t handle;        //Peer attribute handle
}gatt_multi_att_t;

typedef struct gatt_client_write_param {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint8_t att_idx;        //!< Attribute index in client uuid table registed when calling gatt_add_client.
    uint8_t *p_data;        //!< Data pointer to be written
    uint16_t data_len;      //!< Data length to be written
}gatt_client_write_param_t;

typedef struct gatt_client_write_param_handle {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint16_t handle;        //!< peer device characteristic handle
    uint8_t *p_data;        //!< Data pointer to be written
    uint16_t data_len;      //!< Data length to be written
}gatt_client_write_param_handle_t;

typedef struct gatt_client_read_param {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint8_t att_idx;        //!< Attribute index in client uuid table registed when calling gatt_add_client.
}gatt_client_read_param_t;

typedef struct gatt_client_read_param_handle {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint16_t handle;        //!< peer device attribute handle
    uint16_t offset;        //!< Value offset
}gatt_client_read_param_handle_t;

typedef struct gatt_client_read_param_uuid {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint16_t start_hdl;     //!< Start handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
    uint16_t end_hdl;       //!< End handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
    struct gatt_uuid uuid;  //!< Attribute UUID
}gatt_client_read_param_uuid_t;

typedef struct gatt_client_read_multiple_param {
    uint8_t conidx;           //!< Connection index
    uint8_t client_id;        //!< Client index, this is return value after calling gatt_add_client.
    uint8_t att_nb;           //!< Number of attribute to read
    gatt_multi_att_t *p_atts; //!< param of multiple read request
}gatt_client_read_multiple_param_t;

typedef struct gatt_client_enable_event {
    uint8_t conidx;         //!< Connection index
    uint8_t client_id;      //!< Client index, this is return value after calling gatt_add_client.
    uint8_t att_idx;        //!< Attribute index in client uuid table registed when calling gatt_add_client.
}gatt_client_enable_event_t;

typedef struct gatt_send_event {
    uint8_t conidx;         //!< Connection index
    uint8_t svc_id;         //!< Server index, this is return value after calling gatt_add_server.
    uint8_t att_idx;        //!< Attribute index in service attribute table registed when calling gatt_add_service.
    uint8_t *p_data;        //!< Data to be sent as notification.
    uint16_t data_len;      //!< Data length
}gatt_send_event_t;

typedef struct gatt_send_event_handle {
    uint8_t conidx;         //!< Connection index
    uint8_t svc_id;         //!< Server index, this is return value after calling gatt_add_server.
    uint16_t handle;        //!< Attribute handle
    uint8_t *p_data;        //!< Data to be sent as notification.
    uint16_t data_len;      //!< Data length
}gatt_send_event_handle_t;

typedef struct gatt_send_svc_changed_param {
    uint16_t start_hdl;     //!< Service changed Start handle, shall be set in the range of 0x0001 - 0xffff
    uint16_t end_hdl;       //!< Service changed End handle, shall be set in the range of 0x0001 - 0xffff
    bool    all;            //!< true: Send Service changed indication to multiple peer devices
                            //!< false: Send Service changed indication to specified divice
    uint8_t conidx;         //!< Valid when all is false. Connection index.
}gatt_send_svc_changed_param_t;

typedef struct gatt_set_authorization {
    uint8_t conidx;         //!< Connection index
    uint8_t svc_id;         //!< Server index, this is return value after calling gatt_add_server.
    bool    set;            //!< True: enable authorization, false: disable authorization;
    bool    all;            //!< true: Operation applies to all attributes
                            //!< false: Operation applies to a specified attribute
    uint8_t att_idx;        //!< Valid when all is false. Attribute index in service attribute table registed when calling gatt_add_service.
}gatt_set_authorization_t;

typedef struct gatt_svc_report {
    uint16_t start_hdl;     //!< Discovery service start handle
    uint16_t end_hdl;       //!< Discovery service end handle
    struct gatt_uuid uuid;  //!< Service UUID
}gatt_svc_report_t;

typedef struct gatt_char_report {
    uint16_t char_hdl;      //!< characteristic declaration handle
    uint16_t val_hdl;       //!< characteristic value handle
    uint8_t prop;           //!< characteristic properties, see ref@GATT_PROP_BITMAPS_DEFINES bit0 - bit7
    struct gatt_uuid uuid;  //!< characteristic uuid
}gatt_char_report_t;

typedef struct gatt_desc_report {
    uint16_t desc_hdl;       //!< describtion handle
    struct gatt_uuid uuid;   //!< describtion uuid
}gatt_desc_report_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*
 * @fn          gatt_add_service
 *
 * @brief       Addding a services & characteristics into gatt database.
 *
 * @param[in]   p_service : service data to be added.
 *
 * @return      Assigned svc_id.
 */
uint8_t gatt_add_service(struct gatt_service *p_service);

/*
 * @fn          gatt_delete_user_service
 *
 * @brief       Delete a user-defined services into gatt database.
 *              Should not be called when there is a connected device present.
 *
 * @param[in]   service_id : service id, this is return value after calling gatt_add_service.
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gatt_delete_user_service(uint8_t service_id);

/*
 * @fn          gatt_delete_gap_gatt_svc
 *
 * @brief       Delete gap and gatt services into gatt database, should be called before adding user service. Can only be called once.
 *
 * @param[in]   None.
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gatt_delete_gap_gatt_svc(void);

/*
 * @fn          gatt_get_local_svc_hdl
 *
 * @brief       Get the start and end handles of the specified service.
 * 
 * @param[in]   service_id : service id, this is return value after calling gatt_add_service.
 * @param[in]   start_hdl : Pointer to the value of the specified service start handle.
 * @param[in]   end_hdl : Pointer to the value of the specified service end handle.
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gatt_get_local_svc_hdl(uint8_t service_id, uint16_t *start_hdl, uint16_t *end_hdl);

/*********************************************************************
 * @fn          gatt_add_client
 *
 * @brief       Addding a GATT client in the system.
 *
 * @param[in]   p_client : client information.
 *
 * @return      Assigned client_id.
 *
 * @par Sample
 * @code
 * example1 :
 *   const struct gatt_uuid client_att_tb[] =
 *   {
 *       [0]  =
 *       { UUID_SIZE_16, SPSC_UUID128_ARR_TX},
 *       [1]  =
 *       { UUID_SIZE_16, SPSC_UUID128_ARR_RX},
 *   };
 *   func()
 *   {
 *       struct gatt_client client;
 *       client.p_att_tb = client_att_tb;
 *       client.att_nb = 2;
 *       client.gatt_msg_handler = client_msg_handler;
 *   }
 * example2 :   //in this case, there will no be uuids being registed.
 *   func()
 *   {
 *       struct gatt_client client;
 *       client.p_att_tb = NULL;
 *       client.att_nb = 0;
 *       client.gatt_msg_handler = client_msg_handler;
 *   }
 * @endcode
 */
uint8_t gatt_add_client(struct gatt_client *p_client);

/*
 * @fn          gatt_discovery_all_peer_svc
 *
 * @brief       Discover all peer device services.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_discovery_all_peer_svc(uint8_t client_id, uint8_t conidx);

/*
 * @fn          gatt_discovery_peer_svc
 *
 * @brief       Discover assigned peer device service.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   uuid : assigned service
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_discovery_peer_svc(uint8_t client_id, uint8_t conidx, struct gatt_uuid *uuid);

/*
 * @fn          gatt_discovery_peer_char
 *
 * @brief       Discover peer device characteristic when only the peer specified service handle range is known.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   start_hdl : start handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   end_hdl : end handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_discovery_peer_char(uint8_t client_id, uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*
 * @fn          gatt_discovery_peer_char_by_uuid
 *
 * @brief       Discover peer device characteristic by UUID when only the service handle range is known and the peer specified characteristic UUID is known.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   start_hdl : start handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   end_hdl : end handle of the peer specified service, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   uuid : uuid of the specified characteristic
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_discovery_peer_char_by_uuid(uint8_t client_id, uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl, struct gatt_uuid *uuid);

/*
 * @fn          gatt_discovery_peer_desc
 *
 * @brief       Discover peer device describtion when only the peer specified characteristic handle range is known.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   start_hdl : start handle of the peer specified characteristic value handle + 1, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   end_hdl : end handle of the specified characteristic, shall be set in the range of 0x0001 - 0xffff
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_discovery_peer_desc(uint8_t client_id, uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*
 * @fn          gatt_client_write_req
 *
 * @brief       request write operation with ATT_WRITE_REQUEST
 *
 * @param[in]   write_att : write operation parameter.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_write_req(struct gatt_client_write_param *write_att);

/*
 * @fn          gatt_client_write_cmd
 *
 * @brief       request write operation with ATT_WRITE_COMMAND
 *
 * @param[in]   write_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_write_cmd(struct gatt_client_write_param *write_att);

/*
 * @fn          gatt_cilent_write_req_with_handle
 *
 * @brief       request write operation with ATT_WRITE_REQUEST by handle
 *
 * @param[in]   write_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_cilent_write_req_with_handle(struct gatt_client_write_param_handle *write_att);

/*
 * @fn          gatt_cilent_write_cmd_with_handle
 *
 * @brief       request write operation with ATT_WRITE_COMMAND by handle
 *
 * @param[in]   write_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_cilent_write_cmd_with_handle(struct gatt_client_write_param_handle *write_att);

/*
 * @fn          gatt_client_read
 *
 * @brief       Read request.
 *
 * @param[in]   read_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_read(struct gatt_client_read_param *read_att);

/*
 * @fn          gatt_client_read_with_handle
 *
 * @brief       Read request by handle.
 *
 * @param[in]   read_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_read_with_handle(struct gatt_client_read_param_handle *read_att);

/*
 * @fn          gatt_client_read_with_uuid
 *
 * @brief       Read request by uuid.
 *
 * @param[in]   read_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_read_with_uuid(struct gatt_client_read_param_uuid *read_att);

/*
 * @fn          gatt_client_read_multiple_with_handle
 *
 * @brief       Read multiple request by handle. Only attribute values that have a known fixed size can be read.
 *
 * @param[in]   read_att : client information.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_read_multiple_with_handle(struct gatt_client_read_multiple_param *read_att);

/*
 * @fn          gatt_client_enable_ntf_ind
 *
 * @brief       enable notification or indication.
 *
 * @param[in]   event_enable : client information.
 *              type : event type.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_enable_ntf_ind(struct gatt_client_enable_event *event_enable, enum gatt_event_type type);

/*
 * @fn          gatt_notification
 *
 * @brief       Sending notification.
 *
 * @param[in]   ntf_att : in which service and which attribute the notification will be sent
 *                        This parameter contains servcie ID and attribute ID
 *                        to indicate exact which attribute will be used to send notification.
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_notification(struct gatt_send_event *ntf_att);

/*
 * @fn          gatt_notification_with_handle
 *
 * @brief       Sending notification.
 *
 * @param[in]   ntf_att : in which service and which attribute the notification will be sent
 *                        This parameter contains servcie ID and attribute handle
 *                        to indicate exact which attribute will be used to send notification.
 *
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_notification_with_handle(struct gatt_send_event_handle *ntf_att);

/*********************************************************************
 * @fn          gatt_indication
 *
 * @brief       Sending indication.
 *
 * @param[in]   ind_att : in which service and which attribute the indication will be sent
 *                        This parameter contains servcie ID and attribute ID
 *                        to indicate exact which attribute will be used to send indication.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_indication(struct gatt_send_event *ind_att);

/*********************************************************************
 * @fn          gatt_indication_with_handle
 *
 * @brief       Sending indication.
 *
 * @param[in]   ind_att : in which service and which attribute the indication will be sent
 *                        This parameter contains servcie ID and attribute handle
 *                        to indicate exact which attribute will be used to send indication.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_indication_with_handle(struct gatt_send_event_handle *ind_att);

/*
 * @fn          gatt_mtu_exchange_req
 *
 * @brief       Exchange MTU with peer device.
 *
 * @param[in]   user_id : client index or service index, this is return value after calling gatt_add_client() or gatt_add_service().
 * @param[in]   conidx : connection index
 * @param[in]   mtu : 
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_mtu_exchange_req(uint8_t user_id, uint8_t conidx, uint16_t mtu);

/*
 * @fn          gatt_mtu_get
 *
 * @brief       Get the current MTU value of the specified device.
 *
 * @param[in]   conidx : connection index
 * @param[in]   *mtu : Pointer to the return value of MTU
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_mtu_get(uint8_t conidx, uint16_t *mtu);

/*
 * @fn          gatt_set_authorization
 *
 * @brief       Authorize a attribute to a specified device.
 *
 * @param[in]   param : param of authorization.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_set_authorization(struct gatt_set_authorization *param);

/*
 * @fn          gatt_att_info_change
 *
 * @brief       Change the prop or uuid of a service or attribute.
 *              The attributes table(struct type is gatt_attributi_t) of the service shall not be of const type.
 *              If ok, a service changed indication will be automatically sent to 
 *              all clients who have written the CCC of the local service changed characteristics(uuid: 0x2A05)
 *
 * @param[in]   param : param of new info.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_att_info_change(struct gatt_service_change *param);

/*
 * @fn          gatt_svc_changed_ind
 *
 * @brief       send a service changed indication to specified device.
 *
 * @param[in]   param : param of service changed.
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_svc_changed_ind(struct gatt_send_svc_changed_param *p_param);

/*
 * @fn          gatt_client_event_register
 *
 * @brief       Command used by a GATT client user to register for reception of events (notification / indication) for a given handle range.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   start_hdl : start handle of the handle range, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   end_hdl : end handle of the handle range, shall be set in the range of 0x0001 - 0xffff
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_event_register(uint8_t client_id, uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*
 * @fn          gatt_client_event_unregister
 *
 * @brief       Command used by a GATT client user to stop reception of events (notification / indication) onto a specific handle range.
 *
 * @param[in]   client_id : client index, this is return value after calling gatt_add_client.
 * @param[in]   conidx : connection index
 * @param[in]   start_hdl : start handle of the handle range, shall be set in the range of 0x0001 - 0xffff
 * @param[in]   end_hdl : end handle of the handle range, shall be set in the range of 0x0001 - 0xffff
 * 
 * @return      @ref ble_api_err_t.
 */
ble_api_err_t gatt_client_event_unregister(uint8_t client_id, uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

#endif  // _GATT_API_H

