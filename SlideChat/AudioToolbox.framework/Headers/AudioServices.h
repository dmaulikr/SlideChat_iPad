/*==================================================================================================
     File:       AudioToolbox/AudioServices.h

     Contains:   API for general high level audio services.

     Copyright:  (c) 2006 - 2011 by Apple, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(__AudioServices_h__)
#define __AudioServices_h__

//==================================================================================================

/*!
    @header AudioServices
    This header describes the API for several high level audio services.
    
    About AudioServices 
    AudioServices provides a means to play audio for things such as UI sound effects.
    
    About the Audio Hardware Service
    The Audio Hardware Service (AHS) provides a way for applications to query and manipulate the
    aspects of an audio hardware device without incurring the overhead of loading the full audio
    HAL. AHS provides access to all the AudioObjects and their properties on the system. However,
    access is limited to only those properties that do not directly impact IO. For example, you can
    query the device's format but you can't query its IO buffer size. As such, the AHS API directly
    incorporates the various structures and constants in HAL's API, with the caveat that the
    AudioObjectIDs used in AHS cannot be used with the HAL.
*/

//==================================================================================================
#pragma mark    Includes

#include <TargetConditionals.h>
#include <Availability.h>
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
    #if !TARGET_OS_IPHONE
        #include <CoreAudio/AudioHardware.h>
    #endif
    #include <CoreFoundation/CoreFoundation.h>
#else
	#include <AudioHardware.h>
    #include <CFRunLoop.h>
    #include <CFString.h>
    #include <CFURL.h>
#endif

//==================================================================================================

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark    AudioServices Error Constants

/*!
    @enum           AudioServices error codes
    @abstract       Error codes returned from the AudioServices portion of the API.
    @constant       kAudioServicesNoError
                        No error has occurred
    @constant       kAudioServicesUnsupportedPropertyError 
                        The property is not supported.
    @constant       kAudioServicesBadPropertySizeError 
                        The size of the property data was not correct.
    @constant       kAudioServicesBadSpecifierSizeError 
                        The size of the specifier data was not correct.
    @constant       kAudioServicesSystemSoundUnspecifiedError 
                        A SystemSound unspecified error has occurred.
    @constant       kAudioServicesSystemSoundClientTimedOutError 
                        SystemSound client message timed out
*/
enum
{
	kAudioServicesNoError									=  0,
	kAudioServicesUnsupportedPropertyError					= 'pty?',
	kAudioServicesBadPropertySizeError						= '!siz',
	kAudioServicesBadSpecifierSizeError 					= '!spc',

	kAudioServicesSystemSoundUnspecifiedError				= -1500,
	kAudioServicesSystemSoundClientTimedOutError			= -1501
};

//==================================================================================================
#pragma mark    AudioServices Types

/*!
    @typedef        SystemSoundID
    @abstract       SystemSoundIDs are created by the System Sound client application
                    for playback of a provided AudioFile.
*/
typedef UInt32      SystemSoundID;

/*!
    @typedef        AudioServicesPropertyID
    @abstract       Type used for specifying an AudioServices property.
*/
typedef UInt32      AudioServicesPropertyID;

/*!
    @typedef        AudioServicesSystemSoundCompletionProc
    @abstract       A function to be executed when a SystemSoundID finishes playing.
    @discussion     AudioServicesSystemSoundCompletionProc may be provided by client application to be
                    called when a SystemSoundID has completed playback.
    @param          ssID
                        The SystemSoundID that completed playback
    @param          userData
                        Client application user data
*/
typedef void
(*AudioServicesSystemSoundCompletionProc)(  SystemSoundID  ssID, 
                                        	void*          clientData);

//==================================================================================================
#pragma mark    AudioServices Constants

/*!
    @enum           AudioServices constants
    @abstract       Constants for use with System Sound portion of the AudioServices APIs.
    @constant       kSystemSoundID_UserPreferredAlert 
                        Use this constant with the play sound APIs to
                        playback the alert sound selected by the User in System Preferences.
    @constant       kSystemSoundID_Vibrate
                        Use this constant with the play sound APIs to vibrate the device
                        - iOS only 
                            - on a device with no vibration capability (like iPod Touch) this will 
                            do nothing
    @constant       kSystemSoundID_FlashScreen
                        Use this constant with the play sound APIs to flash the screen
                        - Desktop systems only
*/
enum
{
#if TARGET_OS_IPHONE
    kSystemSoundID_Vibrate              = 0x00000FFF
#else
    kSystemSoundID_UserPreferredAlert   = 0x00001000,
    kSystemSoundID_FlashScreen          = 0x00000FFE,
        // this has been renamed to be consistent
    kUserPreferredAlert     = kSystemSoundID_UserPreferredAlert
#endif        
};

//==================================================================================================
#pragma mark    AudioServices Properties

/*!
    @enum           AudioServices property codes
    @abstract       These are the property codes used with the AudioServices API.
    @constant       kAudioServicesPropertyIsUISound
                        a UInt32 where 1 means that the SystemSoundID passed in the
                        inSpecifier parameter will respect the 'Play user interface sounds effects'
                        check box in System Preferences and be silent when the user turns off UI
                        sounds. This property is set to 1 by default. Set to 0 if it is desired for
                        an SystemSoundID to always be heard when passed to AudioServicesPlaySystemSound(), 
                        regardless of user's setting in the Sound Preferences.
    @constant       kAudioServicesPropertyCompletePlaybackIfAppDies 
                        a UInt32 where 1 means that the SystemSoundID passed in the
                        inSpecifier parameter will finish playing even if the client application goes away.
*/
enum
{
    kAudioServicesPropertyIsUISound                   = 'isui',
    kAudioServicesPropertyCompletePlaybackIfAppDies   = 'ifdi'
};

//==================================================================================================
#pragma mark    AudioServices Functions

/*!
    @functiongroup  AudioServices
*/

/*!
    @function       AudioServicesPlayAlertSound
    @abstract       Play an Alert Sound
    @discussion     Play the provided SystemSoundID with AlertSound behavior.
    @param          inSystemSoundID
                        A SystemSoundID for the System Sound server to play. On the desktop you
                        can pass the kSystemSoundID_UserPreferredAlert constant to playback the alert sound 
                        selected by the user in System Preferences. On iOS there is no preferred user alert sound.
*/
extern void 
AudioServicesPlayAlertSound(SystemSoundID inSystemSoundID)                                          
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesPlaySystemSound
    @abstract       Play the sound designated by the provided SystemSoundID.
    @discussion     A SystemSoundID indicating the desired System Sound to be played.
    @param          inSystemSoundID
                        A SystemSoundID for the System Sound server to play.
*/
extern void 
AudioServicesPlaySystemSound(SystemSoundID inSystemSoundID)                                         
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesCreateSystemSoundID
    @abstract       Allows the application to designate an audio file for playback by the System Sound server.
    @discussion     SystemSoundIDs are used passed to
                    AudioServicesPlayAlertSound() & AudioServicesPlaySystemSound(),
                    designating the audio data to be played by the System Sound server.
    @param          inFileURL
                        A CFURLRef for an AudioFile.
    @param          outSystemSoundID
                        Returns a SystemSoundID.
*/
extern OSStatus 
AudioServicesCreateSystemSoundID(   CFURLRef                    inFileURL,
                                    SystemSoundID* 				outSystemSoundID)                   
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);
	
