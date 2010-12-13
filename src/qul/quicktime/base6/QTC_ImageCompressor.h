/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_ImageCompressor
 *  Author:     Lincoln Yu
 *  Purpose:    Base class of QuickDraw Image-compressor object
 * == History ==        
 *	2006-03-04:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#ifndef	_QTC_ImageCompressor_H
#define	_QTC_ImageCompressor_H

#include <qul/quicktime/base6/QTC_Base.h>


#ifdef __cplusplus

_QTC_BEGIN

class ImageCompressor : public BaseComponent
	{
public:
	ImageCompressor (StoragePtr pStorage) : BaseComponent(pStorage) {}
	~ImageCompressor () {}

// Core Selector Range

	// Place reserved for [:GetMPWorkFunction:]
	// Place reserved for [:Unregister:]
	virtual Result Target (ComponentInstance ciTarget) { return 0; }
	virtual Result Register () { return 0; }
	virtual Result Version () { return 0; }
	// Place reserved for [:CanDo:] ...
	virtual Result Close (ComponentInstance ciSelf) { return 0; }
	virtual Result Open (ComponentInstance ciSelf)  { return 0; }

// Derived Image-compressor Range
	virtual Result GetCodecInfo (CodecInfo *pInfo) { return 0; }
	virtual Result GetCompressionTime (PixMapHandle src, const Rect *pSrcRect, short depth, 
		CodecQ *pSpatialQuality, CodecQ *pTemporalQuality, unsigned long *pTime) 
		{ return 0; }
	virtual Result GetMaxCompressionSize (PixMapHandle src, const Rect *pSrcRect, short depth,
		CodecQ quality, long *pSize) 
		{ return 0; }
	virtual Result PreCompress (CodecCompressParams *pParams) { return 0; }
	virtual Result BandCompress (CodecCompressParams *pParams) { return 0; }
	virtual Result PreDecompress (CodecDecompressParams *pParams) { return 0; }
	virtual Result BandDecompress (CodecDecompressParams *pParams) { return 0; }

	// Place reserved for [:Busy:]
	virtual Result GetCompressedImageSize (ImageDescriptionHandle hDesc, Ptr pData, long bufferSize,
		ICMDataProcRecordPtr dataProc, long *pDataSize)
		{ return 0; }
	virtual Result GetSimilarity (PixMapHandle src, const Rect *pSrcRect, ImageDescriptionHandle hDesc,
		Ptr pData, Fixed *pSimilarity)
		{ return 0; }
	// Place reserved for [:TrimImage:]
	// Place reserved for [:RequestSettings:]
	// Place reserved for [:GetSettings:]
	// Place reserved for [:SetSettings:]
	// Place reserved for [:Flush:]
	// Place reserved for [:SetTimeCode:]
	// Place reserved for [:IsImageDescriptionEquivalent:]
	// Place reserved for [:NewMemory:]
	// Place reserved for [:DisposeMemory:]
	// Place reserved for [:HitTestData:]
	// Place reserved for [:NewImageBufferMemory:]
	// Place reserved for [:ExtractAndCombineFields:]
	// Place reserved for [:GetMaxCompressionSizeWithSources:]
	// Place reserved for [:SetTimeBase:]
	// Place reserved for [:SourceChanged:]
	// Place reserved for [:FlushLastFrame:]
	// Place reserved for [:GetSettingsAsText:]
	// Place reserved for [:GetParameterListHandle:]
	// Place reserved for [:GetParameterList:]
	// Place reserved for [:CreateStandardParameterDialog:]
	// Place reserved for [:IsStandardParameterDialogEvent:]
	// Place reserved for [:DismissStandardParameterDialog:]
	// Place reserved for [:StandardParameterDialogDoAction:]
	// Place reserved for [:NewImageGWorld:]
	// Place reserved for [:DisposeImageGWorld:]
	// Place reserved for [:HitTestDataWithFlags:]
	// Place reserved for [:ValidateParameters:]
	// Place reserved for [:GetBaseMPWorkFunction:]
	// Place reserved for [:0x0026:]
	// Place reserved for [:0x0027:]
	// Place reserved for [:RequestGammaLevel:]
	// Place reserved for [:GetSourceDataGammaLevel:]
	// Place reserved for [:0x002A:]
	// Place reserved for [:GetDecompressLatency:]
	// Place reserved for [:MergeFloatingImageOntoWindow:]
	// Place reserved for [:RemoveFloatingImage:]
	// Place reserved for [:GetDITLForSize:]
	// Place reserved for [:DITLInstall:]
	// Place reserved for [:DITLEvent:]
	// Place reserved for [:DITLItem:]
	// Place reserved for [:DITLRemove:]
	// Place reserved for [:DITLValidateInput:]
	virtual Result Preflight (CodecDecompressParams *pParams) { return 0; }
	virtual Result Initialize (ImageSubCodecDecompressCapabilities *pCap) { return 0; }
	virtual Result BeginBand (CodecDecompressParams *pParams, ImageSubCodecDecompressRecord *pDR, long flags) 
		{ return 0; }
	virtual Result DrawBand (ImageSubCodecDecompressRecord *pDR) { return 0; }
	virtual Result EndBand (ImageSubCodecDecompressRecord *pDR, OSErr result, long flags) { return 0; }
	virtual Result QueueStarting () { return 0; }
	virtual Result QueueStopping () { return 0; }
	// Place reserved for [:DroppingFrame:]
	// Place reserved for [:ScheduleFrame:]
	// Place reserved for [:CancelTrigger:]

	};	// class ImageCompressor

