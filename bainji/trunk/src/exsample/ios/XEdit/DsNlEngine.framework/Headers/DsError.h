#ifndef  __dsErrors_h_h
#define	__dsErrors_h_h

#include "DsNleDef.h"


namespace DSNleLib
{
   

#define DS_NOERROR 0 // No error           
#define DS_TRUE    0 // TRUE
#define DS_FALSE   1 // FALSE

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ------------------------ ERROR CODES ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#define DS_E_FAIL                                              MAKE_DSHRESULTERROR (0x8FFF)  // Generic error.
#define DS_E_INVALID_STRUCTURE_SIZE                            MAKE_DSHRESULTERROR (0x8001)  // Structure size not set or not valid.
#define DS_E_NOT_SUPPORTED                                     MAKE_DSHRESULTERROR (0x8002)  // The requested operation is not supported.
#define DS_E_NOT_IMPLEMENTED                                   MAKE_DSHRESULTERROR (0x8003)  // The code is not implemented yet for the requested operation.
#define DS_E_ALREADY_ATTACHED                                  MAKE_DSHRESULTERROR (0x8004)  // A custom object with the same GUID is already attached.
#define DS_E_NOT_FOUND                                         MAKE_DSHRESULTERROR (0x8005)  // No custom object of this GUID has been attached or requested item could not be found.
#define DS_E_LIST_CORRUPTED                                    MAKE_DSHRESULTERROR (0x8006)  // Internal error found in the list. List is corrupted.
#define DS_E_AVCONTENT_IN_USE                                  MAKE_DSHRESULTERROR (0x8007)  // Invalid synchronization usage. Reset and Flush can't be performed if write and read count are not zero.
#define DS_E_AVCONTENT_NOT_FLUSHED                             MAKE_DSHRESULTERROR (0x8008)  // Alias reference can't be re-assigned if the AVContent hasn't been flushed.
#define DS_E_REFERENCE_NOT_ASSIGNED                            MAKE_DSHRESULTERROR (0x8009)  // This operation can't be performed since no user or alias reference has been assigned yet.
#define DS_E_DEVICE_NOT_PRESENT                                MAKE_DSHRESULTERROR (0x800A)  // The module needed for this operation is not properly installed or is not present on the current engine profile.
#define DS_E_SETTINGS_MANAGER_NOT_PRESENT                      MAKE_DSHRESULTERROR (0x800B)  // The settings manager is not properly installed.
#define DS_E_CLOCK_NOT_PRESENT                                 MAKE_DSHRESULTERROR (0x800C)  // The clock driver is not properly installed.
#define DS_E_CANNOT_CREATE_ENGINE_CREATOR                      MAKE_DSHRESULTERROR (0x800D)  //  Engine creator is not properly installed.
#define DS_E_INTERNAL_DIALOG_ERROR                             MAKE_DSHRESULTERROR (0x800E)  // The  Creator helper had an internal error. The main application should quit.
#define DS_E_USER_CANCELLED_DIALOG                             MAKE_DSHRESULTERROR (0x800F)  // The user clicked cancel in the profile or resoultion selection dialog. The main application should quit.
#define DS_E_NO_HARDWARE_PROFILE_FOUND                         MAKE_DSHRESULTERROR (0x8010)  // No hardware profile available in the system.
#define DS_E_NO_RESOURCE_FOUND                                 MAKE_DSHRESULTERROR (0x8011)  // Forgot to put <#include DsfHelper.rc> in the application's resource viewer|.RC properties|Resource Includes|Compile-time Directives
#define DS_E_END_OF_ENUM                                       MAKE_DSHRESULTERROR (0x8012)  // No more items to enumerate.
#define DS_E_NOT_SUPPORTED_BY_SURFACE_CONTAINER                MAKE_DSHRESULTERROR (0x8013)  // This surface operation can't be performed since the object is a surface container. A GetObject must
#define DS_E_INVALID_DEVICE_COUNT                              MAKE_DSHRESULTERROR (0x8014)  // The requested device is not installed. 
#define DS_E_INVALID_RESOLUTIONS_COUNT                         MAKE_DSHRESULTERROR (0x8015)  // The number or resolutions specified is different than the one expected.
#define DS_E_INRES_GENRES_INCOMPATIBLE_COMBINATION             MAKE_DSHRESULTERROR (0x8022)  // The input resolution is not compatible with the genlock resolution.
#define DS_E_OUTRES_GENRES_INCOMPATIBLE_COMBINATION            MAKE_DSHRESULTERROR (0x8023)  // The output resolution is not compatible with the genlock resolution
#define DS_E_MFACTOR_INCOMPATIBLE_COMBINATION                  MAKE_DSHRESULTERROR (0x8024)  // FrameRate M factors not compatible between genlock and input/output resolution
#define DS_E_INVALID_IODEVICE_COMBINATION                      MAKE_DSHRESULTERROR (0x8025)  // The combination of only one ABOB (or ABobSDI) and only one HDBOB is supported.
#define DS_E_INVALID_DEVICE_INDEX                              MAKE_DSHRESULTERROR (0x8026)  // The requested IO Device index is not valid.
#define DS_E_UNSUPPORTED_IOCOMMAND                             MAKE_DSHRESULTERROR (0x8027)  // The requested IO command is not supported.
#define DS_E_NO_FX_FOR_OUTPUT_SURFACE_LOCATION                 MAKE_DSHRESULTERROR (0x8028)  // The effect or composition can't process in the desired output surface memory location.
#define DS_E_DEVICE_MATCHING_FAILED                            MAKE_DSHRESULTERROR (0x8029)  // Can't match input and output device to IO device(s).
#define DS_E_MAX_FLEXENGINE_REACHED                            MAKE_DSHRESULTERROR (0x802B)  // No more  Engines can be created.
#define DS_E_ENGINE_ALREADY_CREATED                            MAKE_DSHRESULTERROR (0x802C)  // A  Engine is already created on the desired hardware profile.
#define DS_E_CO_INITIALIZE_NOT_DONE                            MAKE_DSHRESULTERROR (0x802D)  // Application must call COInitialize before using COM objects.
#define DS_E_TOO_MANY_INPUT_SURFACES                           MAKE_DSHRESULTERROR (0x802E)  // Allowed number of effect input surfaces is exceeded.
#define DS_E_MODULE_BUSY                                       MAKE_DSHRESULTERROR (0x802F)  // The module is still processing the last requested operation.
#define DS_E_NO_INPUT_SURFACE                                  MAKE_DSHRESULTERROR (0x8030)  // This method requires at least one input surface.
#define DS_E_NO_INPUT_CONTAINER                                MAKE_DSHRESULTERROR (0x8031)  // Effect needs effect pattern.
#define DS_E_READ_COUNT_NOT_INCREMENTED                        MAKE_DSHRESULTERROR (0x8032)  // Each SignalReadCompletion should match a IncrementReadCount.
#define DS_E_WRITE_COUNT_NOT_INCREMENTED                       MAKE_DSHRESULTERROR (0x8033)  // Each SignalWriteCompletion should match a IncrementWriteCount.
#define DS_E_MARKWRITE_ERROR                                   MAKE_DSHRESULTERROR (0x8034)  // MarkWriteStartTime not done OR MarkWriteStopTime not done OR MarkWriteStartTime done after MarkWriteStopTime.
#define DS_E_INRES_OUTRES_INCOMPATIBLE_COMBINATION             MAKE_DSHRESULTERROR (0x8035)  // The combination of input resolution and output resolution is invalid.
#define DS_E_NOT_READY_FOR_READ                                MAKE_DSHRESULTERROR (0x8036)  // The host processor module expects the sources to be DS_FALSE for read.
#define DS_E_COPY_FORMATS_COMBINATION_NOT_SUPPORTED            MAKE_DSHRESULTERROR (0x8037)  // The surface format of the source and destination surface imply a special copy that is not supported.
#define DS_E_COPY_RECTANGLES_MISMATCH                          MAKE_DSHRESULTERROR (0x8038)  // The source and destination rectangles specified are invalid or imply an unsupported copy.
#define DS_E_POINTER_NOT_ALIGNED                               MAKE_DSHRESULTERROR (0x8039)  // The data pointer of the source and/or destination surface is not properly aligned.
#define DS_E_FORMAT_NOT_SUPPORTED                              MAKE_DSHRESULTERROR (0x803A)  // The format of the surface or audiosamples are not supported in the requested processing.
#define DS_E_INPUT_FORMAT_NOT_SUPPORTED                        MAKE_DSHRESULTERROR (0x803B)  // The format of the input surface or audiosamples is not supported in the requested processing.
#define DS_E_OUTPUT_FORMAT_NOT_SUPPORTED                       MAKE_DSHRESULTERROR (0x803C)  // The format of the output surface or audiosamples is not supported in the requested processing.
#define DS_E_PROCESSING_FORMAT_NOT_SUPPORTED                   MAKE_DSHRESULTERROR (0x803D)  // The source and destination format imply a processing that is not supported.
#define DS_E_DESTINATION_BUFFER_TOO_SMALL                      MAKE_DSHRESULTERROR (0x803E)  // Destination buffer is too small.
#define DS_E_BUFFER_SIZE_MISMATCH                              MAKE_DSHRESULTERROR (0x803F)  // Input buffers (audiosamples) must have the same number of samples.
#define DS_E_BUFFER_FORMAT_MISMATCH                            MAKE_DSHRESULTERROR (0x8040)  // Input buffers or output buffers (audiosamples) must have the same format.
#define DS_E_IOCTL_FAILED                                      MAKE_DSHRESULTERROR (0x8041)  // A kernel driver IO control has failed.
#define DS_E_MUTEX_CREATION_FAILED                             MAKE_DSHRESULTERROR (0x8042)  // A mutex creation has failed.
#define DS_E_OUT_OF_MEMORY                                     MAKE_DSHRESULTERROR (0x8043)  // Not enough memory to perform the operation
#define DS_E_INVALID_PARAMETER                                 MAKE_DSHRESULTERROR (0x8044)  // One of the method parameter is invalid.
#define DS_E_INVALID_POINTER                                   MAKE_DSHRESULTERROR (0x8045)  // One of the method parameter is a uninitialized pointer.
#define DS_E_ALREADY_IN_USE                                    MAKE_DSHRESULTERROR (0x8046)  // Failed the module/object is already in use.
#define DS_E_NOT_INITIALIZED                                   MAKE_DSHRESULTERROR (0x8047)  // Failed because the module/object is not initialized.
#define DS_E_TIME_OUT                                          MAKE_DSHRESULTERROR (0x8048)  // Failed because there was a time out.
#define DS_E_WRONG_STATE                                       MAKE_DSHRESULTERROR (0x8049)  // Generic wrong state error, module/filter/object does not have the right state.
#define DS_E_INVALID_EFFECT_TREE                               MAKE_DSHRESULTERROR (0x804A)  // The specified effect tree cannot be used by the transform filter.
#define DS_E_INVALID_FILE_FORMAT                               MAKE_DSHRESULTERROR (0x804B)  // Unreadable file format.
#define DS_E_NO_PROFILE_LOADED                                 MAKE_DSHRESULTERROR (0x804C)  // Failed because no profile was selected on the filter graph.
#define DS_E_OUT_OF_RANGE                                      MAKE_DSHRESULTERROR (0x804D)  // Failed because a given value is out of range.
#define DS_E_HW_OVERLAY_NOT_AVAILABLE                          MAKE_DSHRESULTERROR (0x804E)  // Hardware overlay for live window display is not available.
#define DS_E_OUT_OF_DIRECTX_MEMORY                             MAKE_DSHRESULTERROR (0x804F)  // Failure when allocating DirectX memory.
#define DS_E_DIRECTX_IS_LOST                                   MAKE_DSHRESULTERROR (0x8050)  // Failure when the DirectX memory and context are lost.
#define DS_E_INVALID_NODE_RELATIONSHIP                         MAKE_DSHRESULTERROR (0x8051)  // Failure when trying to link tree nodes.
#define DS_E_ENUM_OUT_OF_SYNC			                           MAKE_DSHRESULTERROR (0x8052)  // Using a tree node enumerator out of synch with the current state of the tree.
#define DS_E_INVALID_NODE_INDEX		                           MAKE_DSHRESULTERROR (0x8053)  // Trying to force a node index on a reserved value.
#define DS_E_OUT_OF_POOL_MEMORY		                           MAKE_DSHRESULTERROR (0x8054)  // No more memory available in resource manager internal pool.
#define DS_E_END_OF_DISK       		                           MAKE_DSHRESULTERROR (0x8055)  // End Of Disk. 
#define DS_E_INVALID_DATA       		                           MAKE_DSHRESULTERROR (0x8056)  // Invalid data.  
#define DS_E_INVALID_INDEX       		                        MAKE_DSHRESULTERROR (0x8057)  // Invalid index.  
#define DS_E_WRONG_MEDIATYPE       		                        MAKE_DSHRESULTERROR (0x8058)  // Wrong MediaType. Could occur on SetCutlistManager() or pin connection.  
#define DS_E_WRONG_GRAPHMODE       		                        MAKE_DSHRESULTERROR (0x8059)  // Wrong Graph Mode. Could occur when sending Graph state commands.
#define DS_E_GRAPH_HAS_BEEN_BUILT                              MAKE_DSHRESULTERROR (0x805A)  // Graph has already been built. Could occur when user tries to set a number of source(sink, live window) filters after the graph has been built.
#define DS_E_GRAPH_DESTROYED                                   MAKE_DSHRESULTERROR (0x805b)  // GSC uses this to notify the souce loader threads to stop working.
#define DS_E_INVALID_NAME                                      MAKE_DSHRESULTERROR (0x805c)  // Invalid file name. 
#define DS_E_FILE_VERSION_NOT_SUPPORTED                        MAKE_DSHRESULTERROR (0x805D)  // If the file version is not supported.
#define DS_E_ELEMENT_IS_NOT_COMPATIBLE                         MAKE_DSHRESULTERROR (0x805E)  // The element is not compatible with the cutlist.
#define DS_E_ELEMENT_NOT_FOUND                                 MAKE_DSHRESULTERROR (0x805F)  // The specified element is not found in the cutlist.
#define DS_E_CUTLIST_ID_ALREADY_EXISTS                         MAKE_DSHRESULTERROR (0x8060)  // There has been a cutlist with the specified cutlist ID in the cutlist manager.
#define DS_E_CUTLIST_ID_NOT_FOUND		                        MAKE_DSHRESULTERROR (0x8061)  // There is no cutlist with the specified cutlist ID in the cutlist manager.
#define DS_E_WRONG_ERES                                        MAKE_DSHRESULTERROR (0x8062)  // The editing resolution doesn't match the one set in the profile loader.
#define DS_E_POS_NOT_AT_BOUNDARY                               MAKE_DSHRESULTERROR (0x8063)  // The timeline position is not at the boundary of an element.
#define DS_E_POS_NOT_BETWEEN_TWO_SPLIT_ELEMENTS                MAKE_DSHRESULTERROR (0x8064)  // The timeline position is not at the boundary of two split elements.
#define DS_E_EMPTY_RANGE                                       MAKE_DSHRESULTERROR (0x8065)  // The start point and the end point of the specified range are equal.
#define DS_E_CANNOT_CANCEL                                     MAKE_DSHRESULTERROR (0x8066)  // Cannot cancel a surface when it's delivered too late to respect the preroll time of output device.
#define DS_E_INVALID_REQUEST                                   MAKE_DSHRESULTERROR (0x8067)  // A processing request is invalid.
#define DS_E_REQUEST_NOT_READY                                 MAKE_DSHRESULTERROR (0x8068)  // A processing request is not ready to execute.
#define DS_E_REQUEST_NOT_COMPLETED                             MAKE_DSHRESULTERROR (0x8069)  // A processing request is not completed.
#define DS_E_REQUEST_NOT_FLUSHED                               MAKE_DSHRESULTERROR (0x806A)  // A processing request is not flushed.
#define DS_E_ENGINE_ALREADY_REGISTERED                         MAKE_DSHRESULTERROR (0x806B)  // A streaming engine is already registered on the scheduler.
#define DS_E_REQUEST_REJECTED                                  MAKE_DSHRESULTERROR (0x806C)  // A processing request was refused because the scheduler is flushing itself.
#define DS_E_POOL_FULL                                         MAKE_DSHRESULTERROR (0x806D)  // Adding a new element to a pool is refused because the pool is full.
#define DS_E_UNKNOWN_EXCEPTION                                 MAKE_DSHRESULTERROR (0x806E)  // An unknown exception was caught by the call.
#define DS_E_MAX_VALUE_REACHED                                 MAKE_DSHRESULTERROR (0x806F)  // There's a maximum value for that and it has been reached.  
#define DS_E_INCONSISTENCY_DETECTED                            MAKE_DSHRESULTERROR (0x8070)  // There's an inconsistency that was detected by the object.
#define DS_E_ARRAY_TOO_SMALL                                   MAKE_DSHRESULTERROR (0x8071)  // Provided array is too small.
#define DS_E_NODE_INPUT_NOTREADY                               MAKE_DSHRESULTERROR (0x8072)  // Input to a node is not ready.
#define DS_E_INVALID_HANDLE                                    MAKE_DSHRESULTERROR (0x8073)  // Handle is invalid.
#define DS_E_DIRECTX_LOCK                                      MAKE_DSHRESULTERROR (0x8074)  // Failed to lock some DirectX memory.
#define DS_E_BUFFER_REQUEST_TOO_LATE                           MAKE_DSHRESULTERROR (0x8075)  // The requested buffer could not be processed because it's too late.
#define DS_E_BUFFER_REQUEST_PARTIAL_TOO_LATE                   MAKE_DSHRESULTERROR (0x8076)  // The requested buffer could only be processed partially. The rest was too late.
#define DS_E_SURFACE_FORMAT_MISMATCH                           MAKE_DSHRESULTERROR (0x8077)  // The operation's in/out surface format combination is not supported.
#define DS_E_READ_ACCESS_ONLY                                  MAKE_DSHRESULTERROR (0x8078)  // The alias access type is read-only and therefore, can't be used in write access.
#define DS_E_NOT_AVAILABLE                                     MAKE_DSHRESULTERROR (0x8079)  // The requested data are not available.
#define DS_E_SYNCHRO_ACCESS_NOT_ENABLED                        MAKE_DSHRESULTERROR (0x807A)  // The AVContent synchronization access is disabled. Occur when a module tries to access an AVContent that has been recycled and returned to the free pool.
#define DS_E_HD_BOB_NOT_PRESENT                                MAKE_DSHRESULTERROR (0x807B)  // HD Resolutions are not supported since there's no HD BOB present in the system.
#define DS_E_AUDIO_MIXER_NOT_PRESENT                           MAKE_DSHRESULTERROR (0x807C)  // An error occured during the initialization of the inline audioMixer engine. 
#define DS_E_BUFFER_ENGINE_NOT_PRESENT                         MAKE_DSHRESULTERROR (0x807D)  // An error occured during the initialization of the host buffer engine. 
#define DS_E_SLOW_MOTION_NOT_PRESENT                           MAKE_DSHRESULTERROR (0x807E)  // An error occured during the initialization of the slow motion module. 
#define DS_E_AUDIO_SAMPLES_ENGINE_NOT_PRESENT                  MAKE_DSHRESULTERROR (0x807F)  // An error occured during the initialization of the audio samples engine. 
#define DS_E_FLEX3D_DEVICE_NOT_PRESENT                         MAKE_DSHRESULTERROR (0x8080)  // An error occured during the initialization of the 3D Device. 
#define DS_E_COMPOSITOR_NOT_PRESENT                            MAKE_DSHRESULTERROR (0x8081)  // An error occured during the initialization of the compositor. 
#define DS_E_INPUT_OUTPUT_NOT_PRESENT                          MAKE_DSHRESULTERROR (0x8082)  // An error occured during the initialization of the input/output module. 
#define DS_E_HARDWARE_INFO_NOT_PRESENT                         MAKE_DSHRESULTERROR (0x8083)  // An error occured during the initialization of the hardware info module. 
#define DS_E_KERNEL_DRIVER_NOT_PRESENT                         MAKE_DSHRESULTERROR (0x8084)  // One of the required kernel driver (.sys) is not properly installed. 
#define DS_E_INVALID_PROFILE                                   MAKE_DSHRESULTERROR (0x8085)  // An invalid profile type was selected.
#define DS_E_INVALID_MONITOR                                   MAKE_DSHRESULTERROR (0x8086)  // Invalid monitor combination detected (live window).
#define DS_E_PCI_OPTIMIZER_NOT_PRESENT                         MAKE_DSHRESULTERROR (0x8087)  // An error occured during the initialization of the PCI optimizer module.
#define DS_E_NOT_ENOUGH_INPUT_SURFACES                         MAKE_DSHRESULTERROR (0x8088)  // Not enough surface were send to the effect
#define DS_E_NOT_ENOUGH_INPUT_CONTAINER                        MAKE_DSHRESULTERROR (0x8089)  // Not enough container were send to the effect
#define DS_E_TOO_MANY_INPUT_CONTAINER                          MAKE_DSHRESULTERROR (0x808A)  // Too many container were send to the effect
#define DS_E_HW_RENDERER_CREATION_FAILED                       MAKE_DSHRESULTERROR (0x808B)  // The creation of the hardware implementation of the effect failed.
#define DS_E_OUTSIDE_VISIBLE_AREA                              MAKE_DSHRESULTERROR (0x808C)  // Failed because a window is outside the visible area.
#define DS_E_FILE_NOT_FOUND                                    MAKE_DSHRESULTERROR (0x808D)  // Failed because a file was not found.
#define DS_E_NO_COMPLETION_EVENTS                              MAKE_DSHRESULTERROR (0x808E)  // Error in main DirectShow scheduler because no completion events are available for the scheduler.
#define DS_E_NO_EXECUTION_EVENTS                               MAKE_DSHRESULTERROR (0x808F)  // Error in main DirectShow scheduler because no execution events are available for the scheduler.
#define DS_E_NO_MEMORY_AVAILABILITY_EVENTS                     MAKE_DSHRESULTERROR (0x8090)  // Error in main DirectShow scheduler because no memory availability events are available for the scheduler.

#define DS_E_UNSUPPORTED_GENLOCK_WIDTH_HEIGHT                  MAKE_DSHRESULTERROR (0x8091)  // The requested genlock width and/or height is not supported.
#define DS_E_UNSUPPORTED_INPUT_WIDTH_HEIGHT                    MAKE_DSHRESULTERROR (0x8092)  // The requested input width and/or height is not supported.
#define DS_E_UNSUPPORTED_INPUT1_WIDTH_HEIGHT                   MAKE_DSHRESULTERROR (0x8093)  // The requested input width and/or height is not supported.
#define DS_E_UNSUPPORTED_INPUT2_WIDTH_HEIGHT                   MAKE_DSHRESULTERROR (0x8094)  // The requested input width and/or height is not supported.
#define DS_E_UNSUPPORTED_OUTPUT_WIDTH_HEIGHT                   MAKE_DSHRESULTERROR (0x8095)  // The requested output width and/or height is not supported.
#define DS_E_UNSUPPORTED_OUTPUT1_WIDTH_HEIGHT                  MAKE_DSHRESULTERROR (0x8096)  // The requested output width and/or height is not supported.
#define DS_E_UNSUPPORTED_OUTPUT2_WIDTH_HEIGHT                  MAKE_DSHRESULTERROR (0x8097)  // The requested output width and/or height is not supported.
#define DS_E_UNSUPPORTED_GENLOCK_FRAMERATE_SCANMODE            MAKE_DSHRESULTERROR (0x8098)  // The requested genlock frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_INPUT_FRAMERATE_SCANMODE              MAKE_DSHRESULTERROR (0x8099)  // The requested input frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_INPUT1_FRAMERATE_SCANMODE             MAKE_DSHRESULTERROR (0x809A)  // The requested input frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_INPUT2_FRAMERATE_SCANMODE             MAKE_DSHRESULTERROR (0x809B)  // The requested input frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_OUTPUT_FRAMERATE_SCANMODE             MAKE_DSHRESULTERROR (0x809C)  // The requested output frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_OUTPUT1_FRAMERATE_SCANMODE            MAKE_DSHRESULTERROR (0x809D)  // The requested output frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_OUTPUT2_FRAMERATE_SCANMODE            MAKE_DSHRESULTERROR (0x809E)  // The requested output frame rate and/or scan mode is not supported.
#define DS_E_UNSUPPORTED_GENLOCK_ASPECT_RATIO                  MAKE_DSHRESULTERROR (0x809F)  // The requested genlock aspect ratio is not supported.
#define DS_E_UNSUPPORTED_INPUT_ASPECT_RATIO                    MAKE_DSHRESULTERROR (0x80A0)  // The requested input aspect ratio is not supported.
#define DS_E_UNSUPPORTED_INPUT1_ASPECT_RATIO                   MAKE_DSHRESULTERROR (0x80A1)  // The requested input aspect ratio is not supported.
#define DS_E_UNSUPPORTED_INPUT2_ASPECT_RATIO                   MAKE_DSHRESULTERROR (0x80A2)  // The requested input aspect ratio is not supported.
#define DS_E_UNSUPPORTED_OUTPUT_ASPECT_RATIO                   MAKE_DSHRESULTERROR (0x80A3)  // The requested output aspect ratio is not supported.
#define DS_E_UNSUPPORTED_OUTPUT1_ASPECT_RATIO                  MAKE_DSHRESULTERROR (0x80A4)  // The requested output aspect ratio is not supported.
#define DS_E_UNSUPPORTED_OUTPUT2_ASPECT_RATIO                  MAKE_DSHRESULTERROR (0x80A5)  // The requested output aspect ratio is not supported.

#define DS_E_UIF_VIDEO_CAPTURE_START_FAILED                    MAKE_DSHRESULTERROR (0x80A6)  // The UIF video capture returned an error for the start.
#define DS_E_UIF_AUDIO_CAPTURE_START_FAILED                    MAKE_DSHRESULTERROR (0x80A7)  // The UIF audio capture returned an error for the start.
#define DS_E_UIF_VIDEO_TIMESTAMP_ZERO                          MAKE_DSHRESULTERROR (0x80A8)  // The UIF video capture buffer returned a buffer with timestamp 0.
#define DS_E_UIF_AUDIO_TIMESTAMP_ZERO                          MAKE_DSHRESULTERROR (0x80A9)  // The UIF audio capture returned a buffer with timestamp 0.
#define DS_E_UIF_OUT_OF_HOSTBUFFERS                            MAKE_DSHRESULTERROR (0x80AA)  // The UIF ran out of host buffers to copy the onboard buffers.

#define DS_E_INVALID_PROCESSING_JOB_STATE                      MAKE_DSHRESULTERROR (0x80AB)  // Scheduler has detected an invalid processing job state.
#define DS_E_PROCESSING_JOB_NOT_READY                          MAKE_DSHRESULTERROR (0x80AC)  // Scheduler has a problem with a job not ready.
#define DS_E_PROCESSING_JOB_NOT_PREPARED_FOR_EXECUTION         MAKE_DSHRESULTERROR (0x80AD)  // Scheduler has a problem with a job not prepared for execution.
#define DS_E_PROCESSING_JOB_NOT_EXECUTING                      MAKE_DSHRESULTERROR (0x80AE)  // Scheduler has a problem with a job not executing.
#define DS_E_PROCESSING_JOB_NOT_COMPLETED                      MAKE_DSHRESULTERROR (0x80AF)  // Scheduler has a problem with a job not completed.
#define DS_E_PROCESSING_JOB_EXECUTING                          MAKE_DSHRESULTERROR (0x80B0)  // Scheduler has a problem with a job executing.
#define DS_E_PROCESSING_JOB_COMPLETED                          MAKE_DSHRESULTERROR (0x80B1)  // Scheduler has a problem with a job completed.
#define DS_E_PROCESSING_JOB_REMAINING                          MAKE_DSHRESULTERROR (0x80B2)  // Scheduler has a problem with some remaining jobs.
#define DS_E_GRAPH_HAS_NOT_BEEN_DISASSEMBLED                   MAKE_DSHRESULTERROR (0x80B3)  // Graph has not been disassembled.
#define DS_E_FILTER_NOT_IN_GRAPH                               MAKE_DSHRESULTERROR (0x80B4)  // Graph has probably already been disassembled.
#define DS_E_PROCESSING_NOT_SUPPORTED_IN_CONFIGURATION         MAKE_DSHRESULTERROR (0x80B5)  // Processing in not supported in the current configuration
#define DS_E_INVALID_BITRATE                                   MAKE_DSHRESULTERROR (0x80B6)  // Invalid bit rate in codec for compression
#define DS_E_INVALID_CUTLIST                                   MAKE_DSHRESULTERROR (0x80B7)  // Cutlist parameters are invalid for the given file
#define DS_E_GFX_FILE_IS_REQUIRED                              MAKE_DSHRESULTERROR (0x80B8)  // There's no GFX file provided
#define DS_E_DV_AUDIO_ABSTRACTOR_NOT_PRESENT                   MAKE_DSHRESULTERROR (0x80B9)  // An error occured during the initialization of the DV Audio Abstractor module. 
#define DS_E_MEMORY_REGIONS_ALLOCTED_TOO_MANY                  MAKE_DSHRESULTERROR (0x80Ba)  // Too many memory regions allocated in mvdcache.dll , it's abnormal .

#define DS_E_INVALID_GOP                                       MAKE_DSHRESULTERROR (0x80BB)  // General error reported by the codec for a GOP
#define DS_E_OPEN_GOP_NOT_SUPPORTED                            MAKE_DSHRESULTERROR (0x80BC)  // This type of GOP is not yet supported.
#define DS_E_GOP_TOO_BIG                                       MAKE_DSHRESULTERROR (0x80BD)  // More than 30 frames in a GOP. This is a limit inside the codec. It can be change to something bigger.

#define DS_E_CODEC_INFO_NOT_AVAILABLE                          MAKE_DSHRESULTERROR (0x80BE)  // The codec info is not avilable. No validation of file format can be done.
#define DS_E_VIDEO_DECODER_NOT_AVAILABLE                       MAKE_DSHRESULTERROR (0x80BF)  // The required video decoder is not available. This error code is returned for video only file. This file can not be played.
#define DS_E_AUDIO_DECODER_NOT_AVAILABLE                       MAKE_DSHRESULTERROR (0x80C0)  // The required audio decoder is not available. This error code is returned for audio only file. This file can not be played.
#define DS_E_AVI_NOT_SUPPORTED                                 MAKE_DSHRESULTERROR (0x80C1)  // The interleaved AV file info is known to matrox file reader but the Video and audio formats are not supported. This warning code is only returned for interleaved AV file.  This file can not be played.
#define DS_E_FILE_FORMAT_NOT_SUPPORTED                         MAKE_DSHRESULTERROR (0x80C2)  // The file info is unknown to matrox file reader (eg: a txt file); the file information could not be retried on this file. The file info structure is invalid and should not be used.  This file can not be played.
#define DS_E_ERES_NOT_SET                                      MAKE_DSHRESULTERROR (0x80C3)  // The requested check can not be completed because there is no valid resolution setting currently set.
#define DS_E_INCOMPATIBLE_WIDTH                                MAKE_DSHRESULTERROR (0x80C4)  // The width of the file and the editing resolution is incompatible. The file can not be streamed.
#define DS_E_INCOMPATIBLE_HEIGHT                               MAKE_DSHRESULTERROR (0x80C5)  // The height of the file and the editing resolution is incompatible. The file can not be streamed.
#define DS_E_INCOMPATIBLE_COMPONENT_BIT_COUNT                  MAKE_DSHRESULTERROR (0x80C6)  // The component bit count of the file and the editing resolution is incompatible. The file can not be streamed.
#define DS_E_INCOMPATIBLE_FRAME_RATE                           MAKE_DSHRESULTERROR (0x80C7)  // The frame rate of the file and the editing resolution is incompatible. The file can not be streamed.

#define DS_E_UNSUPPORTED_DEPTH                                 MAKE_DSHRESULTERROR (0x80C8)  // The value for the depth of the surface description is not supported
#define DS_E_UNSUPPORTED_SURFACE_TYPE                          MAKE_DSHRESULTERROR (0x80C9)  // The value for the surface type of the surface description is not supported
#define DS_E_UNSUPPORTED_MEMORY_LOCATION                       MAKE_DSHRESULTERROR (0x80CA)  // The value for the memory location of the surface description is not supported
#define DS_E_INVALID_POLARITY                                  MAKE_DSHRESULTERROR (0x80CB)  // The value for the polarity of the surface description is not valid for the standard used
#define DS_E_UNSUPPORTED_IMAGE_ORIGIN                          MAKE_DSHRESULTERROR (0x80CC)  // The value for the image origin of the surface description is not supported
#define DS_E_SHAPED_VIDEO_NOT_SUPPORTED                        MAKE_DSHRESULTERROR (0x80CD)  // Shaped video is not supported in this module
#define DS_E_UNSUPPORTED_SURFACE_FORMAT                        MAKE_DSHRESULTERROR (0x80CE)  // The value for the surface format of the surface description is not supported
#define DS_E_UNSUPPORTED_COMPONENT_BIT_COUNT                   MAKE_DSHRESULTERROR (0x80CF)  // The value for the component bit count of the surface description is not supported
#define DS_E_SURFACE_WIDTH_MISMATCH                            MAKE_DSHRESULTERROR (0x80D0)  // The width of the input and output surface are not the same
#define DS_E_INVALID_PITCH                                     MAKE_DSHRESULTERROR (0x80D1)  // The pitch does not match the minimum requirement
#define DS_E_SOURCE_BUFFER_TOO_SMALL                           MAKE_DSHRESULTERROR (0x80D2)  // The source buffer is smaller than what it should be
#define DS_E_POLARITY_TOP_LINE_MISMATCH                        MAKE_DSHRESULTERROR (0x80D3)  // The top line value of the surface description does not match the polarity
#define DS_E_INVALID_COMPRESSED_DATA                           MAKE_DSHRESULTERROR (0x80D4)  // The compressed data received if not valid
#define DS_E_SURFACE_HEIGHT_MISMATCH                           MAKE_DSHRESULTERROR (0x80D5)  // The height of the input and output surface are not the same

#define DS_E_WRONG_EDIT_MODE                                   MAKE_DSHRESULTERROR (0x80D6)  // The device control is in the wrong edit mode.
#define DS_E_EDIT_SET_ACTIVE                                   MAKE_DSHRESULTERROR (0x80D7)  // The edit was already set active.
#define DS_E_DEVICE_NOT_CUED                                   MAKE_DSHRESULTERROR (0x80D8)  // The device was not cued.

#define DS_E_LOST_VIDEO_INPUT                                  MAKE_DSHRESULTERROR (0x80D9)  // The video input is lost
#define DS_E_LOST_EMBEDDED_AUDIO_INPUT                         MAKE_DSHRESULTERROR (0x80DA)  // The embedded audio input is lost
#define DS_E_LOST_GENLOCK                                      MAKE_DSHRESULTERROR (0x80DB)  // The genlock was lost.
#define DS_E_GENLOCK_CHANGED                                   MAKE_DSHRESULTERROR (0x80DC)  // The genlock has changed.

#define DS_E_CORRUPTION_DETECTED                               MAKE_DSHRESULTERROR (0x80DD)  // The buffer that was decompressed was corrupted.  The video data outputted may be incomplete.

#define DS_E_XLINK_DISCONNECTED									      MAKE_DSHRESULTERROR (0x80DE)  // The xlink has been disconnected.	

#define DS_E_GOP_POSITION_INVALID                              MAKE_DSHRESULTERROR (0x80DF)  // The IBP codec was asked to decode at a position greater than the number of frame in the GOP.
#define DS_E_INPOINT_NOTSET                                    MAKE_DSHRESULTERROR (0x80E0)  // An inpoint timecode was not set for the device control.
#define DS_E_OUTPOINT_NOTSET                                   MAKE_DSHRESULTERROR (0x80E1)  // An outpoint timecode was not set for the device control.

#define DS_E_VIDEO_INPUT_ABSENT                                MAKE_DSHRESULTERROR (0x80E2)  // The video input is not present.
#define DS_E_INPUT_RESOLUTION_MISMATCH                         MAKE_DSHRESULTERROR (0x80E3)  // The input resolution mismatches.

#define DS_E_FILE_ALREADY_OPENED                               MAKE_DSHRESULTERROR (0x80E4)  // This file has already been subject to a OpenFile
#define DS_E_INVALID_CAPTURE_TYPE                              MAKE_DSHRESULTERROR (0x80E5)  // This capture type is not supported
#define DS_E_VIDEO_ENCODER_NOT_AVAILABLE                       MAKE_DSHRESULTERROR (0x80E6)  // We do not have a codec for the requested capture type
#define DS_E_UNSUPPORTED_ENCODER_RESOLUTION                    MAKE_DSHRESULTERROR (0x80E7)  // The specified resolution is not supported by the video codec
#define DS_E_FILENAME_NOT_SPECIFIED                            MAKE_DSHRESULTERROR (0x80E8)  // We were asked to open a file but no filename has been supplied
#define DS_E_INVALID_START_POSITION                            MAKE_DSHRESULTERROR (0x80E9)  // The requested start position inside the file is invalid
#define DS_E_INVALID_FILE_OPEN_MODE                            MAKE_DSHRESULTERROR (0x80EA)  // This file open mode is not supported
#define DS_E_TDIR_NOT_SUPPORTED                                MAKE_DSHRESULTERROR (0x80EB)  // This feature is not supported
#define DS_E_CUSTOMIO_NOT_SUPPORTED                            MAKE_DSHRESULTERROR (0x80EC)  // This feature is not supported
#define DS_E_STRUCTURE_SIZE_MISMATCH                           MAKE_DSHRESULTERROR (0x80ED)  // The input structure is of an unexpected size
#define DS_E_UNSUPPORTED_SCANMODE                              MAKE_DSHRESULTERROR (0x80EE)  // The scanmode is not supported
#define DS_E_UNSUPPORTED_FRAMERATE                             MAKE_DSHRESULTERROR (0x80EF)  // The framerate is not supported
#define DS_E_UNSUPPORTED_ASPECT_RATIO                          MAKE_DSHRESULTERROR (0x80F0)  // The aspect ratio is not supported
#define DS_E_INVALID_PIPELINE_SIZE                             MAKE_DSHRESULTERROR (0x80F1)  // The pipeline size is not supported
#define DS_E_FILE_NOT_OPENED                                   MAKE_DSHRESULTERROR (0x80F2)  // No OpenFile was performed
#define DS_E_NOT_ENOUGH_INPUT_SAMPLES                          MAKE_DSHRESULTERROR (0x80F3)  // Not enough input audio samples

#define DS_E_GRAPH_MUST_BE_STOPPED                             MAKE_DSHRESULTERROR (0x80F4)  // Tried to add or remove window when graph was not stopped

#define DS_E_INCOMPATIBLE_SCAN_MODE                            MAKE_DSHRESULTERROR (0x80F5)  // The scan mode of the file and the editing resolution is incompatible. The file can not be streamed.
#define DS_E_INVALID_AVIODML_INDEX                             MAKE_DSHRESULTERROR (0x80F6)  // The index of the AVI Open DML file is invalid
#define DS_E_PARAMETER_ARRAY_TOO_BIG                           MAKE_DSHRESULTERROR (0x80F7)  // Parameter array too big
#define DS_E_UNSUPPORTED_AUDIO_CHANNEL_TYPE                    MAKE_DSHRESULTERROR (0x80F8)  // The audio channel type (number of tracks) is not supported

#define DS_E_MUST_GET_COPY_SIZE                                MAKE_DSHRESULTERROR (0x80F9)  // GetSizeOfBufferToAllocateForHostCopy must be called to set the size
#define DS_E_CANNOT_COPY_WHEN_RUNNING                          MAKE_DSHRESULTERROR (0x80FA)  // CopyLastOutputRGBAToHost cannot be called while the graph is running
#define DS_E_INVALID_THREAD_PRIORITY                           MAKE_DSHRESULTERROR (0x80FB)  // The specified thread priority is not within the expected range
#define DS_E_WAV_FILE_SIZE_LIMIT_REACHED                       MAKE_DSHRESULTERROR (0x80FC)  // The .wav file size limit (4 GB) as been reached, cannot write more data 
#define DS_E_DEVICE_HAS_BEEN_OPENED                            MAKE_DSHRESULTERROR (0x80FD)  // The VCR device has been opened. 
#define DS_E_UNSUPPORTED_VBI_RESOLUTION                        MAKE_DSHRESULTERROR (0x80FE)  // The specified VBI resolution is not supported. 
#define DS_E_DROPPED_SAMPLE                                    MAKE_DSHRESULTERROR (0x80FF)  // A sample (field or frame, audio or video) has been dropped. 
#define DS_E_APPEND_SECTOR_SIZE_DIFFERENT                      MAKE_DSHRESULTERROR (0x8100)  // On appending , the file has been copied to a drive that has different sector size  to the original drive.
#define DS_E_APPEND_FORMAT_NOT_COMPATIBLE                      MAKE_DSHRESULTERROR (0x8101)  // On appending ,  foramt of new data is not compatible of old data , muxer is not set properly. 
#define DS_E_TRUNCATE_POSITION_INVALID                         MAKE_DSHRESULTERROR (0x8102)  // On truncation , the required truncation position is not in the file. 
#define DS_E_PLAYLIST_NOT_SET                                  MAKE_DSHRESULTERROR (0x8103)  // Playlist has not been set.
#define DS_E_PIPELINE_SIZE_NOT_SET                             MAKE_DSHRESULTERROR (0x8105)  // Pipeline size has not been set.
#define DS_E_INVALID_PLAYLIST_TYPE                             MAKE_DSHRESULTERROR (0x8106)  // Playlist type is invalid.
#define DS_E_POSITION_PARAMETER_NOT_SET                        MAKE_DSHRESULTERROR (0x8107)  // Position parameter is NULL.
#define DS_E_VIDEO_SURFACES_PARAMETER_NOT_SET                  MAKE_DSHRESULTERROR (0x8108)  // Video surfaces parameter is NULL.
#define DS_E_POOL_ALLOCATION_FAILED                            MAKE_DSHRESULTERROR (0x8109)  // Unable to allocate a specific pool.
#define DS_E_FAILED_TO_RETRIEVE_POSITION_FROM_DISK             MAKE_DSHRESULTERROR (0x810A)  // Specified position was not expected to be read from disk.
#define DS_E_UNSUPPORTED_COMPRESSION_TYPE                      MAKE_DSHRESULTERROR (0x810B)  // Specified compression format is not supported.
#define DS_E_CODEC_OPERATION_FAILED                            MAKE_DSHRESULTERROR (0x810C)  // A codec operation failed with an unknown error.
#define DS_E_AUDIO_SAMPLES_PARAMETER_NOT_SET                   MAKE_DSHRESULTERROR (0x810D)  // Audio samples parameter is NULL.
#define DS_E_TOO_MANY_INPUT_SAMPLES                            MAKE_DSHRESULTERROR (0x810E)  // Caller is expecting too many audio samples in array parameter.
#define DS_E_INPUT_SAMPLES_ARRAY_SIZE_MISMATCH                 MAKE_DSHRESULTERROR (0x810F)  // Caller is changing number of expected samples array size between calls.
#define DS_E_AUDIO_DUPLICATE_FAILED                            MAKE_DSHRESULTERROR (0x8110)  // Audio duplication failed with an unknown error.
#define DS_E_UNABLE_TO_RETRIEVE_AUDIO_SAMPLES                  MAKE_DSHRESULTERROR (0x8111)  // Unable to retrieve audio samples probably due to pipeline size being insufficient.
#define DS_E_TIMEOUT_WAITING_FOR_DISK                          MAKE_DSHRESULTERROR (0x8112)  // Timeout while waiting for a file IO of a specific position.
#define DS_E_CANNOT_CHANGE_DEVICE_WHEN_RUNNING                 MAKE_DSHRESULTERROR (0x8113)  // The graph must be stopped to change the input device. 

#define DS_E_ELEMENT_IN_USE                                    MAKE_DSHRESULTERROR (0x8114)  // Play list element is already in use 
#define DS_E_ELEMENT_NOT_LOCKED                                MAKE_DSHRESULTERROR (0x8115)  // Play list element was not locked
#define DS_E_ELEMENT_LOST                                      MAKE_DSHRESULTERROR (0x8116)  // Play list element was not moved in the play list and is NOT in it anymore
#define DS_E_INVALID_ELEMENT_TYPE                              MAKE_DSHRESULTERROR (0x8117)  // Play list element is not the same type as the play list
#define DS_E_INVALID_PATH_SIZE                                 MAKE_DSHRESULTERROR (0x8118)  // Filename size is too big.
#define DS_E_ELEMENT_ALREADY_IN_PLAYLIST                       MAKE_DSHRESULTERROR (0x8119)  // Play list element is already in the list.
#define DS_E_ELEMENT_FOUND                                     MAKE_DSHRESULTERROR (0x8120)  // A play list element was found at the specified location.
#define DS_E_UNSUPPORTED_AUDIO_DATA_TYPE                       MAKE_DSHRESULTERROR (0x8121)  // The audio data type is not supported on the selected profile
#define DS_E_HARDDISK_ACCESS_FAILURE                           MAKE_DSHRESULTERROR (0x8122)  // Harddisk access failed.
#define DS_E_HARDDISK_TOO_SLOW                                 MAKE_DSHRESULTERROR (0x8123)  // Harddisk is too slow.

#define DS_E_PIPES_ALREADY_LINKED                              MAKE_DSHRESULTERROR (0x8124)  // The pipes are already connected or already have a trig link.
#define DS_E_PIPES_NOT_CONNECTED                               MAKE_DSHRESULTERROR (0x8125)  // The pipes are not connected to one another.
#define DS_E_TOO_MANY_UPSTREAM_PIPES                           MAKE_DSHRESULTERROR (0x8126)  // The maximum number of upstream pipes has been reached.
#define DS_E_TPIA_SMALLER_THAN_TPOA                            MAKE_DSHRESULTERROR (0x8127)  // The target advance at a pipe's input cannot be smaller than the target advance at its output.
#define DS_E_MAXPIPEDEPTH_SMALLER_THAN_ONE                     MAKE_DSHRESULTERROR (0x8128)  // The maximum pipe depth must be greater than 0.
#define DS_E_ENDOFPIPE_WAS_RECEIVED                            MAKE_DSHRESULTERROR (0x8129)  // The EndOfPipe pipe element was received, preventing the requested operation from being executed.
#define DS_E_ENDOFPIPE_WAS_PROCESSED                           MAKE_DSHRESULTERROR (0x812A)  // The EndOfPipe pipe element was processed, preventing the requested operation from being executed.
#define DS_E_INCOMPATIBLE_PIPE_ELEMENT                         MAKE_DSHRESULTERROR (0x812B)  // The pipe element is incompatible with the pipe.
#define DS_E_CANCELLATION_TOO_EARLY                            MAKE_DSHRESULTERROR (0x812C)  // The pipe has not yet reached the position specified for the cancellation (TrigDur or ShuttlingSpeed changed too soon).
#define DS_E_PROBLEM_TALKING_TO_UPSTREAM_PIPE                  MAKE_DSHRESULTERROR (0x812D)  // The pipe had problem talking to an upstream pipe. See the upstream's pipe error.
#define DS_E_PROBLEM_TALKING_TO_DOWNSTREAM_PIPE                MAKE_DSHRESULTERROR (0x812E)  // The pipe had problem talking to a downstream pipe. See the downstream's pipe error.
#define DS_E_INVALID_INSTANTFRAMERATE                          MAKE_DSHRESULTERROR (0x812F)  // Some fields of an InstantFrameRate object are invalid.
#define DS_E_PROBLEM_IN_SUB_PROCESSING                         MAKE_DSHRESULTERROR (0x8130)  // The pipe had problem in the sub-processing of a Trig, a ChangeTrigDur, or an EndOfPipe.
#define DS_E_BAD_PIPE_CONFIGURATION                            MAKE_DSHRESULTERROR (0x8131)  // The pipe is badly configured.
#define DS_E_BAD_PIPE_STATE                                    MAKE_DSHRESULTERROR (0x8132)  // The pipe is in a bad state.
#define DS_E_FAILED_TO_PROCESS_PIPE_ELEMENT                    MAKE_DSHRESULTERROR (0x8133)  // A pipe was not able to properly process a pipe element.
#define DS_E_ALREADY_PROCESSING_PIPE_ELEMENT                   MAKE_DSHRESULTERROR (0x8134)  // A pipe was already processing a pipe element when it was asked to work on another one.
#define DS_E_PROCESSBEST_PIPE_DOESNT_PROPAGATE_TRIGS           MAKE_DSHRESULTERROR (0x8135)  // The ProcessBest processing pipe doesn't support to forward trigs to other pipes.

#define DS_E_TRIM_LENGTH_LARGER_THAN_MEDIA_LENGTH              MAKE_DSHRESULTERROR (0x8136)  // The playlist element's trim length exceeds the media's length
#define DS_E_TRIM_OUTSIDE_MEDIA_RANGE                          MAKE_DSHRESULTERROR (0x8137)  // The playlist element's trim infos are out of the media's boundaries
#define DS_E_TRIM_LENGTH_NOT_SUPPORTED                         MAKE_DSHRESULTERROR (0x8138)  // The playlist element's trim length is invalid (ex: zero, or audio and not 1.0)
#define DS_E_DURATION_LENGTH_NOT_SUPPORTED                     MAKE_DSHRESULTERROR (0x8139)  // The playlist element's duration length is invalid (ex: zero)
#define DS_E_REQUEST_FLUSHED                                   MAKE_DSHRESULTERROR (0x813A)  // Operation on request failed because request is already completed and flushed.
#define DS_E_FILEHANDLE_ALREADY_SET                            MAKE_DSHRESULTERROR (0x813B)  // SetFileHandle on a file handle object failed because the file handle has been set.
#define DS_E_COUNT_OF_LIST_BUSY_BLOCKS_NON_0                   MAKE_DSHRESULTERROR (0x813C)  // In mvdcache , count of list busy blocks is not 0 . This is wrong when FlushCachedInformation is called on blockallocator.
#define DS_E_OUT_OF_ORDER                                      MAKE_DSHRESULTERROR (0x813D)  // This is when a strict order is not respected.
#define DS_E_FILEACCESS_OPENFILE_FAILED                        MAKE_DSHRESULTERROR (0x813E)  // File open failed in CDsdFileAccess module.
#define DS_E_FILEACCESS_GET_FILE_SIZE_FAILED                   MAKE_DSHRESULTERROR (0x813F)  // GetFileSize failed in CDsdFileAccess module.

#define DS_E_MPS_GOP_MEMORY_NOT_ASSIGNED                       MAKE_DSHRESULTERROR (0x8140)  // the GOP does not contain a memory buffer yet
#define DS_E_MPS_FRAME_NOT_FOUND                               MAKE_DSHRESULTERROR (0x8141)  // unable to find the specified frame
#define DS_E_MPS_GOP_CANCELLED                                 MAKE_DSHRESULTERROR (0x8142)  // GOP was cancelled
#define DS_E_MPS_NO_MEMORY_PROVIDER                            MAKE_DSHRESULTERROR (0x8143)  // no memory provider was specified
#define DS_E_MPS_WIDTH_NOT_SUPPORTED                           MAKE_DSHRESULTERROR (0x8144)  // specified width is not supported
#define DS_E_MPS_HEIGHT_NOT_SUPPORTED                          MAKE_DSHRESULTERROR (0x8145)  // specified height is not supported
#define DS_E_MPS_ASPECT_RATIO_NOT_SUPPORTED                    MAKE_DSHRESULTERROR (0x8146)  // specified aspect ratio is not supported
#define DS_E_MPS_FRAME_RATE_NOT_SUPPORTED                      MAKE_DSHRESULTERROR (0x8147)  // specified frame rate is not supported
#define DS_E_MPS_SCAN_MODE_NOT_SUPPORTED                       MAKE_DSHRESULTERROR (0x8148)  // specified scan mode is not supported
#define DS_E_MPS_BIT_COUNT_NOT_SUPPORTED                       MAKE_DSHRESULTERROR (0x8149)  // specified number of bits per pixel is not supported
#define DS_E_MPS_LEVEL_DOES_NOT_MATCH_RESOLUTION               MAKE_DSHRESULTERROR (0x814a)  // specified mpeg level is not supported in the specified resolution
#define DS_E_MPS_MQUANT_VALUE_NOT_ACCEPTED                     MAKE_DSHRESULTERROR (0x814b)  // specified mquant value is not supported
#define DS_E_MPS_INVALID_GOP_SIZE                              MAKE_DSHRESULTERROR (0x814c)  // specified GOP size is not supported
#define DS_E_MPS_INVALID_NUMBER_OF_B_FRAMES                    MAKE_DSHRESULTERROR (0x814d)  // specified number of B frames is not supported
#define DS_E_MPS_TIMECODE_EXCEEDS_MAXIMUM_VALUE                MAKE_DSHRESULTERROR (0x814e)  // specified timecode is not supported
#define DS_E_MPS_ENCODER_INITIALIZATION_FAILED                 MAKE_DSHRESULTERROR (0x814f)  // mvMpegStream encoder was unable to complete its initialization
#define DS_E_MPS_QFACTOR_NOT_SUPPORTED                         MAKE_DSHRESULTERROR (0x8150)  // specified QFactor is not supported
#define DS_E_MPS_ENCODER_CLOSE_FAILED                          MAKE_DSHRESULTERROR (0x8151)  // mvMpegStream encoder was unable to complete its close
#define DS_E_MPS_TIMEOUT_WAITING_FOR_ENCODER                   MAKE_DSHRESULTERROR (0x8152)  // fatal internal mvMpegStream timeout
#define DS_E_MPS_PUTFRAME_SYNCHRONOUS_ERROR                    MAKE_DSHRESULTERROR (0x8153)  // unable to encode the specified frame
#define DS_E_MPS_UNKNOWN_ENCODER_SURFACE                       MAKE_DSHRESULTERROR (0x8154)  // mvMpegStream fatal internal error, unable to retrieve compressed surface
#define DS_E_MPS_GOP_LOCKED                                    MAKE_DSHRESULTERROR (0x8155)  // the GOP is currently locked by mvMpegStream
#define DS_E_MPS_ENCODER_CLOSED                                MAKE_DSHRESULTERROR (0x8156)  // the mvMpegStream encoder is closed, this operation is not valid at this time
#define DS_E_MPS_UNABLE_TO_RETRIEVE_GOP                        MAKE_DSHRESULTERROR (0x8157)  // the mvMpegStream encoder failed to obtain a new GOP

#define DS_E_UNSUPPORTED_PRODUCTS_CONFIGURATION                MAKE_DSHRESULTERROR (0x8158)  // Two differents Matrox product can't co-exist in the same system
#define DS_E_INVALID_PRODUCT_CONFIGURATION                     MAKE_DSHRESULTERROR (0x8159)  // No valid Matrox product was found

#define DS_E_MPS_INVALID_DC_PRECISION                          MAKE_DSHRESULTERROR (0x815A)  // the mvMpegStream encoder DC precision is invalid
#define DS_E_MPS_INVALID_MOTION_SEARCH_TYPE                    MAKE_DSHRESULTERROR (0x815B)  // the mvMpegStream motion search type is invalid
#define DS_E_STATE_MISMATCH                                    MAKE_DSHRESULTERROR (0x815c)  // the device is not in a proper state. eg.instead of play state, the device is in stop state
#define DS_E_END_OF_TAPE                                       MAKE_DSHRESULTERROR (0x815d)  // the device reaches the end of the tape                  
#define DS_E_DEVICE_AUTO_STOP                                  MAKE_DSHRESULTERROR (0x815e)  // Some deck will stop itself after pausing for a while. The application needs to know this error to stop asking for buffer. 
#define DS_E_BAD_GPU_DRIVER                                    MAKE_DSHRESULTERROR (0x815f)  // the current driver for the video card will cause problems.
#define DS_E_FRAME_COUNT_LIMIT_REACHED                         MAKE_DSHRESULTERROR (0x8160)  // the specified file can't store any more frames or samples, possibly because it would overflow the allocated AVI header size

#define DS_E_INCOMPATIBLE_BITS_PER_SAMPLE                      MAKE_DSHRESULTERROR (0x8161)  // At audio sample creation, valid bits per sample is not compatible with total bits per sample
#define DS_E_DEVICE_IN_WRONG_STATE                             MAKE_DSHRESULTERROR (0x8162)  // device is not in the expected state, causes: end-of-tape, the device is stopped by the user while it's playing or device is in local mode instead of remote mode
#define DS_E_TAPE_ABSENT                                       MAKE_DSHRESULTERROR (0x8163)  // There is no tape installed in the device

#define DS_E_OUT_OF_MEMORY_DX                                  MAKE_DSHRESULTERROR (0x8164)  // Not enough memory 3D DX to perform the operation
#define DS_E_INVALID_AUDIO_CHANNEL_TYPE                        MAKE_DSHRESULTERROR (0x8165)  // Specified channel type not supported or invalid
#define DS_E_INVALID_AUDIO_SAMPLING_RATE                       MAKE_DSHRESULTERROR (0x8166)  // Specified sampling rate not supported or invalid
#define DS_E_INVALID_AUDIO_DATA_TYPE                           MAKE_DSHRESULTERROR (0x8167)  // Specified data type not supported or invalid
#define DS_E_IN_POINT_NOT_REACHED							         MAKE_DSHRESULTERROR (0x8168)  // In point couldn't be found
#define DS_E_END_OF_FILE									            MAKE_DSHRESULTERROR (0x8169)  // End of file has been reached		
#define DS_E_FILEACCESS_WRITE_FAILED					            MAKE_DSHRESULTERROR (0x816A)  // Write to file failed
#define DS_E_FILEACCESS_READ_FAILED					               MAKE_DSHRESULTERROR (0x816B)  // Reading a file failed
#define DS_E_INVALID_DONGLE_ID                                 MAKE_DSHRESULTERROR (0x816C)  // The MDUF dongle ID does not correspond to the board dongle ID.
#define DS_E_INVALID_DONGLE_DATA                               MAKE_DSHRESULTERROR (0x816D)  // The MDUF dongle data does not correspond to the board dongle data.
#define DS_E_INVALID_UPGRADE_MODEL                             MAKE_DSHRESULTERROR (0x816E)  // The requested upgrade model is not valid for the current board.
#define DS_E_INVALID_OEM_ID                                    MAKE_DSHRESULTERROR (0x816F)  // The OEM ID does is not represented in the proper range.
#define DS_E_INVALID_BOARD_PART_NUMBER                         MAKE_DSHRESULTERROR (0x8170)  // The MDIF file part number does not match the MDIF dongle data.

#define DS_E_NOT_LOCKED                                        MAKE_DSHRESULTERROR (0x8171)  // Object is not locked (and it needs to be!)
#define DS_E_ALREADY_LOCKED                                    MAKE_DSHRESULTERROR (0x8172)  // Object is already locked.

#define DS_E_FAILED_TO_OPEN_FILE                               MAKE_DSHRESULTERROR (0x8173)  // Failed to open the specified file
#define DS_E_INTERNAL_QUICKTIME_ERROR_OCCURED                  MAKE_DSHRESULTERROR (0x8174)  // An internal quicktime error occured while playing the movie
#define DS_E_FAILED_TO_GET_AUDIO_DATA                          MAKE_DSHRESULTERROR (0x8175)  // Failed to get audio data from the movie
#define DS_E_REQUIRE_QUICKTIME_7                               MAKE_DSHRESULTERROR (0x8176)  // This module/functionnality requires QuickTime 7          
#define DS_E_FAILED_TO_ADD_VIDEO_DATA                          MAKE_DSHRESULTERROR (0x8177)  // Failed to add video data to the movie file       
#define DS_E_FAILED_TO_ADD_AUDIO_DATA                          MAKE_DSHRESULTERROR (0x8178)  // Failed to add audio data to the movie file
#define DS_E_FAILED_TO_INITIALIZE_COMPRESSION_SESSION          MAKE_DSHRESULTERROR (0x8179)  // Failed to initialize a compression session
#define DS_E_FAILED_TO_COMPRESS_FRAME                          MAKE_DSHRESULTERROR (0x817A)  // Failed to compress a frame sent to the compressor
#define DS_E_OPTION_CREATION_FAILED                            MAKE_DSHRESULTERROR (0x817B)  // Failed to create an option for the compression session
#define DS_E_FAILED_TO_CREATE_PATH                             MAKE_DSHRESULTERROR (0x817C)  // An attempt to create a path failed
#define DS_E_FAILED_TO_FLATTEN_MOVIE                           MAKE_DSHRESULTERROR (0x817D)  // Failed to flatten the movie file to the specified destination
#define DS_E_THREAD_CREATION_FAILED                            MAKE_DSHRESULTERROR (0x817E)  // Failed to create a new thread
#define DS_E_AUDIO_EXTRACTION_INITIALIZATION_FAILED            MAKE_DSHRESULTERROR (0x817F)  // Failed to initialize the audio extraction module
#define DS_E_FILE_ALREADY_EXISTS                               MAKE_DSHRESULTERROR (0x8180)  // The specified file already exists

#define DS_E_REBOOT_REQUIRED                                   MAKE_DSHRESULTERROR (0x8181)  // A reboot is required for system to work well.
#define DS_E_WINDOWS_MEDIA_AUDIO_CODEC_NOT_FOUND               MAKE_DSHRESULTERROR (0x8182)  // The requested Windows Media Audio CODEC was not fond in the system.
#define DS_E_VIDEO_POSITION_CANCELLED                          MAKE_DSHRESULTERROR (0x8183)  // A video position that was expected has been cancelled (ex: disk read command cancel)
#define DS_E_AUDIO_POSITION_CANCELLED                          MAKE_DSHRESULTERROR (0x8184)  // An audio position that was expected has been cancelled (ex: disk read command cancel)
#define DS_E_UNEXPECTED_GETPOSITION                            MAKE_DSHRESULTERROR (0x8185)  // An internal GetPosition has failed (flex internal error)
#define DS_E_TIMEOUT_WAITING_FOR_FLEX_AV_CLIP                  MAKE_DSHRESULTERROR (0x8186)  // Timeout while waiting for a flex av clip to return the data
#define DS_E_TIMEOUT_WAITING_FOR_THREAD                        MAKE_DSHRESULTERROR (0x8187)  // Timeout while waiting for an internal thread to wake up
#define DS_E_OUT_OF_CACHE_MEMORY                               MAKE_DSHRESULTERROR (0x8188)  // Unable to retrieve internal memory from the cache module
#define DS_E_MPEG_VIDEO_READ_FAILED                            MAKE_DSHRESULTERROR (0x8189)  // An error occured while reading from the flex mpeg clip reader
#define DS_E_MPEG_AUDIO_READ_FAILED                            MAKE_DSHRESULTERROR (0x8190)  // An error occured while reading from the flex mpeg clip reader
#define DS_E_MPEG_SEEK_FAILED                                  MAKE_DSHRESULTERROR (0x8191)  // An error occured while seeking on the flex mpeg clip reader
#define DS_E_FAILED_TO_GET_VIDEO_DATA_IN_FILE                  MAKE_DSHRESULTERROR (0x8192)  // There is no video data in the file, reach the end of the video file
#define DS_E_FAILED_TO_GET_AUDIO_DATA_IN_FILE                  MAKE_DSHRESULTERROR (0x8193)  // There is no audio data in the file, reach the end of the audio file

#define DS_E_OUTPUT_IS_DISABLED                                MAKE_DSHRESULTERROR (0x8194)  // The output is disabled, so nothing should be sent to it.

#define DS_E_INDEXTABLES_OUT_OF_HEADER_BOUNDARY                MAKE_DSHRESULTERROR (0x8195)  // The IndexTables is out of header boundary. (ex: header size is fixed but the number of IndexTable is growing during capture)

#define DS_E_FILE_IS_NOT_READY                                 MAKE_DSHRESULTERROR (0x8196)  // The file is not ready since there is no indextable. (For XDCAM HD MXF file)

#define DS_E_MULTISAMPLE_TYPE_NOT_SUPPORTED                    MAKE_DSHRESULTERROR (0x8197)  // The multisamlpe type is not supported by the GPU
#define DS_E_MULTISAMPLE_QUALITY_NOT_SUPPORTED                 MAKE_DSHRESULTERROR (0x8198)  // The multisamlpe quality is not supported by the GPU

#define DS_E_INDEXOFFSET_BEYOND_FILE_SIZE                      MAKE_DSHRESULTERROR (0x8199)  // The Index offset is out of file.

#define DS_E_WRONG_DLL_VERSION                                 MAKE_DSHRESULTERROR (0x819A)  // The version of the DLL is not the correct one.

#define DS_E_INPUT_AVCONTENT_IN_ERROR                          MAKE_DSHRESULTERROR (0x819B)  // The input AVContent is in error

#define DS_E_NO_VIDEO_AND_NO_AUDIO_IN_FILE                     MAKE_DSHRESULTERROR (0x819C)  // File without video and audio


#define DS_E_AVCINTRA_HEADER_INVALID                            MAKE_DSHRESULTERROR (0x819D)  // invalid AVCIntra header
#define DS_E_BASEDECODER_INIT_FAILED                            MAKE_DSHRESULTERROR (0x819E)  // the init of the basedecoder failed.
#define DS_E_BASEENCODER_INIT_FAILED                            MAKE_DSHRESULTERROR (0x819F)  // the init of the baseencoder failed.
#define DS_E_LIB_INIT_FAILED                                    MAKE_DSHRESULTERROR (0x81A0)  // the init of the lib failed.
#define DS_E_SSE2_INIT_FAILED                                   MAKE_DSHRESULTERROR (0x81A1)  //  failed to force to SSE2
#define DS_E_AVCINTRA_CODEC_CREATION_FAILED                     MAKE_DSHRESULTERROR (0x81A2)  // the creation of the AVCIntra codec failed.
#define DS_E_960x720_NOT_SUPPORTED                              MAKE_DSHRESULTERROR (0x81A3)  // the native 960x720 is not supported.
#define DS_E_OUTPUT_RESOLUTION_NOT_SUPPORTED                    MAKE_DSHRESULTERROR (0x81A4)  // the output resolution is not supported.
#define DS_E_HD_TO_SD_NOT_SUPPORTED                             MAKE_DSHRESULTERROR (0x81A5)  // HD->SD is not supported.
#define DS_E_OUTPUT_SURFACES_DONT_MATCH                         MAKE_DSHRESULTERROR (0x81A6)  // output sufaces don't match with each other
#define DS_E_H264_DECODER_INIT_FAILED                           MAKE_DSHRESULTERROR (0x81A7)  // the init of H264Decoder failed.
#define DS_E_H264_DECOMPRESSION_FAILED                          MAKE_DSHRESULTERROR (0x81A8)  // the decomp of H264Decoder failed.
#define DS_E_10_TO_8_FAILED                                     MAKE_DSHRESULTERROR (0x81A9)  // 10->8 failed.
#define DS_E_COLOR_CONVERSION_FAILED                            MAKE_DSHRESULTERROR (0x81AA)  // color conversion failed.
#define DS_E_RESIZE_FAILED                                      MAKE_DSHRESULTERROR (0x81AB)  // resize failed.

#define DS_E_INVALID_AUDIO_SEQUENCE                             MAKE_DSHRESULTERROR (0x81AC)  // The sequence of audio samples associated with the video is invalid.  

#define DS_E_FIFO_PUSH_FAILED                                   MAKE_DSHRESULTERROR (0x81AD)  // fail to push an item to an FIFO queue.
#define DS_E_PAUSEAT_TIMEOUT                                    MAKE_DSHRESULTERROR (0x81AE)  // The media position asked by PauseAt not ready after time out. 
#define DS_E_FIFO_POP_FAILED                                    MAKE_DSHRESULTERROR (0x81AF)  // fail to pop an item out of an FIFO queue.
#define DS_E_UNCOMPRESSED_DATA_NOT_READY                        MAKE_DSHRESULTERROR (0x81B0)  // the uncompressed data are not ready
#define DS_E_PUT_DATA_NOT_READY                                 MAKE_DSHRESULTERROR (0x81B1)  // the Put data are not ready
#define DS_E_STREAM_NOT_OPENED                                  MAKE_DSHRESULTERROR (0x81B2)  // the stream is not opened.
#define DS_E_INVALID_CODEC_GUID                                 MAKE_DSHRESULTERROR (0x81B3)  // the codec guid is not valid.
#define DS_E_CANCELLED_BUFFER                                   MAKE_DSHRESULTERROR (0x81B4)  // a buffer is cancelled.
#define DS_E_CANCELLED_BUFFER_CODEC_IN_ERROR_STATE              MAKE_DSHRESULTERROR (0x81B5)  // a buffer is cancelled due to a Codec error.
#define DS_E_CAPTURE_PERFORMANCE_FAILED                         MAKE_DSHRESULTERROR (0x81B6)  // The capture could not be sustained due to system performance.          
#define DS_E_TIMEOUT_WAITING_FOR_MOV_MUXER                      MAKE_DSHRESULTERROR (0x81B7)  // The MOV muxer was not able to process our request within a time out period
#define DS_E_TIMEOUT_WAITING_FOR_VIDEO_SURFACES                 MAKE_DSHRESULTERROR (0x81B8)  // The surfaces were not ready within a time out period
#define DS_E_POSITION_INVALID                                   MAKE_DSHRESULTERROR (0x81B9)  // The specified position cannot be used
#define DS_E_READ_COUNT_ALREADY_AT_ZERO                         MAKE_DSHRESULTERROR (0x81BA)  // The read count is already at zero, and cannot be decremented
#define DS_E_WRITE_COUNT_ALREADY_AT_ZERO                        MAKE_DSHRESULTERROR (0x81BB)  // The write count is already at zero, and cannot be decremented
#define DS_E_INVALID_THREADS_AFFINITY_MASK                      MAKE_DSHRESULTERROR (0x81BC)  // The threads affinity mask is invalid        
#define DS_E_INVALID_THREADS_MAXIMUM                            MAKE_DSHRESULTERROR (0x81BD)  // The maximum number of processing threads given is invalid                                    
#define DS_E_BRAND_CODE_NOT_SPECIFIED                           MAKE_DSHRESULTERROR (0x81BE)  // The major brand for writing MOV files is not specified.  
#define DS_E_FORCE_WRITE_COMPLETION                             MAKE_DSHRESULTERROR (0x81BF)  // Forced a write completion.  
#define DS_E_CODEC_NOT_EXIST                                    MAKE_DSHRESULTERROR (0x81C0)  // The codec does not exist.
#define DS_E_HD_DECOMP_NOT_SUPPORT                              MAKE_DSHRESULTERROR (0x81C1)  // The HD decompression is not supported.
#define DS_E_SD_DECOMP_NOT_SUPPORT                              MAKE_DSHRESULTERROR (0x81C2)  // The SD decompression is not supported.

#define DS_E_ORIGIN_NOT_ALIGNED_WITH_FORMAT_GRANULARITY         MAKE_DSHRESULTERROR (0x81C3)  // The specified origin does not respect the surface format granularity.
#define DS_E_NONZERO_HORIZONTAL_OFFSET_NOT_SUPPORTED_FOR_IO     MAKE_DSHRESULTERROR (0x81C4)  // The horizontal coordinate of alias region origin must be zero for IO surfaces

#define DS_E_MPS_INVALID_ZIGZAG_TYPE                            MAKE_DSHRESULTERROR (0x81C5)  // The mpeg zigzag type is invalid

#define DS_E_DYNAMIC_SHUTTLING_NOT_SUPPORTED                    MAKE_DSHRESULTERROR (0x81C6)  // Dynamic shuttling (changing shuttling speed while playing) is not supported [For Streaming Codecs]
#define DS_E_PREROLL_NOT_RESPECTED                              MAKE_DSHRESULTERROR (0x81C7)  // The hardware preroll of the IO board has not been respected.
#define DS_E_CORRUPT_FILE                                       MAKE_DSHRESULTERROR (0x81C8)  // The file is corrupt.
#define DS_E_FILE_SIZE_NOT_SUPPORTED                            MAKE_DSHRESULTERROR (0x81C9)  // The file size is not supported.

#define DS_E_RTSP_OPEN_SESSION_OPEN                             MAKE_DSHRESULTERROR (0x81CA)  // The RTSP Session is already opened
#define DS_E_RTSP_MEDIA_SESSION_PLAYING                         MAKE_DSHRESULTERROR (0x81CB)  // The RTSP Media Session is not stopped
#define DS_E_RTSP_MEDIA_SESSION_INVLD                           MAKE_DSHRESULTERROR (0x81CC)  // The RTSP Session is invalid
#define DS_E_RTP_MEDIA_SESSION_INVLD                            MAKE_DSHRESULTERROR (0x81CD)  // The RTP Session is invalid
#define DS_E_RTP_MEDIA_SESSION_STOPPING                         MAKE_DSHRESULTERROR (0x81CE)  // Unable to play session

#define DS_E_CODEC_PROFILE_NOT_SUPPORTED                        MAKE_DSHRESULTERROR (0x81CF)  // The required codec profile is not supported.
#define DS_E_RESOLUTION_NOT_SUPPORTED                           MAKE_DSHRESULTERROR (0x81D0)  // The given resolution is not supported.  

#define DS_E_MULTIPLE_VANC_PACKETS_OF_SAME_TYPE_NOT_SUPPORTED   MAKE_DSHRESULTERROR (0x81D1)  // There are at least two vanc packets of the same type in the surface.
#define DS_E_STREAMING_COMMANDS_NOT_PROCESSED                   MAKE_DSHRESULTERROR (0x81D2)  // there are commands in the queue while closing a stream

#define DS_E_MISSING_TRACK_INFOS                                MAKE_DSHRESULTERROR (0x81D3)  // SDsMediaClipTrackInfo doesn't set for all tracks
#define DS_E_WRONG_UNCOMPRESSED_PUT                             MAKE_DSHRESULTERROR (0x81D4)  // Wrong Put in terms of the order of video surface and vbi surface.
#define DS_E_NOT_ENOUGH_OUTPUT_SURFACES                         MAKE_DSHRESULTERROR (0x81D5)  // There are not enough output surfaces.
#define DS_E_TOO_MANY_OUTPUT_SURFACES                           MAKE_DSHRESULTERROR (0x81D6)  // There are too many output surfaces.
#define DS_E_APPENDING_DATA_RATE_NOT_SUPPORTED                  MAKE_DSHRESULTERROR (0x81D7)  // The appending capture data rate is different from the appended file's data rate. 

#define DS_E_UNSUPPORTED_AUDIO_BITS_PER_SAMPLES                 MAKE_DSHRESULTERROR (0x81D8)  // The value in field ulBitsPerSample of the structure SDsCreateAudioSamplesDescription is not supported.
#define DS_E_UNSUPPORTED_AUDIO_VALID_BITS_PER_SAMPLES           MAKE_DSHRESULTERROR (0x81D9)  // The value in field ulValidBitsPerSample of the structure SDsCreateAudioSamplesDescription is not supported.
#define DS_E_INVALID_COLORIMETRY                                MAKE_DSHRESULTERROR (0x81DA)  // The value in field of type EDsColorimetry is invalid.

#define DS_E_AES3_DATA_MISSING_AUDIO_SAMPLE_NUMBER              MAKE_DSHRESULTERROR (0x81DB)  // The AES3 data is missing audio sample number. 

#define DS_E_VIDEO_IO_CHANNEL_NOT_SUPPORTED                     MAKE_DSHRESULTERROR (0x81DC)  // The Video Channel IO is not supported 
#define DS_E_STREAM_INDEX_TOO_HIGH                              MAKE_DSHRESULTERROR (0x81DD)  // The stream index is higher than what is captured or can be captured
#define DS_E_INVALID_VIDEO_STANDARD                             MAKE_DSHRESULTERROR (0x81DE)  // The resolution is not supported or unknown
#define DS_E_NOT_SUPPORTED_FOR_HARDWARE_AVCDECODER              MAKE_DSHRESULTERROR (0x81DF)  // FOR hardware AVC decoder, only support the progressive file and playlist

#define DS_E_UNSUPPORTED_AUDIO_INDEX_FILE                       MAKE_DSHRESULTERROR (0x81E0)  // In index mode we not supported to play a file with multiple audio streams
#define DS_E_UNSUPPORTED_VBI_OPTION                             MAKE_DSHRESULTERROR (0x81E1)  // There is no VBI input, but VBI option is checked. Needs to change the VBI option.
#define DS_E_AUDIO_ENCODER_NOT_AVAILABLE                        MAKE_DSHRESULTERROR (0x81E2)  // We do not have a codec for the requested capture type

#define DS_E_MPEG2TS_MUXER_INIT_FAIL                            MAKE_DSHRESULTERROR (0x81E3)  // Fail to init the MPEG2TS Muxer
#define DS_E_END_OF_STREAM                                      MAKE_DSHRESULTERROR (0x81E4)  // End of stream has been reached
#define DS_E_FAILED_TO_ADD_VBI_DATA                             MAKE_DSHRESULTERROR (0x81E5)  // Failed to add VBI data

#define DS_E_FAILED_TO_LOAD_DLL                                 MAKE_DSHRESULTERROR (0x81E6)  // Failed to load dll
#define DS_E_FAILED_TO_CREATE_DNxHD_CODEC                       MAKE_DSHRESULTERROR (0x81E7)  // Failed to create DNxHD codec
#define DS_E_INVALID_DNxHD_PROFILE                              MAKE_DSHRESULTERROR (0x81E8)  // Failed to create DNxHD codec

#define DS_E_TOPOLOGY_HAS_BEEN_DESTROYED_FROM_CARD              MAKE_DSHRESULTERROR (0x81E9)  // The topology which implements the method has already been destroyed from the card.
#define DS_E_STREAM_HAS_BEEN_DESTROYED_FROM_CARD                MAKE_DSHRESULTERROR (0x81EA)  // The stream which implements the method has already been destroyed from the card.
#define DS_E_NODE_HAS_BEEN_DESTROYED_FROM_CARD                  MAKE_DSHRESULTERROR (0x81EB)  // The node which implements the method has already been destroyed from the card.
#define DS_E_TOPOLOGY_NOT_CONTROLLED_BY_PROCESS                 MAKE_DSHRESULTERROR (0x81EC)  // The topology (or the topology attached to object) is not controlled for modification by the process
#define DS_E_INVALID_REFENCE_MOV_FILENAME_FOR_TDIR              MAKE_DSHRESULTERROR (0x81ED)  // Failed to do MOV TDIR because invalid reference mov file name
#define DS_E_FAILED_TO_CREATE_SOFTWARE_SCALER_EFFECT            MAKE_DSHRESULTERROR (0x81EE)  // Failed to create software scaler effect

#define DS_E_PARITY_ERROR                                       MAKE_DSHRESULTERROR (0x81EF)  // Vanc party error 
#define DS_E_CHECKSUM_ERROR                                     MAKE_DSHRESULTERROR (0x81F0)  // Vanc checksum error 

#define DS_E_FATAL_ERROR_RETEST_WITH_LOGS                       MAKE_DSHRESULTERROR (0x81F1)  // A fatal error occurred. Stability cannot be garanteed from this point. Please retest with logging enabled to know more details.

#define DS_E_UNSUPPORTED_WDS_WIDTH                              MAKE_DSHRESULTERROR (0x81F2) // The requested WDS capture width is not supported, it must be even number.
#define DS_E_INVALID_MEMORY_LOCATION                            MAKE_DSHRESULTERROR (0x81F3) // The expected memory location of the surface or audio samples is incorrect.
#define DS_E_UNSUPPORTED_AUDIO_SAMPLES_PER_SECOND               MAKE_DSHRESULTERROR (0x81F4) // The value in field ulBitsPerSample of the structure SDsCreateAudioSamplesDescription is not supported.
#define DS_E_INVALID_MXF_FILE_TYPE                              MAKE_DSHRESULTERROR (0x81F5) // The captured MXF file type is not set properly

#define DS_E_NODE_IS_IN_USE_BY_A_STREAM                         MAKE_DSHRESULTERROR (0x81F6) // The delete operation on the node can't be done because it is still in use by a stream

#define DS_E_INVALID_NTSC_CLOSED_CAPTION_FORMAT                 MAKE_DSHRESULTERROR (0x81F7) // The NTSC closed caption format is invalid. It should be digital, line21 or both.

#define DS_E_SAME_INPUT_AND_OUTPUT_NODE                         MAKE_DSHRESULTERROR (0x81F8) // We can't use the same node for input and output on a stream
#define DS_E_VERTICAL_UPSCALE_NOT_SUPPORTED                     MAKE_DSHRESULTERROR (0x81F9) // The OnBoard scaler does not support vertical upscale.
#define DS_E_INVALID_BACKGROUND_COLOR                           MAKE_DSHRESULTERROR (0x81FA) // The background color is invalid
#define DS_E_INVALID_BACKGROUND_HANDLING                        MAKE_DSHRESULTERROR (0x81FB) // The background handling is invalid
#define DS_E_INVALID_COMPOSITING_MODE                           MAKE_DSHRESULTERROR (0x81FC) // The compositing mode is invalid
#define DS_E_INVALID_TRANSFORM_TYPE                             MAKE_DSHRESULTERROR (0x81FD) // The transform type is invalid
#define DS_E_INVALID_TRANSPARENCY_VALUE                         MAKE_DSHRESULTERROR (0x81FE) // The transparency value should be between 0 and 1
#define DS_E_WATCHDOG_HAS_BEEN_DESTROYED_FROM_CARD              MAKE_DSHRESULTERROR (0x81FF) // The watchdog which implements the method has already been destroyed from the card.
#define DS_E_INVALID_TIME_VALUE                                 MAKE_DSHRESULTERROR (0x8200) // The time value given is invalid. Probably because it is under 0
#define DS_E_INVALID_AUDIO_VOLUME                               MAKE_DSHRESULTERROR (0x8201) // The audio volume is invalid
#define DS_E_ARGB_NODE_NOT_SUPPORTED                            MAKE_DSHRESULTERROR (0x8202) // The ARGB node supplied is not supported as input/output for a stream
#define DS_E_VIDEO_MIXER_NOT_PRESENT                            MAKE_DSHRESULTERROR (0x8203)  // An error occured during the initialization of the inline videoMixer engine. 


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ---------------------- WARNINGS CODE ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#define  DS_W_HW_OVERLAY_NOT_AVAILABLE                      MAKE_DSHRESULTWARNING(0x0001)  // Hardware overlay for live window display is not available.
#define  DS_W_NOT_SUPPORTED                                 MAKE_DSHRESULTWARNING(0x0002)  // The operation was not completed because it is not supported, but it does not harm the system.
#define  DS_W_BITMAP_NEED_UPDATE                            MAKE_DSHRESULTWARNING(0x0003)  // Information that tells that a bitmap update is required for the live window.
#define  DS_W_PREVIOUS_NODE_REPLACED                        MAKE_DSHRESULTWARNING(0x0004)  // Information that tells that an existing tree node has been replaced.
#define  DS_W_MEMORY_WAS_FREE                               MAKE_DSHRESULTWARNING(0x0005)  // Information that tells that memory was free with the resource manager garbage collection.
#define  DS_W_MEMORY_NO_ACTION                              MAKE_DSHRESULTWARNING(0x0006)  // Information that tells that resource manager was not able to free any memory on garbage collection.
#define  DS_W_NOT_EXECUTING                                 MAKE_DSHRESULTWARNING(0x0007)  // Information cannot be accessed because the job is not executing.
#define  DS_W_ITEM_FOUND                                    MAKE_DSHRESULTWARNING(0x0008)  // Information that tells that the specified item was found.
#define  DS_W_EMPTY_ENGINE                                  MAKE_DSHRESULTWARNING(0x0009)  // Information that tells that returned values may not be good because the processing engine is empty.
#define  DS_W_ENUM_END_OF_LIST                              MAKE_DSHRESULTWARNING(0x000A)  // Enumerator has reached the end of the list to enumerate.
#define  DS_W_FIELD_NOT_FOUND_IN_FILE                       MAKE_DSHRESULTWARNING(0x000B)  // No specified field name is found in the file at the current position.
#define  DS_W_POS_AT_BOUNDARY                               MAKE_DSHRESULTWARNING(0x000C)  // The timeline position is at the boundary of an element.
#define  DS_W_POS_NOT_IN_ELEMENT                            MAKE_DSHRESULTWARNING(0x000D)  // The timeline posiiton is not inside any eleemnt. It is inside a hole.
#define  DS_W_EMPTY_RANGE                                   MAKE_DSHRESULTWARNING(0x000E)  // The start point and the end point of the specified range are equal.
#define  DS_W_NOTHING_TO_DO                                 MAKE_DSHRESULTWARNING(0x000F)  // The object doesn't have anything else to do.
#define  DS_W_REQUEST_NEW_POSITION                          MAKE_DSHRESULTWARNING(0x0010)  // The object requests a new position in order to be fully efficient.
#define  DS_W_MEMORY_NEEDED                                 MAKE_DSHRESULTWARNING(0x0011)  // The object needs memory in order to continue doing its work.
#define  DS_W_CONTAINER_EMPTY                               MAKE_DSHRESULTWARNING(0x0012)  // The container was empty on the call.
#define  DS_W_CONTAINER_NOW_EMPTY                           MAKE_DSHRESULTWARNING(0x0013)  // The container was emptied by the the call.
#define  DS_W_ITEM_FOUND_NOT_COMPLETED                      MAKE_DSHRESULTWARNING(0x0014)  // Information that tells that the specified item was found but operation is not completed.
#define  DS_W_WRONG_EVENT_STATE                             MAKE_DSHRESULTWARNING(0x0015)  // An event is not in the state that should be (signalled or unsignalled).
#define  DS_W_LATE_BUFFER_DROPPED_AT_OUTPUT                 MAKE_DSHRESULTWARNING(0x0016)  // A buffer arrived late at the output and was dropped.
#define  DS_W_STOP_REQUESTED                                MAKE_DSHRESULTWARNING(0x0017)  // While doing some processing, a stop command arrived.
#define  DS_W_FILE_NOT_FOUND                                MAKE_DSHRESULTWARNING(0x0018)  // The file was not found.
#define  DS_W_DROPPED_CAPTURE_OF_VIDEO_AT_INPUT             MAKE_DSHRESULTWARNING(0x0019)  // Dropped some video at capture of input.
#define  DS_W_DROPPED_CAPTURE_OF_AUDIO_AT_INPUT             MAKE_DSHRESULTWARNING(0x001A)  // Dropped some audio at capture of input.
#define  DS_W_MEMORY_WAS_FREE_AUDIO                         MAKE_DSHRESULTWARNING(0x0020)  // Information that tells that audio memory was free with the resource manager garbage collection.
#define  DS_W_MEMORY_WAS_FREE_VIDEO                         MAKE_DSHRESULTWARNING(0x0021)  // Information that tells that video memory was free with the resource manager garbage collection.
#define  DS_W_MEMORY_WAS_FREE_AUDIO_AND_VIDEO               MAKE_DSHRESULTWARNING(0x0022)  // Information that tells that audio and video memory was free with the resource manager garbage collection.
#define  DS_W_CAPTURE_OF_VIDEO_AT_INPUT_START_LATE          MAKE_DSHRESULTWARNING(0x0023)  // Start Time set for first video surface to be captured from input is late.
#define  DS_W_CAPTURE_OF_AUDIO_AT_INPUT_START_LATE          MAKE_DSHRESULTWARNING(0x0024)  // Start Time set for first audio buffer to be captured from input is late.
#define  DS_W_CLOSE_TO_END_OF_DISK                          MAKE_DSHRESULTWARNING(0x0025)  // Close to end of disk .
#define  DS_W_AVI_AUDIO_NOT_SUPPORTED                       MAKE_DSHRESULTWARNING(0x0026)  // The interleaved AV file info is known to matrox file reader but the audio format is not supported. The video portion of the file may still be played.
#define  DS_W_AVI_VIDEO_NOT_SUPPORTED                       MAKE_DSHRESULTWARNING(0x0027)  // The interleaved AV file info is known to matrox file reader but the video format is not supported. The audio portion of the file may still be played.
#define  DS_W_MISMATCH_PIXEL_ASPECT_RATIO                   MAKE_DSHRESULTWARNING(0x0028)  // The pixel aspect ratio is not the same as the current pixel aspect ratio determined from the editing resolution. The file can still be streamed.
#define  DS_W_MISMATCH_FILE_ASPECT_RATIO                    MAKE_DSHRESULTWARNING(0x0029)  // The file aspect ratio is not the same as the current editing aspect ratio. The file may still be streamed.
#define  DS_W_LOST_EMBEDDED_AUDIO_INPUT                     MAKE_DSHRESULTWARNING(0x002A)  // The embedded audio input is lost. 
#define  DS_W_LOST_VIDEO_INPUT                              MAKE_DSHRESULTWARNING(0x002B)  // The video input is lost.  
#define  DS_W_LOST_GENLOCK                                  MAKE_DSHRESULTWARNING(0x002C)  // The genlock source was lost.
#define  DS_W_GENLOCK_CHANGED                               MAKE_DSHRESULTWARNING(0x002D)  // The genlock source has changed.
#define  DS_W_MISMATCH_SCAN_MODE                            MAKE_DSHRESULTWARNING(0x002E)  // The scan mode of the file and the editing resolution are different.
#define  DS_W_AUDIO_INPUT_ABSENT                            MAKE_DSHRESULTWARNING(0x002F)  // The audio input is not present.
#define  DS_W_UIF_VIDEO_BUFFER_NOT_CAPTURED                 MAKE_DSHRESULTWARNING(0x0030)  // The UIF video capture returned a video buffer that was not captured into.
#define  DS_W_UIF_AUDIO_BUFFER_NOT_CAPTURED                 MAKE_DSHRESULTWARNING(0x0031)  // The UIF audio capture returned an audio buffer that was not captured into.
#define  DS_W_AUDIO_DROPPED_AT_RENDERING                    MAKE_DSHRESULTWARNING(0x0032)  // An audio buffer has been asked to be dropped during rendering.
#define  DS_W_VIDEO_DROPPED_AT_RENDERING                    MAKE_DSHRESULTWARNING(0x0033)  // A video buffer has been asked to be dropped during rendering.
#define  DS_W_1394_INPUT_NOT_RECEIVING_DATA                 MAKE_DSHRESULTWARNING(0x0034)  // The 1394 input is not receiving data.
#define  DS_W_1394_INPUT_DISCONNECTED                       MAKE_DSHRESULTWARNING(0x0035)  // The 1394 input is disconnected.
#define  DS_W_AUDIO_INPUT_START_SILENCE                     MAKE_DSHRESULTWARNING(0x0036)  // The audio input started generating silence
#define  DS_W_VIDEO_INPUT_START_BLACK                       MAKE_DSHRESULTWARNING(0x0037)  // The video input started generating black
#define  DS_W_AUDIO_INPUT_GLITCH                            MAKE_DSHRESULTWARNING(0x0038)  // The audio input generated some silence because there was input problem
#define  DS_W_VIDEO_INPUT_GLITCH                            MAKE_DSHRESULTWARNING(0x0039)  // The video input generated some black because there was input problem
#define  DS_W_1394_INPUT_START_RESOLUTION_MISMATCH          MAKE_DSHRESULTWARNING(0x003A)  // The 1394 input started receiving data with wrong resolution.
#define  DS_W_WAV_FILE_SIZE_LIMIT_ALMOST_REACHED            MAKE_DSHRESULTWARNING(0x003B)  // Close to .wav file size limit of 4 GB
#define  DS_W_END_OF_PIPE_FOUND                             MAKE_DSHRESULTWARNING(0x003C)  // An EndOfPipe element was found, need to handle this
#define  DS_W_REQUEST_CANCEL_NOT_SUPPORTED                  MAKE_DSHRESULTWARNING(0x003D)  // A request cannot be cancelled because it is not supported by the request properties.
#define  DS_W_CANNOT_CANCEL_ALREADY_FLUSHED                 MAKE_DSHRESULTWARNING(0x003E)  // A request cannot be cancelled because it is already completed and flushed.
#define  DS_W_INPUT_START_FRAME_RATE_MISMATCH               MAKE_DSHRESULTWARNING(0x003F)  // The input started receiving data with wrong frame rate.
#define  DS_W_NO_MEMORYCOLLECTORSET_FOR_GIVEN_POS_AND_DUR   MAKE_DSHRESULTWARNING(0x0040)  // A ProcessAll or ProcessBest pipe din't have a MemoryCollectorSet to put the given MemoryCollector.
#define  DS_W_REQUEST_COMPLETED                             MAKE_DSHRESULTWARNING(0x0041)  // Tells the scheduler that the last request executed is now completed.
#define  DS_W_PAUSE_AT_TIME_INTERMEDIATE                    MAKE_DSHRESULTWARNING(0x0042)  // Tells the trigger thread that the pause at position returned is not valid yet.
#define  DS_W_RUN_AT_TIME_INTERMEDIATE                      MAKE_DSHRESULTWARNING(0x0043)  // Tells the trigger thread that the run at position returned is not valid yet.
#define  DS_W_STILL_IN_USE                                  MAKE_DSHRESULTWARNING(0x0044)  // Protects access of elements in the VideoCache
#define  DS_W_LOST_AESEBU_AUDIO_INPUT                       MAKE_DSHRESULTWARNING(0x0045)  // The AES/EBU audio input is lost. 
#define  DS_W_GPU_CHIPSET_NOT_VALIDATED                     MAKE_DSHRESULTWARNING(0x0046)  // Given chipset was not tested.
#define  DS_W_GPU_DRIVER_NOT_VALIDATED                      MAKE_DSHRESULTWARNING(0x0047)  // Given driver was not tested
#define  DS_W_CLOSE_OTHER_3D_APPS_WARNING                   MAKE_DSHRESULTWARNING(0x0048)  // Close other 3D applications, since not enough VRAM.
#define  DS_W_VIDEO_INPUT_COPY_PROTECTED                    MAKE_DSHRESULTWARNING(0x0049)  // Video input is copy protected.
#define  DS_W_INVALID_PERFORMANCE_FACTOR_REPORTED           MAKE_DSHRESULTWARNING(0x0050)  // A performance factor has been reported but it seems to be too large.
#define  DS_W_UNABLE_TO_LOAD_DLL                            MAKE_DSHRESULTWARNING(0x0051)  // Unable to load the DLL file.
#define  DS_W_CURRENT_ADAPTER_NOT_ATI                       MAKE_DSHRESULTWARNING(0x0052)  // The graphic adapter used for the presentation window is not on an ATI card.
#define  DS_W_UNSUPPORTED_AUDIO_TRACK_CONFIGURATION         MAKE_DSHRESULTWARNING(0x0053)  // Mixing stereo and mono audio in a file is not supported. 
#define  DS_W_UNCOMPRESSED_SURFACE_NOT_USED                 MAKE_DSHRESULTWARNING(0x0054)  // Uncompressed surface is not used by the codec and released explicitely by the closs_stream call.
#define  DS_W_COMPRESSED_SURFACE_NOT_USED                   MAKE_DSHRESULTWARNING(0x0055)  // compressed surface is not used by the codec and released explicitely by the closs_stream call.
#define  DS_W_VIDEO_IP_MISSING_SURFACE                      MAKE_DSHRESULTWARNING(0x0056)  // The IP Module has detected one or more missing surfaces.
#define  DS_W_AUDIO_IP_MISSING_AUDIOSAMPLES                 MAKE_DSHRESULTWARNING(0x0057)  // The IP Module has detected one or more missing audio samples.
#define  DS_W_LOOP_REQUESTED                                MAKE_DSHRESULTWARNING(0x0058)  // reached end of playlist, Stop/Play requested in order to restart looping.
#define  DS_W_CAPTURE_SETTINGS_CHANGED                      MAKE_DSHRESULTWARNING(0x0059)  // Some of the capture settings have changed.
#define  DS_W_COULD_NOT_DECODE_VANC                         MAKE_DSHRESULTWARNING(0x005a)  // An error occurred while decoding the VANC surface.
#define  DS_W_MULTIPLE_VANC_PACKETS_OF_SAME_TYPE_NOT_SUPPORTED MAKE_DSHRESULTWARNING(0x005b) // Multiple VANC packets of the same type were found in the same frame, which is not supported for this VANC packet type.
#define  DS_W_TIMEOUT_WAITING_FOR_MEMORY                    MAKE_DSHRESULTWARNING(0x005c)  // Request for memory has expired but the maximum allowed has been reached.

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ---------------------- MESSAGE CODES ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define  DS_M_EC_COMPLETE                               MAKE_DSHRESULTWARNING(0x2000)  // EC_COMPLETE
#define  DS_M_FILE_CLOSED                               MAKE_DSHRESULTWARNING(0x2001)  // The file has been closed. Relevent for capture.
#define  DS_M_FILE_SWITCHED                             MAKE_DSHRESULTWARNING(0x2002)  // The file has been switched. Notification given by SinkSwitcher. 
#define  DS_M_START_REPEATING                           MAKE_DSHRESULTWARNING(0x2003)  // The video output has started to repeat. 
#define  DS_M_END_REPEATING                             MAKE_DSHRESULTWARNING(0x2004)  // The video output has stopped to repeat. 
#define  DS_M_START_SILENCE                             MAKE_DSHRESULTWARNING(0x2005)  // The audio output has started to play silence. 
#define  DS_M_END_SILENCE                               MAKE_DSHRESULTWARNING(0x2006)  // The audio output has stopped to play silence. 
#define  DS_M_GRAPH_HAS_BEEN_STOPPED                    MAKE_DSHRESULTWARNING(0x2007)  // The filter graph has been stopped due to a fatal error happens
#define  DS_M_VIDEO_INPUT_RECONNECTED                   MAKE_DSHRESULTWARNING(0x2008)  // The video input gets reconnected 
#define  DS_M_EMBEDDED_AUDIO_INPUT_RECONNECTED          MAKE_DSHRESULTWARNING(0x2009)  // The embedded audio input get reconnected.
#define  DS_M_PERFORMANCE_FACTOR_REPORTED               MAKE_DSHRESULTWARNING(0x200A)  // A performance factor has been reported.
#define  DS_M_EC_COMPLETE_FOR_STOP                      MAKE_DSHRESULTWARNING(0x200B)  // EC_COMPLETE_FOR_STOP
#define  DS_M_FILE_CAPTURE_FINISHED                     MAKE_DSHRESULTWARNING(0x200C)  // File has finished capturing to end-point
#define  DS_M_1394_INPUT_RECEIVING_DATA                 MAKE_DSHRESULTWARNING(0x200D)  // The 1394 has started to receive data.
#define  DS_M_1394_INPUT_CONNECTED                      MAKE_DSHRESULTWARNING(0x200E)  // The 1394 input is connected.
#define  DS_M_AUDIO_INPUT_END_SILENCE                   MAKE_DSHRESULTWARNING(0x200F)  // The audio input stopped generating silence
#define  DS_M_VIDEO_INPUT_END_BLACK                     MAKE_DSHRESULTWARNING(0x2010)  // The video input stopped generating black
#define  DS_M_1394_INPUT_END_RESOLUTION_MISMATCH        MAKE_DSHRESULTWARNING(0x2011)  // The 1394 input stopped receiving data with wrong resolution.
#define  DS_M_START_DEGRADATION                         MAKE_DSHRESULTWARNING(0x2012)  // Playback has started degradation.
#define  DS_M_END_DEGRADATION                           MAKE_DSHRESULTWARNING(0x2013)  // Playback has ended degradation.
#define  DS_M_ELEMENT_READER_DATA_ENOUGH                MAKE_DSHRESULTWARNING(0x2014)  // Element reader has received enough data. 
#define  DS_M_INPUT_END_FRAME_RATE_MISMATCH             MAKE_DSHRESULTWARNING(0x2015)  // The input stopped receiving data with wrong frame rate.
#define  DS_M_CAPTURE_OF_VIDEO_AT_INPUT_STARTED_OFF     MAKE_DSHRESULTWARNING(0x2016)  // Start Time set for first video surface to be captured from input is not exactly captured at start time.  Normal if not genlocked to input.
#define  DS_M_CAPTURE_OF_AUDIO_AT_INPUT_STARTED_OFF     MAKE_DSHRESULTWARNING(0x2017)  // Start Time set for first audio buffer to be captured from input is not exactly captured at start time.  Normal if not genlocked to input.
#define  DS_M_STILL_DEGRADING                           MAKE_DSHRESULTWARNING(0x2018)  // Playback is still in degradation state (between start and end degradation).
#define  DS_M_AESEBU_AUDIO_INPUT_RECONNECTED            MAKE_DSHRESULTWARNING(0x2019)  // The AES/EBU audio input get reconnected.
#define  DS_M_VIDEO_CHUNK_HAS_BEEN_BYTE_SWAPPED         MAKE_DSHRESULTWARNING(0x2020)  // The video chunk has been byte swapped.

   
   

};

      

#endif //__dsErrors_h_h