/*!
    @function       AudioServicesDisposeSystemSoundID
    @abstract       Allows the System Sound server to dispose any resources needed for the provided
                    SystemSoundID.
    @discussion     Allows the application to tell the System Sound server that the resources for the
                    associated audio file are no longer required.
    @param          inSystemSoundID
                        A SystemSoundID that the application no longer needs to use.
*/
extern OSStatus 
AudioServicesDisposeSystemSoundID(SystemSoundID inSystemSoundID)                                    
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesAddSystemSoundCompletion
    @abstract       Call the provided Completion Routine when provided SystemSoundID
                    finishes playing.
    @discussion     Once set, the System Sound server will send a message to the System Sound client
                    indicating which SystemSoundID has finished playing.
    @param          inSystemSoundID
                        The SystemSoundID to associate with the provided completion
                        routine.
    @param          inRunLoop
                        A CFRunLoopRef indicating the desired run loop the completion routine should
                        be run on. Pass NULL to use the main run loop.
    @param          inRunLoopMode
                        A CFStringRef indicating the run loop mode for the runloop where the
                        completion routine will be executed. Pass NULL to use kCFRunLoopDefaultMode.
    @param          inCompletionRoutine
                        An AudioServicesSystemSoundCompletionProc to be called when the provided
                        SystemSoundID has completed playing in the server.
    @param          inClientData
                        A void* to pass client data to the completion routine.
*/
extern OSStatus 
AudioServicesAddSystemSoundCompletion(  SystemSoundID          					inSystemSoundID,
                                    	CFRunLoopRef                        	inRunLoop,
                                    	CFStringRef                        		inRunLoopMode,
                                    	AudioServicesSystemSoundCompletionProc  inCompletionRoutine,
                                    	void*                               	inClientData)       
                                                                    __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesRemoveSystemSoundCompletion
    @abstract       Disassociate any completion proc for the specified SystemSoundID
    @discussion     Tells the SystemSound client to remove any completion proc associated with the
                    provided SystemSoundID
    @param          inSystemSoundID
                        The SystemSoundID for which completion routines should be
                        removed.
*/
extern void 
AudioServicesRemoveSystemSoundCompletion(SystemSoundID inSystemSoundID)                             
                                                                    __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesGetPropertyInfo
    @abstract       Get information about the size of an AudioServices property and whether it can
                    be set.
    @param          inPropertyID
                        a AudioServicesPropertyID constant.
    @param          inSpecifierSize
                        The size of the specifier data.
    @param          inSpecifier
                        A specifier is a buffer of data used as an input argument to some of the
                        properties.
    @param          outPropertyDataSize
                        The size in bytes of the current value of the property. In order to get the
                        property value, you will need a buffer of this size.
    @param          outWritable
                        Will be set to 1 if writable, or 0 if read only.
    @result         returns kAudioServicesNoError if successful.
*/                            
extern OSStatus 
AudioServicesGetPropertyInfo( AudioServicesPropertyID   inPropertyID,
                              UInt32                    inSpecifierSize,
                              const void*               inSpecifier,
                              UInt32*                   outPropertyDataSize,
                              Boolean*                  outWritable)                                
                                                                    __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesGetProperty
    @abstract       Retrieve the indicated property data
    @param          inPropertyID
                        a AudioServicesPropertyID constant.
    @param          inSpecifierSize
                        The size of the specifier data.
    @param          inSpecifier
                        A specifier is a buffer of data used as an input argument to some of the
                        properties.
    @param          ioPropertyDataSize
                        On input, the size of the outPropertyData buffer. On output the number of
                        bytes written to the buffer.
    @param          outPropertyData
                        The buffer in which to write the property data. May be NULL if caller only
                        wants ioPropertyDataSize to be filled with the amount that would have been
                        written.
    @result         returns kAudioServicesNoError if successful.
*/                        
extern OSStatus 
AudioServicesGetProperty(   AudioServicesPropertyID   inPropertyID,
                            UInt32                    inSpecifierSize,
                            const void*               inSpecifier,
                            UInt32*                   ioPropertyDataSize,
                            void*                     outPropertyData)                              
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

/*!
    @function       AudioServicesSetProperty
    @abstract       Set the indicated property data
    @param          inPropertyID
                        a AudioServicesPropertyID constant.
    @param          inSpecifierSize
                        The size of the specifier data.
    @param          inSpecifier
                        A specifier is a buffer of data used as an input argument to some of the
                        properties.
    @param          inPropertyDataSize
                        The size of the inPropertyData buffer.
    @param          inPropertyData
                        The buffer containing the property data.
    @result         returns kAudioServicesNoError if successful.
*/
extern OSStatus 
AudioServicesSetProperty(   AudioServicesPropertyID   inPropertyID,
                            UInt32                    inSpecifierSize,
                            const void*               inSpecifier,
                            UInt32                    inPropertyDataSize,
                            const void*               inPropertyData)                               
                                                                __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_2_0);

#if !TARGET_OS_IPHONE
//==================================================================================================
#pragma mark    Audio Hardware Service Properties

/*!
    @enum           Audio Hardware Service Properties
    @abstract       AudioObjectPropertySelector values that apply to various kinds of AudioObjects
                    only when accessed via the Audio Hardware Service API.
    @constant       kAudioHardwareServiceProperty_ServiceRestarted
                        A Float32 whose value has no meaning. Rather, this property exists so that
                        clients can be informed when the service has been reset for some reason.
                        When a reset happens, any state the client has with AHS, such as cached data
                        or added listeners, must be re-established by the client.
    @constant       kAudioHardwareServiceDeviceProperty_VirtualMasterVolume
                        A Float32 that represents the value of the volume control. The range is
                        between 0.0 and 1.0 (inclusive). This actual volume controls this property
                        manipulates depends on what the device provides. If the device has a true
                        master volume control, this property directly controls that. If the device
                        has individual channel volume controls, this property will apply to those
                        identified by the device's preferred multi-channel layout (or preferred
                        stereo pair if the device is stereo only). Note that this control maintains
                        the relative balance between all the channels it affects.
    @constant       kAudioHardwareServiceDeviceProperty_VirtualMasterBalance
                        A Float32 that represents the value of the stereo balance control. The range
                        is from 0.0 (all power to the left) to 1.0 (all power to the right) with
                        the value of 0.5 signifying that the channels have equal power. This control
                        is only present for devices that have individual channel volume controls. It
                        manipulates the relative balance between the volume controls on the channels
                        identified as the device's default stereo pair.
*/
enum
{
    kAudioHardwareServiceProperty_ServiceRestarted              = 'srst',
    kAudioHardwareServiceDeviceProperty_VirtualMasterVolume     = 'vmvc',
    kAudioHardwareServiceDeviceProperty_VirtualMasterBalance    = 'vmbc'
};

//==================================================================================================
#pragma mark    Audio Hardware Service Functions

/*!
    @functiongroup  Audio Hardware Service
*/

