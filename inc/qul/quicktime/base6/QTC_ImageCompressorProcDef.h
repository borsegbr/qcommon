/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_ImageCompressorProcDef
 *  Author:     Lincoln Yu
 *  Purpose:    Selector definition for QuickTime Image-compressor
 * == History ==        
 *	2006-03-04:	Created.
 *	2006-06-21:	Selector [:DecodeBand:] added.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */


#include "./BasicMacros.h"
#include <qul/quicktime/base6/QTC_Base.h>

#define CALLCOMPONENT_BASENAME()	kQTC_IMC_BaseName

#define CREATE_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), CreateGlobal)
#define DESTROY_GLOBAL				CAT2(CALLCOMPONENT_BASENAME(), DestroyGlobal)

#define DECLARE_PROC(name)			CAT2(CALLCOMPONENT_BASENAME(), name)

#define RESULT_TYPE					ComponentResult
#define COMPLETE_NAME(name)			CAT2(CALLCOMPONENT_BASENAME(), name) (StorageHdl storage
#define D_COMMA()					,
#define CALL_PROC(name)				CAT2(QTC_ImageCompressor_, name) ((**storage).pEmbody
#define C_COMMA()					,
#include "./procdef.h"

#if TARGET_CPU_68K
	#define COMPONENT_C_DISPATCHER
	#define COMPONENT_DISPATCH_MAIN
#endif

#define COMPONENT_UPP_PREFIX()		uppImageCodec
#define COMPONENT_SELECT_PREFIX()  	kImageCodec
#define COMPONENT_DISPATCH_FILE		<qul/quicktime/base6/QTC_ImageCompressorDispatch.h>
#define	GET_DELEGATE_COMPONENT()	((**storage).ciDelegate)
#define	CALLCOMPONENT_GLOBALS()		StorageHdl storage

#define IMAGECODEC_BASENAME() 		CALLCOMPONENT_BASENAME()
#define IMAGECODEC_GLOBALS()		CALLCOMPONENT_GLOBALS()

// Forward definition of core selectors
ComponentResult DECLARE_PROC(Target) (StorageHdl storage, ComponentInstance ciTarget);
ComponentResult DECLARE_PROC(CanDo) (StorageHdl storage, short ftnNumber);
ComponentResult DECLARE_PROC(Register) (StorageHdl storage);
ComponentResult DECLARE_PROC(Version) (StorageHdl storage);
ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf);
ComponentResult DECLARE_PROC(Open) (StorageHdl storage, ComponentInstance ciSelf);

#ifdef _TARGET_OS_MACH
	#include <CoreServices/Components.k.h>
	#include <QuickTime/ImageCodec.k.h>
	#include <QuickTime/ComponentDispatchHelper.c>
#elif defined (_TARGET_OS_WIN32)
	#include <Components.k.h>
	#include <ImageCodec.k.h>
	#include <ComponentDispatchHelper.c>
#else
	#error "Unsupported target."
#endif


#include "./QTC_ImageCompressor.h"


//////////////////////////////////////////////////////////////////////////
// Core Selector Range

#ifndef _TARGET_OS_MAC68K
	#ifdef _DEFINE_GetMPWorkFunction
		DEFINE_PROC_2(GetMPWorkFunction, ComponentMPWorkFunctionUPP*, pWorkFunction, void**, ppRefCon)
	#endif
	// Place reserved for [:Unregister:]
#endif


DEFINE_PROC_1(Target, ComponentInstance, ciTarget)

ComponentResult DECLARE_PROC(Register) (StorageHdl storage)						
	{
	if (storage == NULL)
		return (-1);		// Globals not properly set up, don't register.

	return QTC_ImageCompressor_Register((**storage).pEmbody);
	}

DEFINE_PROC_0(Version)

// Place reserved for [:CanDo:] ...

