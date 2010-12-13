#include "./QTC_MovieExporter.h"

#include <qul/quicktime/base6/BasicMacros.h>

#define RESULT_TYPE				QTC_Result
#define COMPLETE_NAME(name)		CAT2(QTC_MovieExporter_,name) (void *pUnk
#define D_COMMA()				,
#define CALL_PROC(name)			((qtc::MovieExporter *)pUnk)->name (
#define C_COMMA()				
#include <qul/quicktime/base6/procdef.h>

#define CONVERT()				((qtc::MovieExporter *)pUnk)

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget);
DEFINE_PROC_0(Register)
DEFINE_PROC_0(Version)
// Place reserved for [:CanDo:] ...
DEFINE_PROC_1(Close, ComponentInstance, ciSelf);
DEFINE_PROC_1(Open, ComponentInstance, ciSelf);

//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (1)

// Place reserved for [:ToHandle:]
DEFINE_PROC_5(ToFile, const FSSpec*, pFile, Movie, movie, Track, onlyThisTrack, 
			  TimeValue, startTime, TimeValue, duration)
// Place reserved for indicator [:130:]
// Place reserved for [:GetAuxiliaryData:]
DEFINE_PROC_2(SetProgressProc, MovieProgressUPP, proc, long, refCon)
// Place reserved for [:SetSampleDescription:]
DEFINE_PROC_5(DoUserDialog, Movie, movie, Track, onlyThisTrack, TimeValue, startTime, 
			  TimeValue, duration, Boolean*, pCanceled)
// Place reserved for [:GetCreatorType:]
DEFINE_PROC_6(ToDataRef, Handle, hDataRef, OSType, type, Movie, movie, Track, onlyThisTrack, 
              TimeValue, startTime, TimeValue, duration)
DEFINE_PROC_2(FromProceduresToDataRef, Handle, hDataRef, OSType, type)
DEFINE_PROC_6(AddDataSource, OSType, trackType, TimeScale, scale, long*, pTrackID, 
              MovieExportGetPropertyUPP, getPropertyProc, MovieExportGetDataUPP, getDataProc,
			  void*, pRefCon)
DEFINE_PROC_3(Validate, Movie, movie, Track, onlyThisTrack, Boolean*, pValid)
DEFINE_PROC_1(GetSettingsAsAtomContainer, QTAtomContainer*, pSettings)
DEFINE_PROC_1(SetSettingsFromAtomContainer, QTAtomContainer, settings)
DEFINE_PROC_1(GetFileNameExtension, OSType*, pExtension)
DEFINE_PROC_1(GetShortFileTypeString, Str255, strType)
DEFINE_PROC_1(GetSourceMediaType, OSType*, pMediaType)
// Place reserved for [:SetGetMoviePropertyProc:]


//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (2)

// Place reserved for [:NewGetDataAndPropertiesProcs:]
// Place reserved for [:DisposeGetDataAndPropertiesProcs:]