/*!
    @function       AudioHardwareServiceHasProperty
    @abstract       Queries an AudioObject about whether or not it has the given property.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @result         A Boolean indicating whether or not the AudioObject has the given property.
*/
extern Boolean
AudioHardwareServiceHasProperty(    AudioObjectID                       inObjectID,
                                    const AudioObjectPropertyAddress*   inAddress)                  
                                                            __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceIsPropertySettable
    @abstract       Queries an AudioObject about whether or not the given property can be set using
                    AudioHardwareServiceSetPropertyData.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          outIsSettable
                        A Boolean indicating whether or not the property can be set.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceIsPropertySettable( AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        Boolean*                            outIsSettable)          
                                                            __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceGetPropertyDataSize
    @abstract       Queries an AudioObject to find the size of the data for the given property.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData,
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          outDataSize
                        A UInt32 indicating how many bytes the data for the given property occupies.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceGetPropertyDataSize(    AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            UInt32                              inQualifierDataSize,
                                            const void*                         inQualifierData,
                                            UInt32*                             outDataSize)        
                                                            __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceGetPropertyData
    @abstract       Queries an AudioObject to get the data of the given property and places it in
                    the provided buffer.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData,
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          ioDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outData
                        The buffer into which the AudioObject will put the data for the given
                        property.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceGetPropertyData(    AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        UInt32                              inQualifierDataSize,
                                        const void*                         inQualifierData,
                                        UInt32*                             ioDataSize,
                                        void*                               outData)                
                                                        __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceSetPropertyData
    @abstract       Tells an AudioObject to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously.
    @param          inObjectID
                        The AudioObject to change.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being changed.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData,
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          inDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
    @param          inData
                        The buffer containing the data to be used to change the property's value.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceSetPropertyData(    AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        UInt32                              inQualifierDataSize,
                                        const void*                         inQualifierData,
                                        UInt32                              inDataSize,
                                        const void*                         inData)                 
                                                        __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceAddPropertyListener
    @abstract       Registers the given AudioObjectPropertyListenerProc to receive notifications
                    when the given properties change.
    @param          inObjectID
                        The AudioObject to register the listener with.
    @param          inAddress
                        The AudioObjectPropertyAddresses indicating which property the listener
                        should be notified about.
    @param          inListener
                        The AudioObjectPropertyListenerProc to call.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceAddPropertyListener(    AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            AudioObjectPropertyListenerProc     inListener,
                                            void*                               inClientData)       
                                                        __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

/*!
    @function       AudioHardwareServiceRemovePropertyListener
    @abstract       Unregisters the given AudioObjectPropertyListenerProc from receiving
                    notifications when the given properties change.
    @param          inObjectID
                        The AudioObject to unregister the listener from.
    @param          inAddress
                        The AudioObjectPropertyAddresses indicating which property the listener
                        will stop being notified about.
    @param          inListener
                        The AudioObjectPropertyListenerProc being removed.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceRemovePropertyListener( AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            AudioObjectPropertyListenerProc     inListener,
                                            void*                               inClientData)       
                                                        __OSX_AVAILABLE_STARTING(__MAC_10_5,__IPHONE_NA);

#endif //!TARGET_OS_IPHONE

#if TARGET_OS_IPHONE
//==================================================================================================
#pragma mark -
#pragma mark    AudioSession Error Constants

/*!
    @enum           AudioSession error codes
    @abstract       Error codes returned from the AudioSession portion of the API.
    @constant       kAudioSessionNoError
                        No error has occurred
    @constant       kAudioSessionNotInitialized 
                        This error is returned when the AudioSessionInitialize function
                        was not called prior to calling any other AudioSession function.
    @constant       kAudioSessionAlreadyInitialized 
                        This error is returned when you call AudioSessionInitialize more than once.
    @constant       kAudioSessionInitializationError 
                        This error indicates an AudioSession initialization error.
    @constant       kAudioSessionUnsupportedPropertyError 
                        The property is not supported.  This error code can also be used to 
                        indicate that a bad property value was passed in a SetProperty call,
                        an attempt was made to set a read-only property, an attempt was made to
                        read a write-only property, an attempt was made to read a property that
                        is only available by way of a property listener, or an attempt was made 
                        to set a listener on a property for which listeners are not supported.
    @constant       kAudioSessionBadPropertySizeError 
                        The size of the property data was not correct.
    @constant       kAudioSessionNotActiveError 
                        The operation failed because the AudioSession is not active.
                        Calling AudioSessionSetActive(true) first will fix this error in most cases.
    @constant       kAudioSessionNoCategorySet
                        The requested operation failed because it requires that the session have had an
                        audio category explicitly set, and none was set.
    @constant       kAudioSessionIncompatibleCategory
                        The requested operation failed because the AudioSession has an incompatible
                        category (e.g. attempting to play or record when the category is AudioProcessing) or
                        the session is not active.
    @constant       kAudioSessionUnspecifiedError
                        An audio session unspecified error has occurred.  This would indicate an 
                        Apple-internal bug or that the audio system is currently in a bad state.
*/
enum
{
    kAudioSessionNoError                                =  0,
    kAudioSessionNotInitialized                         = '!ini',
    kAudioSessionAlreadyInitialized                     = 'init',
    kAudioSessionInitializationError                    = 'ini?',
    kAudioSessionUnsupportedPropertyError               = 'pty?',
    kAudioSessionBadPropertySizeError                   = '!siz',
    kAudioSessionNotActiveError                         = '!act',
    kAudioServicesNoHardwareError                       = 'nohw',
    kAudioSessionNoCategorySet                          = '?cat',
    kAudioSessionIncompatibleCategory                   = '!cat',
    kAudioSessionUnspecifiedError                       = 'what'
};

//==================================================================================================
#pragma mark -
#pragma mark    AudioSession Types

/*!
    @typedef        AudioSessionPropertyID
    @abstract       Type used for specifying an AudioSession property.
*/
typedef UInt32      AudioSessionPropertyID;

//==================================================================================================
#pragma mark -
#pragma mark    AudioSession Interruption States

/*!
    @enum           AudioSession interruptions states
    @abstract       These are used with the AudioSessionInterruptionListener to indicate
                    if an interruption begins or ends.
    @constant       kAudioSessionBeginInterruption
                        Indicates that this AudioSession has just been interrupted.
    @constant       kAudioSessionEndInterruption 
                        Indicates the end of an interruption.
*/
enum {
    kAudioSessionBeginInterruption  = 1,
    kAudioSessionEndInterruption    = 0
};

//==================================================================================================
#pragma mark -
#pragma mark    AudioSession Audio Categories

/*!
    @enum           AudioSession audio categories states
    @abstract       These are used with as values for the kAudioSessionProperty_AudioCategory property
                    to indicate the audio category of the AudioSession.
    @constant       kAudioSessionCategory_AmbientSound 
                        Use this category for background sounds such as rain, car engine noise, etc.  
                        Mixes with other music.
    @constant       kAudioSessionCategory_SoloAmbientSound 
                        Use this category for background sounds.  Other music will stop playing.
    @constant       kAudioSessionCategory_MediaPlayback 
                        Use this category for music tracks.
    @constant       kAudioSessionCategory_RecordAudio 
                        Use this category when recording audio.
    @constant       kAudioSessionCategory_PlayAndRecord 
                        Use this category when recording and playing back audio.
    @constant       kAudioSessionCategory_AudioProcessing
                        Use this category when using a hardware codec or signal processor while
                        not playing or recording audio.
*/
enum {
    kAudioSessionCategory_AmbientSound               = 'ambi',
    kAudioSessionCategory_SoloAmbientSound           = 'solo',
    kAudioSessionCategory_MediaPlayback              = 'medi',
    kAudioSessionCategory_RecordAudio                = 'reca',
    kAudioSessionCategory_PlayAndRecord              = 'plar',
    kAudioSessionCategory_AudioProcessing            = 'proc'
};

