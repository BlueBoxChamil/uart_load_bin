#ifndef _OPP_API_H
#define _OPP_API_H

#include "goep_api.h"

/*---------------------------------------------------------------------------
 * Object Push Profile (OPush) Layer 
 *
 *     The OPUSH layer provides two levels of service for applications
 *     implementing the Object Push profile. The basic service provides
 *     functions for registering the OPush SDP entries and for building
 *     OPush SDP queries. These functions can be used directly with the
 *     OBEX API. The expanded services include functions for building
 *     and sending OPush compliant messages.
 */

/* Functions used by macros below */
ObStatus OPUSH_RegisterClnt(GoepClientApp *Client, 
                            const ObStoreFuncTable *ObStoreFuncs,
                            BtSecurityParms *SecParms,
                            int UseOldSecApis);

/****************************************************************************
 *
 * Section: Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * OPUSH_RegisterClient()
 *
 *     NOTE: This API is deprecated.  Use OPUSH_RegisterClientSec instead.
 *
 *     Registers the object push client with the GOEP multiplexor.
 *     All of the events specified in the GOEP layer are delivered to the
 *     OPUSH client, except for GOEP_EVENT_TP_CONNECTED.
 *     
 * Parameters:
 *     Client - The client application's registration structure. The 
 *          'callback' field must be filled in.
 *
 *     ObStoreFuncs - Pointer to the Object Store Function Table provided 
 *          by the application layer.  These functions are passed down to 
 *          the OBEX protocol layer in the ObexAppHandle structure.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The OBEX Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the OBEX Client.
 *
 *     OB_STATUS_BUSY - An OPush client is already registered.
 *     
 */
ObStatus OPUSH_RegisterClient(GoepClientApp *Client, 
                              const ObStoreFuncTable *ObStoreFuncs);
#define OPUSH_RegisterClient(_CLIENT, _OBSH) \
        OPUSH_RegisterClnt(_CLIENT, _OBSH, 0, TRUE);

/*---------------------------------------------------------------------------
 * OPUSH_RegisterClientSec()
 *
 *     Registers the object push client with the GOEP multiplexor.
 *     All of the events specified in the GOEP layer are delivered to the
 *     OPUSH client, except for GOEP_EVENT_TP_CONNECTED. This routine also 
 *     registers a security record for the client.
 *     
 * Parameters:
 *     Client - The client application's registration structure. The 
 *          'callback' field must be filled in.
 *
 *     ObStoreFuncs - Pointer to the Object Store Function Table provided 
 *          by the application layer.  These functions are passed down to 
 *          the OBEX protocol layer in the ObexAppHandle structure.
 *
 *     SecParms - Pointer to the Security Parameters (level and PIN length)
 *          used by the OPUSH Client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The OBEX Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the OBEX Client.
 *
 *     OB_STATUS_BUSY - An OPush client is already registered.
 *     
 */
ObStatus OPUSH_RegisterClientSec(GoepClientApp *Client, 
                                 const ObStoreFuncTable *ObStoreFuncs,
                                 BtSecurityParms *SecParms);
#define OPUSH_RegisterClientSec(_CLIENT, _OBSH, _PARMS) \
        OPUSH_RegisterClnt(_CLIENT, _OBSH, _PARMS, FALSE);

/*---------------------------------------------------------------------------
 * OPUSH_Connect()
 *
 *     This function is used by the client to create a transport connection
 *     to the specified device and issue an OBEX Connect Request.
 *
 * Parameters:
 *     Client - The structure used to register the client.
 *     
 *     Target - This structure describes the server to which the client
 *         wishes to connect.
 *
 *     Req - This structure describes the SRM settings for the connection.
 *         This parameter is optional, based on if SRM is being used for 
 *         this connection. Only enabled when OBEX_SRM_MODE is XA_ENABLED. 
 *
 * Returns:
 *     OB_STATUS_PENDING - Connection was successfully started. Completion
 *         will be signaled via a call to the application callback.
 *
 *         If the connection is successful, a GOEP_EVENT_COMPLETE event for
 *         the GOEP_OPER_CONNECT operation will be signaled.
 *
 *         If the transport connection is successful, but the OBEX Connect
 *         failed, the completion event will be GOEP_EVENT_TP_DISCONNECTED
 *         for the operation GOEP_OPER_CONNECT. At this point the transport
 *         is DISCONNECTED. 
 * 
 *         If the transport connection is unsuccessful, the completion event
 *         will be GOEP_EVENT_TP_DISCONNECTED, GOEP_EVENT_DISCOVERY_FAILED, or
 *         GOEP_EVENT_NO_SERVICE_FOUND.
 *
 *     OB_STATUS_SUCCESS - The client is now connected.
 *
 *     OB_STATUS_FAILED - Unable to start the operation because the client
 *         is in the middle of starting up a connection.
 *     
 *     OB_STATUS_BUSY - The client is currently executing an operation.
 *
 */
