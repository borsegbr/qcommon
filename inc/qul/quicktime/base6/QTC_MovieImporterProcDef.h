/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieImporterProcDef
 *  Author:     Lincoln Yu
 *  Purpose:    Selector definition for QuickTime Movie-importer
 * == History ==        
 *	2006-02-09:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#include "./BasicMacros.h"
#include <qul/quicktime/base6/QTC_Base.h>


#define CALLCOMPONENT_BASENAME()	kQTC_EAT_BaseName

#define CREATE_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), CreateGlobal)
#define DESTROY_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), DestroyGlobal)

#define DECLARE_PROC(name)			CAT2(CALLCOMPONENT_BASENAME(), name)

#define RESULT_TYPE					ComponentResult
#define COMPLETE_NAME(name)			CAT2(CALLCOMPONENT_BASENAME(), name) (StorageHdl storage
#define D_COMMA()					,
#define CALL_PROC(name)				CAT2(QTC_MovieImporter_, name) ((**storage).pEmbody
#define C_COMMA()					,
#include "./procdef.h"

#if TARGET_CPU_68K
	#define COMPONENT_C_DISPATCHER
	#define COMPONENT_DISPATCH_MAIN
#endif

#define COMPONENT_DISPATCH_FILE		<qul/quicktime/base6/QTC_MovieImporterDispatch.h>
#define	GET_DELEGATE_COMPONENT()	((**storage).ciDelegate)
#define CALLCOMPONENT_GLOBALS() 	StorageHdl storage

#define MOVIEIMPORT_BASENAME()		CALLCOMPONENT_BASENAME()
#define	MOVIEIMPORT_GLOBALS()		CALLCOMPONENT_GLOBALS()

#define COMPONENT_UPP_SELECT_ROOT()	MovieImport

// Forward definition of core selectors
ComponentResult DECLARE_PROC(Target) (StorageHdl storage, ComponentInstance ciTarget);
ComponentResult DECLARE_PROC(CanDo) (StorageHdl storage, short ftnNumber);
ComponentResult DECLARE_PROC(Register) (StorageHdl storage);
ComponentResult DECLARE_PROC(Version) (StorageHdl storage);
ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf);
ComponentResult DECLARE_PROC(Open) (StorageHdl storage, ComponentInstance ciSelf);

#if defined (_TARGET_OS_MACH)
	#include <CoreServices/Components.k.h>
	#include <QuickTime/QuickTimeComponents.k.h>
	#include <QuickTime/ComponentDispatchHelper.c>
#elif defined (_TARGET_OS_WIN32)
	#include <Components.k.h>
	#include <QuickTimeComponents.k.h>
	#include <ComponentDispatchHelper.c>
#else
	#error "Unsupported target."
#endif

#include "./QTC_MovieImporter.h"

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget)

ComponentResult DECLARE_PROC(Register) (StorageHdl storage)						
	{
	if (storage == NULL)
		return (-1);		// Globals not properly set up, don't register.
	return QTC_MovieImporter_Register((**storage).pEmbody);
	}


DEFINE_PROC_0(Version)

// Place reserved for [:CanDo:] ...

ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf)
	{
	ComponentResult err = noErr;

	if (storage != NULL)
		{
		err = QTC_MovieImporter_Close((**storage).pEmbody, ciSelf);
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
	err = QTC_MovieImporter_Open((**storage).pEmbody, ciSelf);

	return (err);
	}

//////////////////////////////////////////////////////////////////////////
// Derived Movie-importer Range

// Place reserved for [:Handle:]
#ifdef _DEFINE_File
	DEFINE_PROC_8(File, const FSSpec*, pFile, Movie, movie, Track, targetTrack, 
		Track*,	pUsedTrack, TimeValue, time, TimeValue*, pDurationAdded, long, flagsIn, 
		long*, pFlagsOut)
#endif
// Place reserved for [:SetSampleDuration:]
// Place reserved for [:SetSampleDescription:]
// Place reserved for [:SetMediaFile:]
// Place reserved for [:SetDimensions:]
// Place reserved for [:SetChunkSize:]
// Place reserved for [:SetProgressProc:]
// Place reserved for [:SetAuxiliaryData:]
// Place reserved for [:SetFromScrap:]
#ifdef _DEFINE_DoUserDialog
	DEFINE_PROC_3(DoUserDialog, const FSSpec*, pFile, Handle, hData, Boolean*, pCanceled)
#endif
// Place reserved for [:SetDuration:]
// Place reserved for [:GetAuxiliaryDataType:]
#ifdef _DEFINE_Validate
	DEFINE_PROC_3(Validate, const FSSpec*, pFile, Handle, hData, Boolean*, pValid)
#endif
// Place reserved for [:GetFileType:]
#ifdef _DEFINE_DataRef
	DEFINE_PROC_9(DataRef, Handle, hDataRef, OSType, type, Movie, movie, 
		Track, targetTrack, Track*, pUsedTrack, TimeValue, time, 
		TimeValue*, pDurationAdded, long, flagsIn, long*, pFlagsOut)
#endif
// Place reserved for [:GetSampleDescription:]
#ifdef _DEFINE_GetMIMETypeList
	DEFINE_PROC_1(GetMIMETypeList, QTAtomContainer*, pMimeInfo)
#endif
// Place reserved for [:SetOffsetAndLimit:]
// Place reserved for [:GetSettingsAsAtomContainer:]
// Place reserved for [:SetSettingsFromAtomContainer:]
// Place reserved for [:SetOffsetAndLimit64:]
// Place reserved for [:Idle:]
#ifdef _DEFINE_ValidateDataRef
	DEFINE_PROC_3(ValidateDataRef, Handle, hDataRef, OSType, type, UInt8*, pValid)
#endif
// Place reserved for [:GetLoadState:]
// Place reserved for [:GetMaxLoadedTime:]
// Place reserved for [:EstimateCompletionTime:]
// Place reserved for [:SetDontBlock:]
// Place reserved for [:GetDontBlock:]
// Place reserved for [:SetIdleManager:]
// Place reserved for [:SetNewMovieFlags:]