#pragma mark    AudioSession Audio Category Routing Overrides

/*!
    @enum           AudioSession audio category routing overrides
    @abstract       These are used with as values for the kAudioSessionProperty_OverrideAudioRoute property.
    @constant       kAudioSessionOverrideAudioRoute_None
                        No override.  Return audio routing to the default state for the current audio category.
    @constant       kAudioSessionOverrideAudioRoute_Speaker 
                        Route audio output to speaker.  Use this override with the kAudioSessionCategory_PlayAndRecord
                        category, which by default routes the output to the receiver.
*/
enum {
    kAudioSessionOverrideAudioRoute_None    = 0,
    kAudioSessionOverrideAudioRoute_Speaker = 'spkr'
};

//==================================================================================================
#pragma mark    AudioSession reason codes for route change

/*!
    @enum           AudioSession reason codes for route change
    @abstract       These are codes used when the kAudioSessionProperty_AudioRoute property changes
    @constant       kAudioSessionRouteChangeReason_Unknown
                        The reason is unknown.
    @constant       kAudioSessionRouteChangeReason_NewDeviceAvailable
                        A new device became available (e.g. headphones have been plugged in).
    @constant       kAudioSessionRouteChangeReason_OldDeviceUnavailable
                        The old device became unavailable (e.g. headphones have been unplugged).
    @constant       kAudioSessionRouteChangeReason_CategoryChange
                        The audio category has changed (e.g. kAudioSessionCategory_MediaPlayback
                        has been changed to kAudioSessionCategory_PlayAndRecord).
    @constant       kAudioSessionRouteChangeReason_Override
                        The route has been overridden (e.g. category is kAudioSessionCategory_PlayAndRecord
                        and the output has been changed from the receiver, which is the default, to the speaker).
    @constant       kAudioSessionRouteChangeReason_WakeFromSleep
                        The device woke from sleep.
    @constant       kAudioSessionRouteChangeReason_NoSuitableRouteForCategory
                        Returned when there is no route for the current category (for instance RecordCategory 
                        but no input device)
*/
enum {
	kAudioSessionRouteChangeReason_Unknown = 0,
	kAudioSessionRouteChangeReason_NewDeviceAvailable = 1,
	kAudioSessionRouteChangeReason_OldDeviceUnavailable = 2,
	kAudioSessionRouteChangeReason_CategoryChange = 3,
	kAudioSessionRouteChangeReason_Override = 4,
	kAudioSessionRouteChangeReason_WakeFromSleep = 6,
    kAudioSessionRouteChangeReason_NoSuitableRouteForCategory = 7
};

// see documentation for kAudioSessionProperty_AudioRouteChange
// Note: the string refers to "OutputDevice" for historical reasons.  Audio routes may contain zero or more inputs and 
//      zero or more outputs.
#define kAudioSession_AudioRouteChangeKey_Reason    "OutputDeviceDidChange_Reason"
    
// CFString version of kAudioSession_AudioRouteChangeKey_Reason.  This is more convenient to use than the raw string version.
// Available in iOS 5.0 or greater          
extern const CFStringRef kAudioSession_RouteChangeKey_Reason                __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);

// CFDictionary keys for kAudioSessionProperty_AudioRouteChange
// Available in iOS 5.0 or greater      
extern const CFStringRef   kAudioSession_AudioRouteChangeKey_PreviousRouteDescription  __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSession_AudioRouteChangeKey_CurrentRouteDescription   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);

// CFDictionary keys for kAudioSessionProperty_AudioRouteDescription    
// Available in iOS 5.0 or greater    
extern const CFStringRef   kAudioSession_AudioRouteKey_Inputs   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSession_AudioRouteKey_Outputs  __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);

// key(s) for the CFDictionary associated with each entry of the CFArrays returned by kAudioSession_AudioRouteKey_Inputs
// and kAudioSession_AudioRouteKey_Outputs.  
// Available in iOS 5.0 or greater        
extern const CFStringRef   kAudioSession_AudioRouteKey_Type     __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
    
    
/*!
    @enum           AudioSession route input types
    @abstract       These are the strings used with the kAudioSession_AudioRouteKey_Type key for the CFDictionary associated 
                    with kAudioSession_AudioRouteKey_Inputs.
                    Available in iOS 5.0 or greater      
    @constant       kAudioSessionInputRoute_LineIn
                    A line in input 
    @constant       kAudioSessionInputRoute_BuiltInMic
                    A built-in microphone input.  (Note that some devices like early iPods do not have this input)
    @constant       kAudioSessionInputRoute_HeadsetMic
                    A microphone that is part of a headset (combined microphone and headphones)
    @constant       kAudioSessionInputRoute_BluetoothHFP
                    A microphone that is part of a Bluetooth Hands-Free Profile device
    @constant       kAudioSessionInputRoute_USBAudio
                    A Universal Serial Bus input
 */    
extern const CFStringRef   kAudioSessionInputRoute_LineIn       __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSessionInputRoute_BuiltInMic   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSessionInputRoute_HeadsetMic   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSessionInputRoute_BluetoothHFP __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSessionInputRoute_USBAudio     __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
    
/*!
    @enum           AudioSession route output types
    @abstract       These are strings used with the kAudioSession_AudioRouteKey_Type key for the CFDictionary associated 
                    with kAudioSession_AudioRouteKey_Outputs. 
                    Available in iOS 5.0 or greater       
    @constant       kAudioSessionOutputRoute_LineOut
                    A line out output
    @constant       kAudioSessionOutputRoute_Headphones
                    Speakers in a headset (mic and headphones) or simple headphones
    @constant       kAudioSessionOutputRoute_BluetoothHFP
                    Speakers that are part of a Bluetooth Hands-Free Profile device
    @constant       kAudioSessionOutputRoute_BluetoothA2DP
                    Speakers in a Bluetooth A2DP device
    @constant       kAudioSessionOutputRoute_BuiltInReceiver
                    The speaker you hold to your ear when on a phone call
    @constant       kAudioSessionOutputRoute_BuiltInSpeaker
                    The built-in speaker
    @constant       kAudioSessionOutputRoute_USBAudio
                    Speaker(s) in a Universal Serial Bus device
    @constant       kAudioSessionOutputRoute_HDMI
                    Output via High-Definition Multimedia Interface
    @constant       kAudioSessionOutputRoute_AirPlay
                    Output on a remote Air Play device
 */
extern const CFStringRef kAudioSessionOutputRoute_LineOut           __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);        
extern const CFStringRef kAudioSessionOutputRoute_Headphones        __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);     
extern const CFStringRef kAudioSessionOutputRoute_BluetoothHFP      __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);   
extern const CFStringRef kAudioSessionOutputRoute_BluetoothA2DP     __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);  
extern const CFStringRef kAudioSessionOutputRoute_BuiltInReceiver   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef kAudioSessionOutputRoute_BuiltInSpeaker    __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef kAudioSessionOutputRoute_USBAudio          __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);   
extern const CFStringRef kAudioSessionOutputRoute_HDMI              __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef kAudioSessionOutputRoute_AirPlay           __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);    

    
// CFDictionary keys for kAudioSessionProperty_InputSources
extern const CFStringRef   kAudioSession_InputSourceKey_ID            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSession_InputSourceKey_Description   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);

