#ifndef __SDP_API_H
#define __SDP_API_H

#include "bt_types.h"
#include "me_api.h"

typedef uint16_t SdpAttributeId;

/*---------------------------------------------------------------------------
 * SdpAttribute structure
 *
 *     Defines an attribute's ID and value. SdpAttribute structures
 *     are stored in a SdpRecord prior to calling the SDP_AddRecord
 *     function. 
 */
typedef struct _SdpAttribute
{
    SdpAttributeId   id;       /* Attribute ID. */

    uint16_t              len;      /* Length of the value buffer */
    
    const uint8_t       *value;    /* An array of bytes that contains the value
                                * of the attribute. The buffer is in
                                * Data Element form (see SdpDataElemType
                                * and SdpDataElemSize).
                                */
    
    /* Group: The following field is for internal use only */
    uint16_t              flags;

} SdpAttribute;

/*---------------------------------------------------------------------------
 * SdpRecord structure
 *
 *     Defines the contents of a service record. Service records are
 *     initialized and provided to SDP_AddRecord.
 */
typedef struct _SdpRecord
{    
    ListEntry       node;    /* For internal use only. */
    
    uint8_t              num;     /* Total number of attributes in "attribs". */

    /* Pointer to an array of attributes.
     *
     * Store the attributes in ascending order by attribute ID, and
     * do not add two attributes with the same ID.
     *
     * Do not include an AID_SERVICE_RECORD_HANDLE attribute. This
     * attribute is handled automatically by SDP.
     */
    SdpAttribute   *attribs;
  
    /* The service class fields of the class of device. Use the values defined
     * in me.h. The device class portion is ignored.
     */
    BtClassOfDevice classOfDevice; 

    /* Group: The following fields are for internal use only. */    
    uint32_t             handle;         /* Service Record Handle */
    uint32_t             recordState;    /* Service Record State */
    uint16_t             flags;          /* Flag to keep track of marked attributes */
    uint16_t             handleFlag;     /* Flag to keep track of service record handle */
    uint16_t             stateFlag;      /* Flag to keep track of service record state */
  
} SdpRecord;

typedef struct 
{
    uint8_t  *sdpatt_table;
    uint8_t  *sdpdeviceId_table;
    
    uint16_t att_table_size;
    uint16_t deviceId_size;
}sdp_AttTable_Info_t;

#endif