/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieExporterProcDef
 *  Author:     Lincoln Yu
 *  Purpose:    Selector definition for QuickTime Movie-exporter
 * == History ==        
 *	2006-02-09:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#include "./BasicMacros.h"
#include <qul/quicktime/base6/QTC_Base.h>


#define CALLCOMPONENT_BASENAME()	kQTC_SPIT_BaseName

#define CREATE_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), CreateGlobal)
#define DESTROY_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), DestroyGlobal)

#define DECLARE_PROC(name)			CAT2(CALLCOMPONENT_BASENAME(), name)

#define RESULT_TYPE					ComponentResult
#define COMPLETE_NAME(name)			CAT2(CALLCOMPONENT_BASENAME(), name) (StorageHdl storage
#define D_COMMA()					,
#define CALL_PROC(name)				CAT2(QTC_MovieExporter_, name) ((**storage).pEmbody
#define C_COMMA()					,
#include "./procdef.h"

#if TARGET_CPU_68K
	#define COMPONENT_C_DISPATCHER
	#define COMPONENT_DISPATCH_MAIN
#endif

#define COMPONENT_DISPATCH_FILE		<qul/quicktime/base6/QTC_MovieExporterDispatch.h>
#define	GET_DELEGATE_COMPONENT()	((**storage).ciDelegate)
#define CALLCOMPONENT_GLOBALS() 	StorageHdl storage

#define MOVIEEXPORT_BASENAME()		CALLCOMPONENT_BASENAME()
#define	MOVIEEXPORT_GLOBALS()		CALLCOMPONENT_GLOBALS()

#define COMPONENT_UPP_SELECT_ROOT()	MovieExport

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

#include "./QTC_MovieExporter.h"

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget)

ComponentResult DECLARE_PROC(Register) (StorageHdl storage)						
	{
	if (storage == NULL)
		return (-1);		// Globals not properly set up, don't register.
	return QTC_MovieExporter_Register((**storage).pEmbody);
	}


DEFINE_PROC_0(Version)

// Place reserved for [:CanDo:] ...

ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf)
	{
	ComponentResult err = noErr;

	if (storage != NULL)
		{
		err = QTC_MovieExporter_Close((**storage).pEmbody, ciSelf);
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
	err = QTC_MovieExporter_Open((**storage).pEmbody, ciSelf);

	return (err);
	}

//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (1)

// Place reserved for [:ToHandle:]
#ifdef _DEFINE_ToFile
	DEFINE_PROC_5(ToFile, const FSSpec*, pFile, Movie, movie, Track, onlyThisTrack,
        TimeValue, startTime, TimeValue, duration)
#endif
// Place reserved for indicator [:130:]
// Place reserved for [:GetAuxiliaryData:]
#ifdef _DEFINE_SetProgressProc
	DEFINE_PROC_2(SetProgressProc, MovieProgressUPP, proc, long, refCon)
#endif
// Place reserved for [:SetSampleDescription:]
#ifdef _DEFINE_DoUserDialog
	DEFINE_PROC_5(DoUserDialog, Movie, movie, Track, onlyThisTrack, TimeValue, startTime, 
		TimeValue, duration, Boolean*, pCanceled)
#endif
// Place reserved for [:GetCreatorType:]
#ifdef _DEFINE_ToDataRef
	DEFINE_PROC_6(ToDataRef, Handle, hDataRef, OSType, type, Movie, movie, Track, onlyThisTrack, 
		TimeValue, startTime, TimeValue, duration)
#endif
#ifdef _DEFINE_FromProceduresToDataRef
	DEFINE_PROC_2(FromProceduresToDataRef, Handle, hDataRef, OSType, type)
#endif
#ifdef _DEFINE_AddDataSource
	DEFINE_PROC_6(AddDataSource, OSType, trackType, TimeScale, scale, long*, pTrackID, 
		MovieExportGetPropertyUPP, getPropertyProc, MovieExportGetDataUPP, getDataProc,
		void*, pRefCon)
#endif
#ifdef _DEFINE_Validate
	DEFINE_PROC_3(Validate, Movie, movie, Track, onlyThisTrack, Boolean*, pValid)
#endif
#ifdef _DEFINE_GetSettingsAsAtomContainer
	DEFINE_PROC_1(GetSettingsAsAtomContainer, QTAtomContainer*, pSettings)
#endif
#ifdef _DEFINE_SetSettingsFromAtomContainer
	DEFINE_PROC_1(SetSettingsFromAtomContainer, QTAtomContainer, settings)
#endif
#ifdef _DEFINE_GetFileNameExtension
	DEFINE_PROC_1(GetFileNameExtension, OSType*, pExtension)
#endif
#ifdef _DEFINE_GetShortFileTypeString
	DEFINE_PROC_1(GetShortFileTypeString, Str255, strType)
#endif
#ifdef _DEFINE_GetSourceMediaType
	DEFINE_PROC_1(GetSourceMediaType, OSType*, pMediaType)
#endif
// Place reserved for [:SetGetMoviePropertyProc:]


//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (2)

// Place reserved for [:NewGetDataAndPropertiesProcs:]
// Place reserved for [:DisposeGetDataAndPropertiesProcs:]