// CFDictionary keys for kAudioSessionProperty_OutputDestinations
extern const CFStringRef   kAudioSession_OutputDestinationKey_ID            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
extern const CFStringRef   kAudioSession_OutputDestinationKey_Description   __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
    
    
//==================================================================================================
#pragma mark    AudioSession interruption types for end interruption events

/*!
     @enum          AudioSession Interruption types
     @abstract      When an app's AudioSessionInterruptionListener is called at the end of an interruption event,
                    the app may query to see if it should resume audio or not.  The interruption type can be 
                    obtained through the kAudioSessionProperty_InterruptionType, available in iOS 4.0 and
                    greater.
     @constant      kAudioSessionInterruptionType_ShouldResume 
                        Indicates that the interruption was one where it is appropriate to resume playback
                        at the conclusion of the interruption.  (e.g.: a phone call was rejected)
     @constant      kAudioSessionInterruptionType_ShouldNotResume
                        Indicates that the interruption was one where it is not appropriate to resume playback
                        at the conclusion of the interruption.  (e.g.: interruption due to iPod playback)
 */
enum {  // typedef UInt32 AudioSessionInterruptionType
    kAudioSessionInterruptionType_ShouldResume                      = 'irsm',
    kAudioSessionInterruptionType_ShouldNotResume                   = '!rsm'
};
typedef UInt32 AudioSessionInterruptionType;	    

//==================================================================================================
#pragma mark    AudioSession mode values
/*!
     @enum          AudioSession Modes
     @abstract      Modes modify the audio category in order to introduce behavior that is tailored to the specific
                    use of audio within an application.  Available in iOS 5.0 and greater.
     @constant      kAudioSessionMode_Default 
                        The default mode.
     @constant      kAudioSessionMode_VoiceChat
                        Only valid with kAudioSessionCategory_PlayAndRecord.  Appropriate for Voice Over IP
                        (VOIP) applications.  Reduces the number of allowable audio routes to be only those
                        that are appropriate for VOIP applications and may engage appropriate system-supplied
                        signal processing.  Has the side effect of setting 
                        kAudioSessionProperty_OverrideCategoryEnableBluetoothInput to true.
     @constant      kAudioSessionMode_VideoRecording     
                        Only valid with kAudioSessionCategory_PlayAndRecord or kAudioSessionCategory_RecordAudio.
                        Modifies the audio routing options and may engage appropriate system-supplied signal processing.
     @constant      kAudioSessionMode_Measurement
                        Appropriate for applications that wish to minimize the effect of system-supplied signal
                        processing for input and/or output audio signals.
     @constant      kAudioSessionMode_GameChat
                        Set by Game Kit on behalf of an application that uses a GKVoiceChat object; valid 
                        only with the kAudioSessionCategory_PlayAndRecord category.
                        Do not set this mode directly. If you need similar behavior and are not using
                        a GKVoiceChat object, use the kAudioSessionMode_VoiceChat mode.
 */
enum { 
    kAudioSessionMode_Default               = 'dflt',
    kAudioSessionMode_VoiceChat             = 'vcct',
    kAudioSessionMode_VideoRecording        = 'vrcd',
    kAudioSessionMode_Measurement           = 'msmt',
    kAudioSessionMode_GameChat              = 'gmct'
};

//==================================================================================================
#pragma mark    AudioSession Properties