_QTC_END

#endif	// ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

// Core Selector Range

QTC_Result QTC_ImageCompressor_Target (void *pUnk, ComponentInstance ciTarget);
QTC_Result QTC_ImageCompressor_Register (void *pUnk);
QTC_Result QTC_ImageCompressor_Version (void *pUnk);
// Place reserved for [:CanDo:] ...
QTC_Result QTC_ImageCompressor_Close (void *pUnk, ComponentInstance ciSelf);
QTC_Result QTC_ImageCompressor_Open (void *pUnk, ComponentInstance ciSelf);

// Derived Image-compressor Range

QTC_Result QTC_ImageCompressor_GetCodecInfo (void *pUnk, CodecInfo *pInfo);
QTC_Result QTC_ImageCompressor_GetCompressionTime (void *pUnk, PixMapHandle src, const Rect *pSrcRect, short depth, 
												   CodecQ *pSpatialQuality, CodecQ *pTemporalQuality, unsigned long *pTime);
QTC_Result QTC_ImageCompressor_GetMaxCompressionSize (void *pUnk, PixMapHandle src, const Rect *pSrcRect, short depth,
													  CodecQ quality, long *pSize);
QTC_Result QTC_ImageCompressor_PreCompress (void *pUnk, CodecCompressParams *pParams);
QTC_Result QTC_ImageCompressor_BandCompress (void *pUnk, CodecCompressParams *pParams);
QTC_Result QTC_ImageCompressor_PreDecompress (void *pUnk, CodecDecompressParams *pParams);
QTC_Result QTC_ImageCompressor_BandDecompress (void *pUnk, CodecDecompressParams *pParams);
// Place reserved for [:busy:]
QTC_Result QTC_ImageCompressor_GetCompressedImageSize (void *pUnk, ImageDescriptionHandle hDesc, Ptr pData, long bufferSize,
													   ICMDataProcRecordPtr dataProc, long *pDataSize);
QTC_Result QTC_ImageCompressor_GetSimilarity (void *pUnk, PixMapHandle src, const Rect *pSrcRect, ImageDescriptionHandle hDesc,
											  Ptr pData, Fixed *pSimilarity);
// Place reserved for [:TrimImage:]
// Place reserved for [:RequestSettings:]
// Place reserved for [:GetSettings:]
// Place reserved for [:SetSettings:]
// Place reserved for [:Flush:]
// Place reserved for [:SetTimeCode:]
// Place reserved for [:IsImageDescriptionEquivalent:]
// Place reserved for [:NewMemory:]
// Place reserved for [:DisposeMemory:]
// Place reserved for [:HitTestData:]
// Place reserved for [:NewImageBufferMemory:]
// Place reserved for [:ExtractAndCombineFields:]
// Place reserved for [:GetMaxCompressionSizeWithSources:]
// Place reserved for [:SetTimeBase:]
// Place reserved for [:SourceChanged:]
// Place reserved for [:FlushLastFrame:]
// Place reserved for [:GetSettingsAsText:]
// Place reserved for [:GetParameterListHandle:]
// Place reserved for [:GetParameterList:]
// Place reserved for [:CreateStandardParameterDialog:]
// Place reserved for [:IsStandardParameterDialogEvent:]
// Place reserved for [:DismissStandardParameterDialog:]
// Place reserved for [:StandardParameterDialogDoAction:]
// Place reserved for [:NewImageGWorld:]
// Place reserved for [:DisposeImageGWorld:]
// Place reserved for [:HitTestDataWithFlags:]
// Place reserved for [:ValidateParameters:]
// Place reserved for [:GetBaseMPWorkFunction:]
// Place reserved for [:0x0026:]
// Place reserved for [:0x0027:]
// Place reserved for [:RequestGammaLevel:]
// Place reserved for [:GetSourceDataGammaLevel:]
// Place reserved for [:0x002A:]
// Place reserved for [:GetDecompressLatency:]
// Place reserved for [:MergeFloatingImageOntoWindow:]
// Place reserved for [:RemoveFloatingImage:]
// Place reserved for [:GetDITLForSize:]
// Place reserved for [:DITLInstall:]
// Place reserved for [:DITLEvent:]
// Place reserved for [:DITLItem:]
// Place reserved for [:DITLRemove:]
// Place reserved for [:DITLValidateInput:]
QTC_Result QTC_ImageCompressor_Preflight (void *pUnk, CodecDecompressParams *pParams);
QTC_Result QTC_ImageCompressor_Initialize (void *pUnk, ImageSubCodecDecompressCapabilities *pCap);
QTC_Result QTC_ImageCompressor_BeginBand (void *pUnk, CodecDecompressParams *pParams, ImageSubCodecDecompressRecord *pDR, long flags);
QTC_Result QTC_ImageCompressor_DrawBand (void *pUnk, ImageSubCodecDecompressRecord *pDR);
QTC_Result QTC_ImageCompressor_EndBand (void *pUnk, ImageSubCodecDecompressRecord *pDR, OSErr result, long flags);
QTC_Result QTC_ImageCompressor_QueueStarting (void *pUnk);
QTC_Result QTC_ImageCompressor_QueueStopping (void *pUnk);
// Place reserved for [:DroppingFrame:]
// Place reserved for [:ScheduleFrame:]
// Place reserved for [:CancelTrigger:]

#ifdef __cplusplus
}
#endif

#endif	// ifndef _QTC_ImageCompressor_H