ObStatus OPUSH_Connect(GoepClientApp *Client, ObexTpAddr *Target);

/*---------------------------------------------------------------------------
 * OPUSH_Disconnect()
 *
 *     This function is used by the client to issue an OBEX Disconnect Request.
 *     When the OBEX Disconnect is completed, the transport connection will
 *     be disconnected automatically.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started.
 *         Completion will be signaled with the GOEP_EVENT_TP_DISCONNECTED 
 *         event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 */
ObStatus OPUSH_Disconnect(GoepClientApp *Client);
#define OPUSH_Disconnect(_CLIENT) GOEP_Disconnect(_CLIENT)

/*---------------------------------------------------------------------------
 * OPUSH_TpDisconnect()
 *
 *     Initiates the disconnection of the clients transport connection.
 *     
 * Parameters:
 *     Client - The registered client requesting the disconnect.
 *     
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started.
 *         Completion will be signaled via an event to the application
 *         callback.
 *
 *     OB_STATUS_NO_CONNECT - Operation failed because there is
 *         no client connection.
 *
 *     OB_STATUS_FAILED - Operation failed.
 */
ObStatus OPUSH_TpDisconnect(GoepClientApp *Client);
#define OPUSH_TpDisconnect(_CLIENT) GOEP_TpDisconnect(_CLIENT)

/*---------------------------------------------------------------------------
 * OPUSH_ClientAbort()
 *
 *     Aborts the current client operation. The completion event will signal
 *     the status of the operation in progress, either COMPLETE or ABORTED.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_SUCCESS - Operation was successful.
 *
 *     OB_STATUS_FAILED - Operation failed to start.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *
 *     OB_STATUS_INVALID_PARM - Invalid parameter.
 */
ObStatus OPUSH_ClientAbort(GoepClientApp *Client);
#define OPUSH_ClientAbort(_CLIENT) GOEP_ClientAbort(_CLIENT)

/*---------------------------------------------------------------------------
 * OPUSH_Push()
 *
 *     Initiates the OBEX "Put" operation to send an object to the remote
 *     server.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Object - Parameters necessary to build a push request. This
 *         parameter must be provided.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 *
 *     OB_STATUS_FAILED - Operation was not started because
 *         of an underlying transport problem.
 *
 *     OB_STATUS_INVALID_HANDLE - The object store handle was invalid.
 *     
 */
ObStatus OPUSH_Push(GoepClientApp *Client, GoepObjectReq *Object);
#define OPUSH_Push(_CLIENT, _OBJECT) GOEP_Push(_CLIENT, _OBJECT)

/*---------------------------------------------------------------------------
 * OPUSH_Pull()
 *
 *     Initiates the OBEX "Get" operation to retrieve an object from
 *     the remote server.  
 *
 *     If this operation fails due to a failure in adding queued headers
 *     (GOEP_ClientQueueHeader), it may be useful to set the "More" parameter 
 *     when reissuing this command.  Doing so will allow the GET operation to
 *     send all the headers that will fit in the existing GET request, but will 
 *     also force the GET request to not send the final bit. This allows
 *     subsequent GET request packets to send additional headers prior to a 
 *     GET response sending back the object being described in the GET request 
 *     headers.  Any additional headers that cannot fit in the initial GET 
 *     request will be sent when OPUSH_ClientContinue is called during the 
 *     GOEP_EVENT_CONTINUE.  GOEP_ClientQueueHeader should be called before or 
 *     during this event to ensure they are sent in the next GET request packet.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Object - Parameters necessary to build a pull request. This
 *         parameter must be provided.
 *
 *     More - More headers exist in addition to the queued headers already
 *         added via GOEP_ClientQueueHeader.  
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 *
 *     OB_STATUS_INVALID_HANDLE - The object store handle was invalid.
 */
ObStatus OPUSH_Pull(GoepClientApp *Client, GoepObjectReq *object, int more);
#define OPUSH_Pull(_CLIENT, _OBJECT, _MORE) GOEP_Pull(_CLIENT, _OBJECT, _MORE)

/*---------------------------------------------------------------------------
 * OPUSH_ClientContinue()
 *
 *     This function is called by the client in response to a received
 *     GOEP_EVENT_CONTINUE event. It must be called once for every
 *     CONTINUE event received. It may or may not be called in the context
 *     of the callback and can be deferred for flow control purposes.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_FAILED - The client is not expecting a continue.
 *
 *     OB_STATUS_SUCCESS - The continue was successful.
 */
ObStatus OPUSH_ClientContinue(GoepClientApp *Client);
#define OPUSH_ClientContinue(_CLIENT) GOEP_ClientContinue(_CLIENT)

#endif  // _OPP_API_H
