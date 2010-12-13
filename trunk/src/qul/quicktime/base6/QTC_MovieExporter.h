/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieExporter
 *  Author:     Lincoln Yu
 *  Purpose:    Base class of QuickTime Movie-exporter object
 * == History ==        
 *	2006-02-11:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#ifndef	_QTC_MovieExporter_H
#define	_QTC_MovieExporter_H

#include <qul/quicktime/base6/QTC_Base.h>


#ifdef __cplusplus

_QTC_BEGIN

class MovieExporter : public BaseComponent
	{
public:

	MovieExporter(StoragePtr pStorage) : BaseComponent(pStorage) {}
	virtual ~MovieExporter() {}

// Core Selector Range

	virtual Result Target (ComponentInstance ciTarget) { return 0; }
	virtual Result Register () { return 0; }
	virtual Result Version () { return 0; }
	// Place reserved for [:CanDo:] ...
	virtual Result Close (ComponentInstance ciSelf) { return 0; }
	virtual Result Open (ComponentInstance ciSelf)  { return 0; }

// Derived Movie-exporter Range (1)

	// Place reserved for [:ToHandle:]
	virtual Result ToFile(const FSSpec *pFile, Movie movie, Track onlyThisTrack, 
		TimeValue startTime, TimeValue duration)
		{ return 0; }
	// Place reserved for indicator [:130:]
	// Place reserved for [:GetAuxiliaryData:]
	virtual Result SetProgressProc(MovieProgressUPP proc, long refCon)
		{ return 0; }
	// Place reserved for [:SetSampleDescription:]
	virtual Result DoUserDialog(Movie movie, Track onlyThisTrack,
		TimeValue startTime, TimeValue duration, Boolean *pCanceled)
		{ return 0; }
	// Place reserved for [:GetCreatorType:]
	virtual Result ToDataRef(Handle hDataRef, OSType type,
		Movie movie, Track onlyThisTrack, TimeValue startTime, TimeValue duration)
		{ return 0; }
	virtual Result FromProceduresToDataRef(Handle hDataRef, OSType type)
		{ return 0; }
	virtual Result AddDataSource(OSType trackType, TimeScale scale,
		long *pTrackID, MovieExportGetPropertyUPP getPropertyProc,
		MovieExportGetDataUPP getDataProc, void *pRefCon)
		{ return 0; }
	virtual Result Validate(Movie movie, Track onlyThisTrack, Boolean *pValid)
		{ return 0; }
	virtual Result GetSettingsAsAtomContainer(QTAtomContainer *pSettings)
		{ return 0; }
	virtual Result SetSettingsFromAtomContainer(QTAtomContainer settings)
		{ return 0; }
	virtual Result GetFileNameExtension(OSType *pExtension)
		{ return 0; }
	virtual Result GetShortFileTypeString(Str255 strType)
		{ return 0; }
	virtual Result GetSourceMediaType(OSType *pMediaType)
		{ return 0; }
	// Place reserved for [:SetGetMoviePropertyProc:]

// Derived Movie-exporter Range (2)

	// Place reserved for [:NewGetDataAndPropertiesProcs:]
	// Place reserved for [:DisposeGetDataAndPropertiesProcs:]


	};	// class MovieExporter

_QTC_END

#endif	// ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

QTC_Result QTC_MovieExporter_Target (void *pUnk, ComponentInstance ciTarget);
QTC_Result QTC_MovieExporter_Register (void *pUnk);
QTC_Result QTC_MovieExporter_Version (void *pUnk);
// Place reserved for [:CanDo:] ...
QTC_Result QTC_MovieExporter_Close (void *pUnk, ComponentInstance ciSelf);
QTC_Result QTC_MovieExporter_Open (void *pUnk, ComponentInstance ciSelf);

//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (1)

// Place reserved for [:ToHandle:]
QTC_Result QTC_MovieExporter_ToFile(void *pUnk, const FSSpec *pFile,
						  Movie movie, Track onlyThisTrack, TimeValue startTime,
						  TimeValue duration);
// Place reserved for indicator [:130:]
// Place reserved for [:GetAuxiliaryData:]
QTC_Result QTC_MovieExporter_SetProgressProc(void *pUnk, MovieProgressUPP proc, long refCon);
// Place reserved for [:SetSampleDescription:]
QTC_Result QTC_MovieExporter_DoUserDialog(void *pUnk, Movie movie, Track onlyThisTrack,
										  TimeValue startTime, TimeValue duration, 
										  Boolean *pCanceled);
// Place reserved for [:GetCreatorType:]
QTC_Result QTC_MovieExporter_ToDataRef(void *pUnk, Handle hDataRef, OSType type, 
									   Movie movie, Track onlyThisTrack, 
									   TimeValue startTime, TimeValue duration);
QTC_Result QTC_MovieExporter_FromProceduresToDataRef(void *pUnk, Handle hDataRef, OSType type);
QTC_Result QTC_MovieExporter_AddDataSource(void *pUnk, OSType trackType, TimeScale scale, 
										   long *pTrackID, 
										   MovieExportGetPropertyUPP getPropertyProc,
                                           MovieExportGetDataUPP getDataProc, void *pRefCon);
QTC_Result QTC_MovieExporter_Validate(void *pUnk, Movie movie, Track onlyThisTrack, 
									  Boolean *pValid);
QTC_Result QTC_MovieExporter_GetSettingsAsAtomContainer(void *pUnk, QTAtomContainer *pSettings);
QTC_Result QTC_MovieExporter_SetSettingsFromAtomContainer(void *pUnk, QTAtomContainer settings);
QTC_Result QTC_MovieExporter_GetFileNameExtension(void *pUnk, OSType *pExtension);
QTC_Result QTC_MovieExporter_GetShortFileTypeString(void *pUnk, Str255 strType);
QTC_Result QTC_MovieExporter_GetSourceMediaType(void *pUnk, OSType *pMediaType);
// Place reserved for [:SetGetMoviePropertyProc:]


//////////////////////////////////////////////////////////////////////////
// Derived Movie-exporter Range (2)

// Place reserved for [:NewGetDataAndPropertiesProcs:]
// Place reserved for [:DisposeGetDataAndPropertiesProcs:]

#ifdef __cplusplus
}
#endif

#endif	// ifndef _QTC_MovieExporter_H
