#ifndef _BT_ERROR_H
#define _BT_ERROR_H
#include <stdint.h>

#include "cmsis_compiler.h"

#include "btconfig.h"

#ifndef LISTS_DEFINED
#define LISTS_DEFINED
typedef struct  _ListEntry 
{
    struct _ListEntry *Flink;
    struct _ListEntry *Blink;

} ListEntry;

#define InitializeListEntry(Entry) (\
    (Entry)->Flink = (Entry)->Blink = 0 )

#endif

typedef __PACKED_STRUCT bdaddr_t {
    uint8_t A[6];
} BD_ADDR;

__PACKED_STRUCT class_of_device_t {
    uint8_t  A[3];
};

__PACKED_STRUCT link_key_t{
    uint8_t  A[16];
};


#define XA_DISABLED 0   /* Disable the feature */
#define XA_ENABLED  1   /* Enable the feature */

/*---------------------------------------------------------------------------
 * BtStatus type
 *
 *     This type is returned from most stack APIs to indicate the success
 *     or failure of the operation. In many cases, BT_STATUS_PENDING
 *     is returned, meaning that a future callback will indicate the
 *     result of the operation.
 */
typedef int16_t BtStatus;

#define BT_STATUS_SUCCESS           0
#define BT_STATUS_FAILED            1
#define BT_STATUS_PENDING           2
#define BT_STATUS_BUSY              11
#define BT_STATUS_NO_RESOURCES      12
#define BT_STATUS_NOT_FOUND         13
#define BT_STATUS_DEVICE_NOT_FOUND  14
#define BT_STATUS_CONNECTION_FAILED 15
#define BT_STATUS_TIMEOUT           16
#define BT_STATUS_NO_CONNECTION     17
#define BT_STATUS_INVALID_PARM      18
#define BT_STATUS_IN_PROGRESS       19
#define BT_STATUS_RESTRICTED        20
#define BT_STATUS_INVALID_TYPE      21
#define BT_STATUS_HCI_INIT_ERR      22
#define BT_STATUS_NOT_SUPPORTED     23
#define BT_STATUS_IN_USE            5
#define BT_STATUS_SDP_CONT_STATE    24
#define BT_STATUS_CANCELLED         25

/* The last defined status code */
#define BT_STATUS_LAST_CODE         25

/* Protocol extensions can safely add status codes by defining
 * them as (BT_STATUS_OPEN + x).
 */
#define BT_STATUS_OPEN              30

/* End of BtStatus */

#define BD_ADDR_SIZE    6

#ifndef NULL
	#define NULL 0
#endif

#ifndef SUCCESS
	#define SUCCESS     0
#endif

#ifndef FAILURE
	#define FAILURE     1
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define BIT0                            0x00000001
#define BIT1                            0x00000002
#define BIT2                            0x00000004
#define BIT3                            0x00000008
#define BIT4                            0x00000010
#define BIT5                            0x00000020
#define BIT6                            0x00000040
#define BIT7                            0x00000080
#define BIT8                            0x00000100
#define BIT9                            0x00000200
#define BIT10                           0x00000400
#define BIT11                           0x00000800
#define BIT12                           0x00001000
#define BIT13                           0x00002000
#define BIT14                           0x00004000
#define BIT15                           0x00008000
#define BIT16                           0x00010000
#define BIT17                           0x00020000
#define BIT18                           0x00040000
#define BIT19                           0x00080000
#define BIT20                           0x00100000
#define BIT21                           0x00200000
#define BIT22                           0x00400000
#define BIT23                           0x00800000
#define BIT24                           0x01000000
#define BIT25                           0x02000000
#define BIT26                           0x04000000
#define BIT27                           0x08000000
#define BIT28                           0x10000000
#define BIT29                           0x20000000
#define BIT30                           0x40000000
#define BIT31                           0x80000000

#if BT_SCO_HCI_DATA == XA_ENABLED

/*---------------------------------------------------------------------------
 * CmgrAudioData structure
 *
 *     Contains audio data.
 */
typedef struct _CmgrAudioData {
    uint16_t       len;
    uint8_t       *data;

#if BT_STACK_VERSION >= 300
    uint8_t        errFlags;
#endif
    
} CmgrAudioData;

/*---------------------------------------------------------------------------
 * HfAudioData type
 *
 * This type is only available when BT_SCO_HCI_DATA is set to XA_ENABLED.  
 * It can be used to store audio data received from the remote device.  
 */
typedef CmgrAudioData HfAudioData;

/* End of HfAudioData */ 

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * BtPacket structure
 *
 *     Represents a packet of data. These packets may be used by applications
 *     to provide data to the stack. Certain stack events may also provide
 *     data to the application enclosed in a BtPacket structure.
 */
typedef struct _BtPacket
{
    ListEntry         node;    /* Used internally by the stack. */

    uint8_t          *data;    /* Points to a buffer of user data.  */
    uint16_t          dataLen; /* Indicates the length of "data" in bytes. */

    uint16_t          flags;   /* Must be initialized to BTP_FLAG_NONE by
                           * applications running on top of L2CAP. Other
                           * higher layer protocols must never modify this
                           * value.
                           */

#if L2CAP_PRIORITY == XA_ENABLED
    BtPriority   priority; /* May be used to override the L2CAP channel
                            * BtPriority. If set to 0, packet will use the
                            * default priority set for the L2CAP channel.
                            */
#endif

    /* Group: The following fields are for internal use only by the stack. */
    void             *ulpContext;
    uint8_t          *tail;
    uint16_t          tailLen;

#if XA_STATISTICS == XA_ENABLED
    uint32_t          rfc_timer;
    uint32_t          hci_timer;
    uint32_t          l2cap_timer;
#endif /* XA_STATISTICS == XA_ENABLED */

    uint16_t          llpContext;
    uint16_t          remoteCid;
    
#if L2CAP_NUM_ENHANCED_CHANNELS > 0
    uint8_t           segStart;
    uint16_t          segNum;
    uint16_t          segCount;
    uint8_t           fcs[2];
#endif /* L2CAP_NUM_ENHANCED_CHANNELS > 0 */

    uint8_t           hciPackets;
    uint8_t           headerLen;
    uint8_t           header[BT_PACKET_HEADER_LEN];
} BtPacket;

#if 1
typedef struct _EvmTimer EvmTimer;

/*---------------------------------------------------------------------------
 * Callback used to notify a stack component that the timer expired.
 */
typedef void (*EvmTimerNotify)(EvmTimer*);

typedef void (*co_time_timer_cb)(void* p_env);

/// Timer structure
typedef struct co_time_timer
{
    /// Pointer to next timer in timer list
    struct co_time_timer * p_next;
    /// Pointer to environment that will be used as callback parameter.
    void*                  p_env;
    /// Callback to execute in background context upon timer expiration
    co_time_timer_cb       cb;
    /// Expiration time [0-31] part (in milliseconds)
    uint32_t                    exp_time_ms_lsb;
    /// Timer bit field (@see enum co_time_timer_bf)
    uint32_t                    timer_bf;
} co_time_timer_t;

/*---------------------------------------------------------------------------
 * EvmTimer structure
 *
 *     Represents a stack timer. This structure is for use by
 *     internal stack components only.
 */
struct _EvmTimer
{

    void            *context;  /* Context area for use by callers */
    EvmTimerNotify   func;     /* Function to call when timer fires */
    struct co_time_timer co_time;
};
#endif
#endif