/*!
    @enum           AudioSession property codes
    @abstract       These are the property codes used with the AudioSession API.
    @constant       kAudioSessionProperty_PreferredHardwareSampleRate
                        A Float64 indicating the preferred hardware sample rate for the AudioSession.
                        The actual sample rate may be different
    @constant       kAudioSessionProperty_PreferredHardwareIOBufferDuration 
                        A Float32 indicating the preferred hardware IO buffer duration in seconds.
                        The actual IO buffer duration may be different
    @constant       kAudioSessionProperty_AudioCategory 
                        A UInt32 value indicating the audio category for the AudioSession (see constants above).
    @constant       kAudioSessionProperty_AudioRouteChange 
                        The value for this property is ONLY provided with the property changed callback. You 
                        cannot get the value of this property (or set it).
                        The property changed callback provides a CFDictionaryRef with keyed values:
                        Key = kAudioSession_AudioRouteChangeKey_Reason; value is a CFNumberRef with one of the 
                        reasons listed above.
                        Key = kAudioSession_AudioRouteChangeKey_PreviousRouteDescription; value is a CFDictionaryRef containing
                        information about the previous route.  This dictionary is of exactly the same format as the 
                        dictionary associated with kAudioSessionProperty_AudioRouteDescription.  Available in iOS 5.0 or 
                        greater.
                        Key = kAudioSession_AudioRouteChangeKey_CurrentRouteDescription; value is a CFDictionaryRef containing
                        information about the new route.  This dictionary is of exactly the same format as the 
                        dictionary associated with kAudioSessionProperty_AudioRouteDescription.  Available in iOS 5.0 or 
                        greater.
    @constant       kAudioSessionProperty_CurrentHardwareSampleRate 
                        A Float64 indicating the current hardware sample rate
    @constant       kAudioSessionProperty_CurrentHardwareInputNumberChannels 
                        A UInt32 indicating the current number of hardware input channels
    @constant       kAudioSessionProperty_CurrentHardwareOutputNumberChannels 
                        A UInt32 indicating the current number of hardware output channels
    @constant       kAudioSessionProperty_CurrentHardwareOutputVolume 
                        A Float32 indicating the current output volume
    @constant       kAudioSessionProperty_CurrentHardwareInputLatency 
                        A Float32 indicating the current hardware input latency in seconds.
    @constant       kAudioSessionProperty_CurrentHardwareOutputLatency 
                        A Float32 indicating the current hardware output latency in seconds.
    @constant       kAudioSessionProperty_CurrentHardwareIOBufferDuration 
                        A Float32 indicating the current hardware IO buffer duration in seconds.
    @constant       kAudioSessionProperty_OtherAudioIsPlaying 
                        A UInt32 with a value other than zero when someone else, typically the iPod application, 
                        is playing audio
    @constant       kAudioSessionProperty_OverrideAudioRoute 
                        A UInt32 with one of two values: kAudioSessionOverrideAudioRoute_None or 
                        kAudioSessionOverrideAudioRoute_Speaker
    @constant       kAudioSessionProperty_AudioInputAvailable 
                        A UInt32 with a value other than zero when audio input is available.
                        Use this property, rather than the device model, to determine if audio input is available.
                        A listener will notify you when audio input becomes available.  For instance, when a headset 
                        is attached to the second generation iPod Touch, audio input becomes available via the wired 
                        microphone. 
    @constant       kAudioSessionProperty_ServerDied
    					Available with iOS 3.0 or greater
                        The value for this property is ONLY provided with the property changed callback. You cannot get the 
                        value of this property (or set it). The property changed callback notifies you that
                        the audio server has died.
    @constant       kAudioSessionProperty_OtherMixableAudioShouldDuck
    					Available with iOS 3.0 or greater
                        If the current session category of an application allows mixing (iPod playback in the background 
                        for example), then that other audio will be ducked when the current application makes any sound. 
                        An example of this is the Nike app that does this as it provides periodic updates to its user (it 
                        ducks any iPod music currently being played while it provides its status).
                        This defaults to off (0). Note that the other audio will be ducked for as long as the current 
                        session is active. 
                        You will need to deactivate your audio session when you want full volume playback of the other audio.
                        If your category is the Playback category and you have this set to its default (non-mixable), setting
                        this value to on, will also make your category mixable with others
                        (kAudioSessionProperty_OverrideCategoryMixWithOthers will be set to true)
    @constant       kAudioSessionProperty_OverrideCategoryMixWithOthers
    					Available with iOS 3.0 or greater
                        This allows an application to change the default behavior of some audio session categories with regards to 
                        whether other applications can play while your session is active. The two typical cases are:
                            (1) PlayAndRecord category
                                this will default to false, but can be set to true. This would allow iPod to play in the background 
                                while an app had both audio input and output enabled
                            (2) MediaPlayback category
                                this will default to false, but can be set to true. This would allow iPod to play in the background, 
                                but an app will still be able to play regardless of the setting of the ringer switch 
                            (3) Other categories
                                this defaults to false and cannot be changed (that is, the mix with others setting of these categories 
                                cannot be overridden 
                        An application must be prepared for setting this property to fail as behaviour may change in future releases. 
                        If an application changes their category, they should reassert the override (it is not sticky across 
                        category changes)
    @constant       kAudioSessionProperty_OverrideCategoryDefaultToSpeaker
    					Available with iOS 3.1 or greater
                        This allows an application to change the default behaviour of some audio session categories with regards to 
                        the audio route. The current category behavior is:
                            (1) PlayAndRecord category
                                this will default to false, but can be set to true. this will route to Speaker (instead of Receiver) 
                                when no other audio route is connected.
                            (2) Other categories
                                this defaults to false and cannot be changed (that is, the default to speaker setting of these 
                                categories cannot be overridden 
                        An application must be prepared for setting this property to fail as behaviour may change in future releases. 
                        If an application changes their category, they should reassert the override (it is not sticky across category changes)
    @constant       kAudioSessionProperty_OverrideCategoryEnableBluetoothInput
    					Available with iOS 3.1 or greater
                        This allows an application to change the default behaviour of some audio session categories with regards to showing 
                        bluetooth devices as available routes. The current category behavior is:
                            (1) PlayAndRecord category
                                this will default to false, but can be set to true. This will allow a paired bluetooth device to show up as 
                                an available route for input, while playing through the category-appropriate output
                            (2) Record category
                                this will default to false, but can be set to true. This will allow a paired bluetooth device to show up 
                                as an available route for input 
                            (3) Other categories
                                this defaults to false and cannot be changed (that is, enabling bluetooth for input in these categories is 
                                not allowed) 
                        An application must be prepared for setting this property to fail as behaviour may change in future releases. 
                        If an application changes their category, they should reassert the override (it is not sticky across category changes)
    @constant       kAudioSessionProperty_InterruptionType
                        Available with iOS 4.0 or greater
                        This is a read-only property that gives the type of the end interruption event.  Media playback apps (i.e., 
                        those apps that have a "play" button), may use this property as a guideline for when to resume playing after an 
                        interruption ends.  Apps without a "play" button, (e.g., games) should always resume audio playback when the 
                        interruption ends.  This property is only valid within the scope of the client app's AudioSessionInterruptionListener 
                        callback and only valid for the AudioSessionEndInterruption event.  Attempting to read the property at any other 
                        time is invalid.  
    @constant       kAudioSessionProperty_Mode
                        Available with iOS 5.0 or greater
                        A UInt32 value that specifies the mode to be combined with the Audio Category.  See AudioSession mode 
                        values defined above.
    @constant       kAudioSessionProperty_InputSources
                        Available with iOS 5.0 or greater
                        For use with certain accessories, such as some USB audio devices, that support input source selection.  
                        If the attached accessory supports source selection, provides a description of the available sources.
                        Not to be confused with kAudioSessionProperty_AudioRouteDescription, which provides a description
                        of the current audio route.  
                        A CFArray of CFDictionaries with the keys listed below.  If no input sources are 
                        available, a valid CFArray with 0 entries will be returned by a get operation.
                        Key = kAudioSession_InputSourceKey_ID; value is a CFNumberRef representing a system-defined identifier
                        for the input source.  This is the identifier to be used when setting the input source.  
                        Key = kAudioSession_InputSourceKey_Description; value is a CFStringRef description of the input source 
                        suitable for displaying in a user interface.  Examples: "Internal Mic", "External Mic", 
                        "Ext 48V Mic", "Instrument", "External Line Connector"
    @constant       kAudioSessionProperty_OutputDestinations
                        Available with iOS 5.0 or greater
                        For use with certain accessories, such as some USB audio devices, that support output destination selection.  
                        If the attached accessory supports destination selection, provides a description of the available destinations.
                        Not to be confused with kAudioSessionProperty_AudioRouteDescription, which provides a description
                        of the current audio route. 
                        A CFArray of CFDictionaries with the keys listed below.  If no output destinations are 
                        available, a valid CFArray with 0 entries will be returned by a get operation.
                        Key = kAudioSession_OutputDestinationKey_ID; value is a CFNumberRef representing a system-defined identifier
                        for the output destination.  This is the identifier to be used when setting the destination.
                        Key = kAudioSession_OutputDestinationKey_Description; value is a CFStringRef description of the output 
                        destination suitable for displaying in a user interface. 
    @constant       kAudioSessionProperty_InputSource
                        Available with iOS 5.0 or greater
                        For use with certain accessories, such as some USB audio devices, that support input source selection. 
                        A CFNumberRef value that specifies the input source to be selected.  The value must be one of the 
                        IDs provided by the kAudioSession_InputSourceKey_ID as part of the data associated with 
                        kAudioSessionProperty_InputSources.
    @constant       kAudioSessionProperty_OutputDestination
                        Available with iOS 5.0 or greater
                        For use with certain accessories, such as some USB audio devices, that support output destination selection.
                        A CFNumberRef value that specifies the output destination to be selected.  The value must be one 
                        of the IDs provided by the kAudioSession_OutputDestinationKey_ID as part of the data associated with
                        kAudioSessionProperty_OutputDestinations.
    @constant       kAudioSessionProperty_InputGainAvailable
                        Available with iOS 5.0 or greater
                        A UInt32 with a value other than zero when audio input gain is available.  Some inputs may not 
                        provide the ability to set the input gain, so check this value before attempting to set input gain.
    @constant       kAudioSessionProperty_InputGainScalar
                        Available with iOS 5.0 or greater
                        A Float32 value defined over the range [0.0, 1.0], with 0.0 corresponding to the lowest analog 
                        gain setting and 1.0 corresponding to the highest analog gain setting.  Attempting to set values
                        outside of the defined range will result in the value being "clamped" to a valid input.  This is 
                        a global input gain setting that applies to the current input source for the entire system.  
                        When no applications are using the input gain control, the system will restore the default input
                        gain setting for the input source.  Note that some audio accessories, such as USB devices, may 
                        not have a default value.  This property is only valid if kAudioSessionProperty_InputGainAvailable
                        is true.  Note that route change events represent substantive changes to the audio system. Input 
                        gain settings are not guaranteed to persist across route changes. Application code should be aware
                        that route change events can (and likely will) cause a change to input gain settings, and so should
                        be prepared to reassess the state of input gain after the new route is established.
    @constant       kAudioSessionProperty_AudioRouteDescription 
                        Available with iOS 5.0 or greater
                        A CFDictionaryRef with information about the current audio route; keyed values:
                        Key = kAudioSession_AudioRouteKey_Inputs; value is a CFArray of CFDictionaries with information about the 
                        inputs utilitized in the current audio route.  
                        Key = kAudioSession_AudioRouteKey_Outputs; value is a CFArray of CFDictionaries with information about the 
                        outputs utilitized in the current audio route.
                        Both kAudioSession_AudioRouteKey_Inputs and kAudioSession_AudioRouteKey_Outputs return a CFArray of
                        CFDictionaries with Key = kAudioSession_AudioRouteKey_Type; value is a CFString corresponding
                        to the input or output types documented above.
 */
