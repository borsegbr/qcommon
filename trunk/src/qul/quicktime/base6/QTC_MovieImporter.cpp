#include "./QTC_MovieImporter.h"

#include <qul/quicktime/base6/BasicMacros.h>

#define RESULT_TYPE				QTC_Result
#define COMPLETE_NAME(name)		CAT2(QTC_MovieImporter_,name) (void *pUnk
#define D_COMMA()				,
#define CALL_PROC(name)			((qtc::MovieImporter *)pUnk)->name (
#define C_COMMA()				
#include <qul/quicktime/base6/procdef.h>

#define CONVERT()				((qtc::MovieImporter *)pUnk)

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget)
DEFINE_PROC_0(Register)
DEFINE_PROC_0(Version)
// Place reserved for [:CanDo:] ...
DEFINE_PROC_1(Close, ComponentInstance, ciSelf)
DEFINE_PROC_1(Open, ComponentInstance, ciSelf)

//////////////////////////////////////////////////////////////////////////
// Derived Movie-importer Range

// Place reserved for [:Handle:]
DEFINE_PROC_8(File, const FSSpec*, pFile, Movie, movie, Track, targetTrack, Track*, pUsedTrack,
			  TimeValue, time, TimeValue*, pDurationAdded, long, flagsIn, long*, pFlagsOut)
// Place reserved for [:SetSampleDuration:]
// Place reserved for [:SetSampleDescription:]
// Place reserved for [:SetMediaFile:]
// Place reserved for [:SetDimensions:]
// Place reserved for [:SetChunkSize:]
// Place reserved for [:SetProgressProc:]
// Place reserved for [:SetAuxiliaryData:]
// Place reserved for [:SetFromScrap:]
DEFINE_PROC_3(DoUserDialog, const FSSpec*, pFile, Handle, hData, Boolean*, pCanceled)
// Place reserved for [:SetDuration:]
// Place reserved for [:GetAuxiliaryDataType:]
DEFINE_PROC_3(Validate, const FSSpec*, pFile, Handle, hData, Boolean*, pValid)
// Place reserved for [:GetFileType:]
DEFINE_PROC_9(DataRef, Handle, hDataRef, OSType, type, Movie, movie, 
			  Track, targetTrack, Track*, pUsedTrack, TimeValue, time, 
			  TimeValue*, pDurationAdded, long, flagsIn, long*, pFlagsOut)
// Place reserved for [:GetSampleDescription:]
DEFINE_PROC_1(GetMIMETypeList, QTAtomContainer*, pMimeInfo)
// Place reserved for [:SetOffsetAndLimit:]
// Place reserved for [:GetSettingsAsAtomContainer:]
// Place reserved for [:SetSettingsFromAtomContainer:]
// Place reserved for [:SetOffsetAndLimit64:]
// Place reserved for [:Idle:]
DEFINE_PROC_3(ValidateDataRef, Handle, hDataRef, OSType, type, UInt8*, pValid)
// Place reserved for [:GetLoadState:]
// Place reserved for [:GetMaxLoadedTime:]
// Place reserved for [:EstimateCompletionTime:]
// Place reserved for [:SetDontBlock:]
// Place reserved for [:GetDontBlock:]
// Place reserved for [:SetIdleManager:]
// Place reserved for [:SetNewMovieFlags:]
// Place reserved for [:GetDestinationMediaType:]