ComponentResult DECLARE_PROC(Close) (StorageHdl storage, ComponentInstance ciSelf)
	{
	ComponentResult err = noErr;

	if (storage != NULL)
		{
		err = QTC_ImageCompressor_Close((**storage).pEmbody, ciSelf);
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
	err = QTC_ImageCompressor_Open((**storage).pEmbody, ciSelf);

	return (err);
	}

//////////////////////////////////////////////////////////////////////////
// Derived Image-compressor Range

#ifdef _DEFINE_GetCodecInfo
	DEFINE_PROC_1(GetCodecInfo, CodecInfo*, pInfo)
#endif
#ifdef _DEFINE_GetCompressionTime
	DEFINE_PROC_6(GetCompressionTime, PixMapHandle, src, const Rect*, pSrcRect, short, depth, 
		  CodecQ*, pSpatialQuality, CodecQ*, pTemporalQuality, unsigned long*, pTime)
#endif
#ifdef _DEFINE_GetMaxCompressionSize
	DEFINE_PROC_5(GetMaxCompressionSize, PixMapHandle, src, const Rect*, pSrcRect, short, depth, 
		  CodecQ, quality, long*, pSize) 
#endif
#ifdef _DEFINE_PreCompress
	DEFINE_PROC_1(PreCompress, CodecCompressParams*, pParams)
#endif
#ifdef _DEFINE_BandCompress
	DEFINE_PROC_1(BandCompress, CodecCompressParams*, pParams)
#endif
#ifdef _DEFINE_PreDecompress
	DEFINE_PROC_1(PreDecompress, CodecDecompressParams*, pParams)
#endif
#ifdef _DEFINE_BandDecompress
	DEFINE_PROC_1(BandDecompress, CodecDecompressParams*, pParams)
#endif
// Place reserved for [:busy:]
#ifdef _DEFINE_GetCompressedImageSize
	DEFINE_PROC_5(GetCompressedImageSize, ImageDescriptionHandle, hDesc, Ptr, pData, long, bufferSize, 
		  ICMDataProcRecordPtr, dataProc, long*, pDataSize) 
#endif
#ifdef _DEFINE_GetSimilarity
	DEFINE_PROC_5(GetSimilarity, PixMapHandle, src, const Rect*, pSrcRect, ImageDescriptionHandle, hDesc, 
		  Ptr, pData, Fixed*, pSimilarity) 
#endif
// Place reserved for [:TrimImage:]
// Place reserved for [:RequestSettings:]
// Place reserved for [:GetSettings:]
// Place reserved for [:SetSettings:]
#ifdef _DEFINE_Flush
	DEFINE_PROC_0(Flush)
#endif
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
#ifdef _DEFINE_Preflight
	DEFINE_PROC_1(Preflight, CodecDecompressParams*, pParams)
#endif
#ifdef _DEFINE_Initialize
	DEFINE_PROC_1(Initialize, ImageSubCodecDecompressCapabilities*, pCap)
#endif
#ifdef _DEFINE_BeginBand
	DEFINE_PROC_3(BeginBand, CodecDecompressParams*, pParams, ImageSubCodecDecompressRecord*, pDR, long, flags)
#endif
#ifdef _DEFINE_DrawBand
	DEFINE_PROC_1(DrawBand, ImageSubCodecDecompressRecord*, pDR)
#endif
#ifdef _DEFINE_EndBand
	DEFINE_PROC_3(EndBand, ImageSubCodecDecompressRecord*, pDR, OSErr, result, long, flags)
#endif
#ifdef _DEFINE_QueueStarting
	DEFINE_PROC_0(QueueStarting)
#endif
#ifdef _DEFINE_QueueStopping
	DEFINE_PROC_0(QueueStopping)
#endif
// Place reserved for [:DroppingFrame:]
// Place reserved for [:ScheduleFrame:]
// Place reserved for [:CancelTrigger:]
// Place reserved for [:10:]
// Place reserved for [:11:]
// Place reserved for [:12:]
// Place reserved for [:13:]
// Place reserved for [:14:]
#ifdef _DEFINE_DecodeBand
	DEFINE_PROC_2(DecodeBand, ImageSubCodecDecompressRecord*, pDR, unsigned long, flags)
#endif