enum { // typedef UInt32 AudioSessionPropertyID
    kAudioSessionProperty_PreferredHardwareSampleRate           = 'hwsr',   // Float64          (get/set)
    kAudioSessionProperty_PreferredHardwareIOBufferDuration     = 'iobd',   // Float32          (get/set)
    kAudioSessionProperty_AudioCategory                         = 'acat',   // UInt32           (get/set)
    kAudioSessionProperty_AudioRouteChange                      = 'roch',   // CFDictionaryRef  (property listener)
    kAudioSessionProperty_CurrentHardwareSampleRate             = 'chsr',   // Float64          (get only)
    kAudioSessionProperty_CurrentHardwareInputNumberChannels    = 'chic',   // UInt32           (get only)
    kAudioSessionProperty_CurrentHardwareOutputNumberChannels   = 'choc',   // UInt32           (get only)
    kAudioSessionProperty_CurrentHardwareOutputVolume           = 'chov',   // Float32          (get only/property listener)
    kAudioSessionProperty_CurrentHardwareInputLatency           = 'cilt',   // Float32          (get only)
    kAudioSessionProperty_CurrentHardwareOutputLatency          = 'colt',   // Float32          (get only)
    kAudioSessionProperty_CurrentHardwareIOBufferDuration       = 'chbd',   // Float32          (get only)
    kAudioSessionProperty_OtherAudioIsPlaying                   = 'othr',   // UInt32           (get only)
    kAudioSessionProperty_OverrideAudioRoute                    = 'ovrd',   // UInt32           (set only)
    kAudioSessionProperty_AudioInputAvailable                   = 'aiav',   // UInt32           (get only/property listener)
    kAudioSessionProperty_ServerDied                            = 'died',   // UInt32           (property listener)
    kAudioSessionProperty_OtherMixableAudioShouldDuck           = 'duck',   // UInt32           (get/set)
    kAudioSessionProperty_OverrideCategoryMixWithOthers         = 'cmix',   // UInt32           (get, some set)
    kAudioSessionProperty_OverrideCategoryDefaultToSpeaker      = 'cspk',   // UInt32           (get, some set)
    kAudioSessionProperty_OverrideCategoryEnableBluetoothInput  = 'cblu',   // UInt32           (get, some set)
    kAudioSessionProperty_InterruptionType                      = 'type',   // UInt32           (get only)
    kAudioSessionProperty_Mode                                  = 'mode',   // UInt32           (get/set)
    kAudioSessionProperty_InputSources                          = 'srcs',   // CFArrayRef       (get only/property listener)
    kAudioSessionProperty_OutputDestinations                    = 'dsts',   // CFArrayRef       (get only/property listener)
    kAudioSessionProperty_InputSource                           = 'isrc',   // CFNumberRef      (get/set)
    kAudioSessionProperty_OutputDestination                     = 'odst',   // CFNumberRef      (get/set)
    kAudioSessionProperty_InputGainAvailable                    = 'igav',   // UInt32           (get only/property listener)
    kAudioSessionProperty_InputGainScalar                       = 'igsc',   // Float32          (get/set/property listener)
    kAudioSessionProperty_AudioRouteDescription                 = 'crar',   // CFDictionaryRef  (get only)
};
    
//==================================================================================================
#pragma mark -
#pragma mark Callbacks
/*!
    @typedef        AudioSessionInterruptionListener
    @abstract       A function to be called when an interruption begins or ends.
    @discussion     AudioSessionInterruptionListener has to be provided by client applications in the
                    AudioSessionInitialize function.  It will be called when an interruption begins or ends.
    @param          inClientData
                        The client user data to use when calling the listener.
    @param          inInterruptionState
                        Indicates if the interruption begins (kAudioSessionBeginInterruption)
                        or ends (kAudioSessionEndInterruption)
*/
typedef void (*AudioSessionInterruptionListener)(
                                void *                  inClientData,
                                UInt32                  inInterruptionState);

/*!
    @typedef        AudioSessionPropertyListener
    @abstract       A function to be executed when a property changes.
    @discussion     AudioSessionPropertyListener may be provided by client application to be
                    called when a property changes.
    @param          inClientData
                        The client user data to use when calling the listener.
    @param          inID
                        The AudioSession property that changed
    @param          inDataSize
                        The size of the payload
    @param          inData
                        The payload of the property that changed (see data type for each property)
*/
typedef void (*AudioSessionPropertyListener)(
								void *                  inClientData,
								AudioSessionPropertyID	inID,
								UInt32                  inDataSize,
                                const void *            inData);

//==================================================================================================
#pragma mark -
#pragma mark    AudioSession Functions

/*!
    @functiongroup  AudioSession
*/

/*!
    @function       AudioSessionInitialize
    @abstract       Initialize the AudioSession.
    @discussion     This function has to be called once before calling any other
                    AudioSession functions.
    @param          inRunLoop
                        A CFRunLoopRef indicating the desired run loop the interruption routine should
                        be run on. Pass NULL to use the main run loop.
    @param          inRunLoopMode
                        A CFStringRef indicating the run loop mode for the runloop where the
                        completion routine will be executed. Pass NULL to use kCFRunLoopDefaultMode.
    @param          inInterruptionListener
                        An AudioSessionInterruptionListener to be called when the AudioSession
                        is interrupted.
    @param          inClientData
                        The client user data to use when calling the interruption listener.
*/
extern OSStatus
AudioSessionInitialize(             CFRunLoopRef                        inRunLoop, 
                                    CFStringRef                         inRunLoopMode, 
                                    AudioSessionInterruptionListener    inInterruptionListener, 
                                    void                                *inClientData)              
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

/*!
    @function       AudioSessionSetActive
    @abstract       Activate or deactivate the AudioSession.
    @discussion     Call this function with active set to true to activate this AudioSession (interrupt
                    the currently active AudioSession).
                    Call this function with active set to false to deactivate this AudioSession (allow
                    another interrupted AudioSession to resume).
                    When active is true this call may fail if the currently active AudioSession has a higher priority.
    @param          active
                        A Boolean indicating if you want to make this AudioSession active or inactive.
*/
extern OSStatus
AudioSessionSetActive(              Boolean                             active)                     
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

//==================================================================================================
#pragma mark    AudioSessionActivationFlags for AudioSessionSetActiveWithFlags
    
