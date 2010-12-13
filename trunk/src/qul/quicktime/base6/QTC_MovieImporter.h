/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieImporter
 *  Author:     Lincoln Yu
 *  Purpose:    Base class of QuickTime Movie-importer object
 * == History ==        
 *	2006-02-09:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#ifndef	_QTC_MovieImporter_H
#define	_QTC_MovieImporter_H

#include <qul/quicktime/base6/QTC_Base.h>


#ifdef __cplusplus

_QTC_BEGIN

class MovieImporter : public BaseComponent
	{
public:

	MovieImporter(StoragePtr pStorage) : BaseComponent(pStorage) {}
	virtual ~MovieImporter() {}

// Core Selector Range

	virtual Result Target (ComponentInstance ciTarget) { return 0; }
	virtual Result Register () { return 0; }
	virtual Result Version () { return 0; }
	// Place reserved for [:CanDo:] ...
	virtual Result Close (ComponentInstance ciSelf) { return 0; }
	virtual Result Open (ComponentInstance ciSelf)  { return 0; }

// Derived Movie-importer Range

	// Place reserved for [:Handle:]

	virtual Result File (const FSSpec *pFile, Movie movie, 
		Track targetTrack, Track *pUsedTrack, TimeValue time, TimeValue *pDurationAdded,
		long flagsIn, long *pFlagsOut)
		{ return 0; }

	// Place reserved for [:SetSampleDuration:]
	// Place reserved for [:SetSampleDescription:]
	// Place reserved for [:SetMediaFile:]
	// Place reserved for [:SetDimensions:]
	// Place reserved for [:SetChunkSize:]
	// Place reserved for [:SetProgressProc:]
	// Place reserved for [:SetAuxiliaryData:]
	// Place reserved for [:SetFromScrap:]

	virtual Result DoUserDialog (const FSSpec *pFile, Handle hData, Boolean *pCanceled)
		{ return 0; }

	// Place reserved for [:SetDuration:]
	// Place reserved for [:GetAuxiliaryDataType:]

	virtual Result Validate (const FSSpec *pFile, Handle hData, Boolean *pValid)
		{ return 0; }

	// Place reserved for [:GetFileType:]

	virtual Result DataRef (Handle hDataRef, OSType type, Movie movie, Track targetTrack, 
		Track *pUsedTrack, TimeValue time, TimeValue *pDurationAdded, long flagsIn, 
		long *pFlagsOut)
		{ return 0; }

	// Place reserved for [:GetSampleDescription:]

	virtual Result GetMIMETypeList (QTAtomContainer *pMimeInfo)
		{ return 0; }

	// Place reserved for [:SetOffsetAndLimit:]
	// Place reserved for [:GetSettingsAsAtomContainer:]
	// Place reserved for [:SetSettingsFromAtomContainer:]
	// Place reserved for [:SetOffsetAndLimit64:]
	// Place reserved for [:Idle:]

	virtual Result ValidateDataRef (Handle hDataRef, OSType type, UInt8 *pValid)
		{ return 0; }

	// Place reserved for [:GetLoadState:]
	// Place reserved for [:GetMaxLoadedTime:]
	// Place reserved for [:EstimateCompletionTime:]
	// Place reserved for [:SetDontBlock:]
	// Place reserved for [:GetDontBlock:]
	// Place reserved for [:SetIdleManager:]
	// Place reserved for [:SetNewMovieFlags:]
	// Place reserved for [:GetDestinationMediaType:]

	};	// class MovieImporter

_QTC_END

#endif	// ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
// Core Selector Range

QTC_Result QTC_MovieImporter_Target (void *pUnk, ComponentInstance ciTarget);
QTC_Result QTC_MovieImporter_Register (void *pUnk);
QTC_Result QTC_MovieImporter_Version (void *pUnk);
// Place reserved for [:CanDo:] ...
QTC_Result QTC_MovieImporter_Close (void *pUnk, ComponentInstance ciSelf);
QTC_Result QTC_MovieImporter_Open (void *pUnk, ComponentInstance ciSelf);

//////////////////////////////////////////////////////////////////////////
// Derived Movie-importer Range

// Place reserved for [:Handle:]
QTC_Result QTC_MovieImporter_File (void *pUnk, const FSSpec *pFile, Movie movie, 
								   Track targetTrack, Track *pUsedTrack, 
								   TimeValue time, TimeValue *pDurationAdded,
								   long flagsIn, long *pFlagsOut);
// Place reserved for [:SetSampleDuration:]
// Place reserved for [:SetSampleDescription:]
// Place reserved for [:SetMediaFile:]
// Place reserved for [:SetDimensions:]
// Place reserved for [:SetChunkSize:]
// Place reserved for [:SetProgressProc:]
// Place reserved for [:SetAuxiliaryData:]
// Place reserved for [:SetFromScrap:]
QTC_Result QTC_MovieImporter_DoUserDialog (void *pUnk, const FSSpec *pFile, Handle hData, 
										   Boolean *pCanceled);
// Place reserved for [:SetDuration:]
// Place reserved for [:GetAuxiliaryDataType:]
QTC_Result QTC_MovieImporter_Validate (void *pUnk, const FSSpec *pFile, Handle hData, 
									   Boolean *pValid);
// Place reserved for [:GetFileType:]
QTC_Result QTC_MovieImporter_DataRef (void *pUnk, Handle hDataRef, OSType type, Movie movie, 
									  Track targetTrack, Track *pUsedTrack, TimeValue time, 
									  TimeValue *pDurationAdded, long flagsIn, long *pFlagsOut);
// Place reserved for [:GetSampleDescription:]
QTC_Result QTC_MovieImporter_GetMIMETypeList (void *pUnk, QTAtomContainer *pMimeInfo);
// Place reserved for [:SetOffsetAndLimit:]
// Place reserved for [:GetSettingsAsAtomContainer:]
// Place reserved for [:SetSettingsFromAtomContainer:]
// Place reserved for [:SetOffsetAndLimit64:]
// Place reserved for [:Idle:]
QTC_Result QTC_MovieImporter_ValidateDataRef (void *pUnk, Handle hDataRef, OSType type, 
											  UInt8 *pValid);
// Place reserved for [:GetLoadState:]
// Place reserved for [:GetMaxLoadedTime:]
// Place reserved for [:EstimateCompletionTime:]
// Place reserved for [:SetDontBlock:]
// Place reserved for [:GetDontBlock:]
// Place reserved for [:SetIdleManager:]
// Place reserved for [:SetNewMovieFlags:]
// Place reserved for [:GetDestinationMediaType:]

#ifdef __cplusplus
}
#endif

#endif	// ifndef _QTC_MovieImporter_H
