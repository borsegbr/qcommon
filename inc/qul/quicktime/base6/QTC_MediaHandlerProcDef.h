/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MediaHandlerProcDef
 *  Author:     Lincoln Yu
 *  Purpose:    Selector definition for QuickDraw Media-handler
 * == History ==        
 *	2006-01-25:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#include <stdio.h>

#include "./BasicMacros.h"
#include <qul/quicktime/base6/QTC_Base.h>


#define CALLCOMPONENT_BASENAME()	kQTC_MH_BaseName

#define CREATE_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), CreateGlobal)
#define DESTROY_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), DestroyGlobal)

#define DECLARE_PROC(name)			CAT2(CALLCOMPONENT_BASENAME(), name)

#define RESULT_TYPE					ComponentResult
#define COMPLETE_NAME(name)			CAT2(CALLCOMPONENT_BASENAME(), name) (StorageHdl storage
#define D_COMMA()					,
#define CALL_PROC(name)				CAT2(QTC_MediaHandler_, name) ((**storage).pEmbody
#define C_COMMA()					,
#include "./procdef.h"


#if TARGET_CPU_68K
	#define COMPONENT_C_DISPATCHER
	#define COMPONENT_DISPATCH_MAIN
#endif

#define COMPONENT_UPP_PREFIX()		uppMedia
#define COMPONENT_SELECT_PREFIX()  	kMedia
#define COMPONENT_DISPATCH_FILE		<qul/quicktime/base6/QTC_MediaHandlerDispatch.h>
#define	GET_DELEGATE_COMPONENT()	((**storage).ciDelegate)
#define	CALLCOMPONENT_GLOBALS()		StorageHdl storage

#define MEDIA_BASENAME() 			CALLCOMPONENT_BASENAME()
#define MEDIA_GLOBALS()				CALLCOMPONENT_GLOBALS()

// Forward definition of core selectors
ComponentResult DECLARE_PROC(Target) (StorageHdl storage, ComponentInstance ciTarget);
ComponentResult DECLARE_PROC(CanDo) (StorageHdl storage, short ftnNumber);
ComponentResult DECLARE_PROC(Register) (StorageHdl storage);
ComponentResult DECLARE_PROC(Version) (StorageHdl storage);
ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf);
ComponentResult DECLARE_PROC(Open) (StorageHdl storage, ComponentInstance ciSelf);

#ifdef _TARGET_OS_MACH
	#include <CoreServices/Components.k.h>
	#include <QuickTime/MediaHandlers.k.h>
	#include <QuickTime/ComponentDispatchHelper.c>
#elif defined (_TARGET_OS_WIN32)
	#include <Components.k.h>
	#include <MediaHandlers.k.h>
	#include <ComponentDispatchHelper.c>
#else
	#error "Unsupported target."
#endif


#include "./QTC_MediaHandler.h"


//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget)

ComponentResult DECLARE_PROC(Register) (StorageHdl storage)						
	{
	if (storage == NULL)
		return (-1);		// Globals not properly set up, don't register.

	return QTC_MediaHandler_Register((**storage).pEmbody);
	}

DEFINE_PROC_0(Version)

// Place reserved for [:CanDo:] ...

ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf)
	{
	ComponentResult err = noErr;

	if (storage != NULL)
		{
		err = QTC_MediaHandler_Close((**storage).pEmbody, ciSelf);
		if ((**storage).ciDelegate != NULL)
			{
			CloseComponent((**storage).ciDelegate);
			}
		DESTROY_GLOBAL(*storage);
		DisposePtr((Ptr)storage);
		}
	return (err);
	}

ComponentResult DECLARE_PROC(Open) (StorageHdl storage, ComponentInstance ciSelf)						
	{
	ComponentInstance	ciDelegate	= NULL;
	ComponentResult		err = noErr;

	// allocate the private global storage used by this component instance
	storage = (StorageHdl)NewPtr(sizeof(StoragePtr));
	*storage = (StoragePtr)CREATE_GLOBAL();
	SetComponentInstanceStorage(ciSelf, (Handle)storage);

	// actual opening process
	err = QTC_MediaHandler_Open((**storage).pEmbody, ciSelf);

	return (err);
	}


//////////////////////////////////////////////////////////////////////////
// Derived Media-handler Range

#ifdef _DEFINE_Initialize
	DEFINE_PROC_1(Initialize, GetMovieCompleteParams*, pGMC)
#endif

#ifdef _DEFINE_Idle			
	DEFINE_PROC_4(Idle, TimeValue, timeMedia, long, flagsIn, long*, pFlagsOut, 
		const TimeRecord*, pTimeMovie)
#endif

#ifdef _DEFINE_GetMediaInfo	
	DEFINE_PROC_1(GetMediaInfo, Handle, h)
#endif

// Place reserved for [:PutMediaInfo:] ...

#ifdef _DEFINE_SetActive	
	DEFINE_PROC_1(SetActive, Boolean, mediaEnable)
#endif

#ifdef _DEFINE_SetRate		
	DEFINE_PROC_1(SetRate, Fixed, rate)
#endif

// Place reserved for [:GGetStatus:] ...

#ifdef _DEFINE_TrackEdited	
	DEFINE_PROC_0(TrackEdited)