/*!
    @enum           Flags for AudioSessionSetActiveWithFlags
    @abstract       These are valid bitmap flags that may be combined and passed to AudioSessionSetActiveWithFlags
                    to refine this routine's behavior.
    @constant       kAudioSessionSetActiveFlag_NotifyOthersOnDeactivation
                        Notify an interrupted app that the interruption has ended and it may resume playback. Only
                        valid on session deactivation.
*/
enum {
    kAudioSessionSetActiveFlag_NotifyOthersOnDeactivation       = (1 << 0)  //  0x01
};
    
/*!
    @function       AudioSessionSetActiveWithFlags
    @abstract       Same functionality as AudioSessionSetActive, with an additional flags parameter for 
                    refining behavior.
    @discussion     Call this function with active set to true to activate this AudioSession (interrupt
                    the currently active AudioSession).
                    Call this function with active set to false to deactivate this AudioSession (allow
                    another interrupted AudioSession to resume).
                    Pass in one or more flags to refine the behavior during activation or deactivation.
                    When active is true this call may fail if the currently active AudioSession has a 
                    higher priority.
    @param          active
                        A Boolean indicating if you want to make this AudioSession active or inactive.
    @param          inFlags
                        A bitmap containing one or more flags from the AudioSessionActivationFlags enum.
*/
extern OSStatus
AudioSessionSetActiveWithFlags(     Boolean                             active,
                                    UInt32                              inFlags)                    
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);

/*!
    @function       AudioSessionGetProperty
    @abstract       Get the value of a property.
    @discussion     This function can be called to get the value for a property of the AudioSession.
                    Valid properties are listed in an enum above.
    @param          inID
                        The AudioSessionPropertyID for which we want to get the value.
    @param          ioDataSize
                        The size of the data payload.
                        On entry it should contain the size of the memory pointed to by outData.
                        On exit it will contain the actual size of the data.
    @param          outData
                        The data for the property will be copied here.
    @return         kAudioSessionNoError if the operation was successful.  If the property is a
                    write-only property or only available by way of property listeners, 
                    kAudioSessionUnsupportedPropertyError will be returned.  Other error codes 
                    listed under AudioSession Error Constants also apply to this function.
*/
extern OSStatus
AudioSessionGetProperty(            AudioSessionPropertyID              inID,
                                    UInt32                              *ioDataSize,
                                    void                                *outData)                   
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

/*!
    @function       AudioSessionSetProperty
    @abstract       Set the value of a property.
    @discussion     This function can be called to set the value for a property of the AudioSession.
                    Valid properties are listed in an enum above.
    @param          inID
                        The AudioSessionPropertyID for which we want to set the value.
    @param          inDataSize
                        The size of the data payload.
    @param          inData
                        The data for the property we want to set.
    @return         kAudioSessionNoError if the operation was successful.  If the property is a
                    read-only property or an invalid property value is passed in, 
                    kAudioSessionUnsupportedPropertyError will be returned.  Other error codes 
                    listed under AudioSession Error Constants also apply to 
                    this function.
 */
extern OSStatus
AudioSessionSetProperty(            AudioSessionPropertyID              inID,
                                    UInt32                              inDataSize,
                                    const void                          *inData)                    
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

/*!
    @function       AudioSessionGetPropertySize
    @abstract       Get the size of the payload for a property.
    @discussion     This function can be called to get the size for the payload of a property.
                    Valid properties are listed in an enum above.
    @param          inID
                        The AudioSessionPropertyID for which we want to get the size of the payload.
    @param          outDataSize
                        The size of the data payload will be copied here.
*/
extern OSStatus
AudioSessionGetPropertySize(        AudioSessionPropertyID              inID,
                                    UInt32                              *outDataSize)               
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

/*!
    @function       AudioSessionAddPropertyListener
    @abstract       Add a property listener.
    @discussion     This function can be used to add a listener to be called when a property changes.
                    If a listener and user data already exist for this property, they will be replaced.
                    Valid properties are listed above.
    @param          inID
                        The AudioSessionPropertyID for which we want to set a listener.
    @param          inProc
                        The listener to be called when the property changes.
    @param          inClientData
                        The client user data to use when calling the listener.
    @return         kAudioSessionNoError if the operation was successful.  If the property does
                    not support listeners, kAudioSessionUnsupportedPropertyError will be returned.  
                    Other error codes listed under AudioSession Error Constants also apply to 
                    this function.
 */
extern OSStatus
AudioSessionAddPropertyListener(    AudioSessionPropertyID              inID,
                                    AudioSessionPropertyListener        inProc,
                                    void                                *inClientData)              
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_0);

/*!
    @function       AudioSessionRemovePropertyListener
    @abstract       see AudioSessionRemovePropertyListenerWithUserData
    @discussion     see AudioSessionRemovePropertyListenerWithUserData
*/
extern OSStatus
AudioSessionRemovePropertyListener(	AudioSessionPropertyID          inID)                           
                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_NA,__MAC_NA,__IPHONE_2_0,__IPHONE_2_0);

/*!
    @function       AudioSessionRemovePropertyListener
    @abstract       Remove a property listener.
    @discussion     This function can be called to remove the listener for a property. The caller
                    provides the same proc and user data that was used to add the listener. This ensures
                    that there can be more than one listener established for a given property ID,
                    and each listener can be removed as requested.
                    Valid properties are listed above.
    @param          inID
                        The AudioSessionPropertyID for which we want to remove the listener.
    @param          inProc
                        The proc that was used to add the listener that needs to be removed.
    @param          inClientData
                        The client data that was used to add the listener that needs to be removed.
    @return         kAudioSessionNoError if the operation was successful.  If the property does
                    not support listeners, kAudioSessionUnsupportedPropertyError will be returned.  
                    Other error codes listed under AudioSession Error Constants also apply to 
                    this function.
*/
extern OSStatus
AudioSessionRemovePropertyListenerWithUserData(	AudioSessionPropertyID          inID,
                                                AudioSessionPropertyListener    inProc,
                                                void                            *inClientData)              
                                                            __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_2_1);

#pragma mark -
#pragma mark Deprecated

/*!
    @enum           AudioSession audio categories states
    @abstract       These two session categories are deprecated in iOS 3.0 or later
    @constant       kAudioSessionCategory_UserInterfaceSoundEffects
                        use kAudioSessionCategory_AmbientSound
    @constant       kAudioSessionCategory_LiveAudio 
                        use kAudioSessionCategory_MediaPlayback
*/
enum {
    kAudioSessionCategory_UserInterfaceSoundEffects  = 'uifx',
    kAudioSessionCategory_LiveAudio                  = 'live'
};

/*!
 @enum           AudioSession audio categories states
 @abstract       Deprecated AudioSession properties
 @constant       kAudioSessionProperty_AudioRoute 
 Deprecated in iOS 5.0; Use kAudioSessionProperty_AudioRouteDescription 
 */
enum {
    kAudioSessionProperty_AudioRoute                            = 'rout',   // CFStringRef      (get only)        
};

// deprecated dictionary keys
    
// Deprecated in iOS 5.0; Use kAudioSession_AudioRouteChangeKey_PreviousRouteDescription instead    
#define kAudioSession_AudioRouteChangeKey_OldRoute  "OutputDeviceDidChange_OldRoute"
//==================================================================================================
#endif //TARGET_OS_IPHONE

#ifdef __cplusplus
}
#endif

#endif /* __AudioServices_h__ */
