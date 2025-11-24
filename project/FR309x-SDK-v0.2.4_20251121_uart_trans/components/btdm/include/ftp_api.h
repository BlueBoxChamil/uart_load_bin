#ifndef __FTP_API_H
#define __FTP_API_H

#include "goep_api.h"

/*---------------------------------------------------------------------------
 * File Transfer Profile (Ftp) Layer 
 *
 *     The FTP layer provides two levels of service for applications
 *     implementing the File Transfer profile. The basic service provides
 *     functions for registering the FTP SDP entries and for building
 *     FTP SDP queries. These functions can be used directly with the
 *     OBEX API. The expanded services include functions which expand the
 *     basic GOEP API to include Ftp specific functionality.
 */

/* Functions used by macros below */
ObStatus FTP_RegisterClnt(GoepClientApp *Client, const ObStoreFuncTable *ObStoreFuncs,
                          BtSecurityParms *SecParms, int UseOldSecApis);

/****************************************************************************
 *
 * Section: Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * FTP_RegisterClient()
 *
 *     NOTE: This API is deprecated.  Use FTP_RegisterClientSec instead.
 *
 *     Registers the file transfer client with the GOEP multiplexor.
 *     All of the events specified in the GOEP layer are delivered to the
 *     FTP client, except for GOEP_EVENT_TP_CONNECTED.
 *     
 * Parameters:
 *     Client - The client application's registration structure. The 
 *          'callback' and 'obstore' fields must be filled in.
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
 *     OB_STATUS_BUSY - An Ftp client is already registered.
 *     
 */
ObStatus FTP_RegisterClient(GoepClientApp *Client, 
                            const ObStoreFuncTable *ObStoreFuncs);
#define FTP_RegisterClient(_CLIENT, _OBSH) \
        FTP_RegisterClnt(_CLIENT, _OBSH, 0, TRUE);

/*---------------------------------------------------------------------------
 * FTP_RegisterClientSec()
 *
 *     Registers the file transfer client with the GOEP multiplexor.
 *     All of the events specified in the GOEP layer are delivered to the
 *     FTP client, except for GOEP_EVENT_TP_CONNECTED. This routine also 
 *     registers a security record for the client.
 *     
 * Parameters:
 *     Client - The client application's registration structure. The 
 *          'callback' and 'obstore' fields must be filled in.
 *
 *     ObStoreFuncs - Pointer to the Object Store Function Table provided 
 *          by the application layer.  These functions are passed down to 
 *          the OBEX protocol layer in the ObexAppHandle structure.
 *
 *     SecParms - Pointer to the Security Parameters (level and PIN length)
 *          used by the FTP Client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The OBEX Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the OBEX Client.
 *
 *     OB_STATUS_BUSY - An Ftp client is already registered.
 *     
 */
ObStatus FTP_RegisterClientSec(GoepClientApp *Client, 
                               const ObStoreFuncTable *ObStoreFuncs,
                               BtSecurityParms *SecParms);
#define FTP_RegisterClientSec(_CLIENT, _OBSH, _PARMS) \
        FTP_RegisterClnt(_CLIENT, _OBSH, _PARMS, FALSE);

/*---------------------------------------------------------------------------
 * FTP_Connect()
 *
 *     This function is used by the client to create a transport connection
 *     to the specified device and issue an OBEX Connect Request. The FTP
 *     Target header is automatically sent in the OBEX Connect request.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Target - This structure describes the server to which the client
 *         wishes to connect.
 *
 *     ConnReq - Optional connect request parameters. This parameter may
 *         be zero if no directed connection headers, authentication headers,
 *         or SRM settings are necessary.
 *
 * Returns:
 *     OB_STATUS_PENDING - Connection was successfully started. Completion
 *         will be signaled by an event to the application callback. 
 *
 *         If the connection is successful, a GOEP_EVENT_COMPLETE event for
 *         the GOEP_OPER_CONNECT operation will be signaled.
 *
 *         If the transport connection is successful, but the OBEX Connect
 *         failed, the completion event will be GOEP_EVENT_ABORTED for the
 *         operation GOEP_OPER_CONNECT. At this point the transport is
 *         CONNECTED but a successful OBEX Connect has not been sent.
 *         Additional calls to this function will generate new OBEX Connect
 *         requests. 
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
ObStatus FTP_Connect(GoepClientApp *Client, ObexTpAddr *Target, 
                         GoepConnectReq *ConnReq);

/*---------------------------------------------------------------------------
 * FTP_Disconnect()
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
ObStatus FTP_Disconnect(GoepClientApp *Client);
#define FTP_Disconnect(_CLIENT) GOEP_Disconnect(_CLIENT)

/*---------------------------------------------------------------------------
 * FTP_TpDisconnect()
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
ObStatus FTP_TpDisconnect(GoepClientApp *Client);
#define FTP_TpDisconnect(_CLIENT) GOEP_TpDisconnect(_CLIENT)

/*---------------------------------------------------------------------------
 * FTP_ClientAbort()
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
ObStatus FTP_ClientAbort(GoepClientApp *Client);
#define FTP_ClientAbort(_CLIENT) GOEP_ClientAbort(_CLIENT)

/*---------------------------------------------------------------------------
 * FTP_Push()
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
ObStatus FTP_Push(GoepClientApp *Client, GoepObjectReq *Object);
#define FTP_Push(_CLIENT, _OBJECT) GOEP_Push(_CLIENT, _OBJECT)

/*---------------------------------------------------------------------------
 * FTP_Pull()
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
 *     request will be sent when FTP_ClientContinue is called during the 
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
ObStatus FTP_Pull(GoepClientApp *Client, GoepObjectReq *object, int more);
#define FTP_Pull(_CLIENT, _OBJECT, _MORE) GOEP_Pull(_CLIENT, _OBJECT, _MORE)

/*---------------------------------------------------------------------------
 * FTP_Delete()
 *
 *     Initiates the OBEX "Put" operation to delete an object on the remote
 *     server. The 'ObjectName' field is required.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     ObjectName - The name of the object to delete on the server.
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
 */
ObStatus FTP_Delete(GoepClientApp *Client, const GoepUniType *ObjectName);

/*---------------------------------------------------------------------------
 * FTP_SetFolderFwd()
 *
 *     Performs the OBEX "SetPath" operation to a new folder.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     FolderName - The name of the child folder to change to (required).
 *
 *     AllowCreate - Set to TRUE to allow the server to create the folder
 *          if it does not exist (like a CreateFolder function).
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
 */
ObStatus FTP_SetFolderFwd(GoepClientApp *Client, 
                           const GoepUniType *FolderName, 
                           int AllowCreate);

/*---------------------------------------------------------------------------
 * FTP_SetFolderBkup()
 *
 *     Performs the OBEX "SetPath" operation to change to the parent folder.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
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
 */
ObStatus FTP_SetFolderBkup(GoepClientApp *Client);

/*---------------------------------------------------------------------------
 * FTP_SetFolderRoot()
 *
 *     Performs the OBEX "SetPath" operation to change to the root folder.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
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
 */
ObStatus FTP_SetFolderRoot(GoepClientApp *Client);

/*---------------------------------------------------------------------------
 * FTP_ClientContinue()
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
ObStatus FTP_ClientContinue(GoepClientApp *Client);
#define FTP_ClientContinue(_CLIENT) GOEP_ClientContinue(_CLIENT)

#endif /* __FTP_API_H */