#endif

// Place reserved for [:SetMediaTimeScale:] ...
// Place reserved for [:SetMovieTimeScale:] ...

#ifdef _DEFINE_SetGWorld	
	DEFINE_PROC_2(SetGWorld, CGrafPtr, port, GDHandle, gd)
#endif

#ifdef _DEFINE_SetDimensions	
	DEFINE_PROC_2(SetDimensions, Fixed, width, Fixed, height)
#endif

// Place reserved for [:SetClip:] ...

#ifdef _DEFINE_SetMatrix	
	DEFINE_PROC_1(SetMatrix, MatrixRecord*, pMatrix)
#endif

// Place reserved for [:GetTrackOpaque:] ...
// Place reserved for [:SetGraphicsMode:] ...
// Place reserved for [:GetGraphicsMode:] ...
// Place reserved for [:GSetVolume:] ...
// Place reserved for [:SetSoundBalance:] ...
// Place reserved for [:GetSoundBalance:] ...
// Place reserved for [:GetNextBoundsChange:] ...
// Place reserved for [:GetSrcRgn:] ...
#ifdef _DEFINE_Preroll
	DEFINE_PROC_2(Preroll, TimeValue, time, Fixed, rate)
#endif
#ifdef _DEFINE_SampleDescriptionChanged	
	DEFINE_PROC_1(SampleDescriptionChanged, long, index)
#endif
#ifdef _DEFINE_HasCharacteristic
	DEFINE_PROC_2(HasCharacteristic, OSType, characteristic, Boolean*, pHasIt)
#endif
// Place reserved for [:HasCharacteristic:] ...
// Place reserved for [:GetOffscreenBufferSize:] ...
// Place reserved for [:SetHints:] ...
// Place reserved for [:GetName:] ...
// Place reserved for [:ForceUpdate:] ...
// Place reserved for [:GetDrawingRgn:] ...
// Place reserved for [:GSetActiveSegment:] ...
// Place reserved for [:InvalidateRegion:] ...
// Place reserved for [:GetNextStepTime:] ...
#ifdef _DEFINE_GetNextStepTime
	DEFINE_PROC_4(GetNextStepTime, short, flags, TimeValue, mediaTimeIn, TimeValue *, mediaTimeOut, Fixed, rate)
#endif
// Place reserved for [:SetNonPrimarySourceData:] ...
// Place reserved for [:ChangedNonPrimarySource:] ...
// Place reserved for [:TrackReferencesChanged:] ...
// Place reserved for [:GetSampleDataPointer:] ...
// Place reserved for [:ReleaseSampleDataPointer:] ...
// Place reserved for [:TrackPropertyAtomChanged:] ...
// Place reserved for [:SetTrackInputMapReference:] ...
// Place reserved for [:SetVideoParam:] ...
// Place reserved for [:GetVideoParam:] ...
// Place reserved for [:Compare:] ...
// Place reserved for [:GetClock:] ...
// Place reserved for [:SetSoundOutputComponent:] ...
// Place reserved for [:GetSoundOutputComponent:] ...
// Place reserved for [:SetSoundLocalizationData:] ...
// Place reserved for [:GetInvalidRegion:] ...

#if HANDLER_SWAPS_SAMPLE_DESC	
	DEFINE_PROC_1(SampleDescriptionB2N, SampleDescriptionHandle, hDesc)
	DEFINE_PROC_1(SampleDescriptionN2B, SampleDescriptionHandle, hDesc)
#endif

// Place reserved for [:QueueNonPrimarySourceData:] ...
// Place reserved for [:FlushNonPrimarySourceData:] ...
// Place reserved for [:GetURLLink:] ...
// Place reserved for [:MakeMediaTimeTable:] ...
// Place reserved for [:HitTestForTargetRefCon:] ...
// Place reserved for [:HitTestTargetRefCon:] ...
// Place reserved for [:GetActionsForQTEvent:] ...
// Place reserved for [:DisposeTargetRefCon:] ...
// Place reserved for [:TargetRefConsEqual:] ...
// Place reserved for [:SetActionsCallback:] ...
// Place reserved for [:PrePrerollBegin:] ...
// Place reserved for [:PrePrerollCancel:] ...
// Place reserved for [:EnterEmptyEdit:] ...
// Place reserved for [:CurrentMediaQueuedData:] ...
// Place reserved for [:GetEffectiveVolume:] ...
// Place reserved for [:ResolveTargetRefCon:] ...
// Place reserved for [:GetSoundLevelMeteringEnabled:] ...
// Place reserved for [:SetSoundLevelMeteringEnabled:] ...
// Place reserved for [:GetSoundLevelMeterInfo:] ...
// Place reserved for [:GetEffectiveSoundBalance:] ...
// Place reserved for [:SetScreenLock:] ...
// Place reserved for [:SetDoMCActionCallback:] ...
// Place reserved for [:GetErrorString:] ...
// Place reserved for [:GetSoundEqualizerBands:] ...
// Place reserved for [:SetSoundEqualizerBands:] ...
// Place reserved for [:GetSoundEqualizerBandLevels:] ...
// Place reserved for [:DoIdleActions:] ...
// Place reserved for [:SetSoundBassAndTreble:] ...


