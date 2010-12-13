#include "./QTC_ImageCompressor.h"

#include <qul/quicktime/base6/BasicMacros.h>


#define RESULT_TYPE				QTC_Result
#define COMPLETE_NAME(name)		CAT2(QTC_ImageCompressor_,name) (void *pUnk
#define D_COMMA()				,
#define CALL_PROC(name)			((qtc::ImageCompressor *)pUnk)->name (
#define C_COMMA()				
#include <qul/quicktime/base6/procdef.h>

#define CONVERT()				((qtc::ImageCompressor *)pUnk)


//////////////////////////////////////////////////////////////////////////
// Core Selector Range

DEFINE_PROC_1(Target, ComponentInstance, ciTarget)
DEFINE_PROC_0(Register)
DEFINE_PROC_0(Version)
// Place reserved for [:CanDo:] ...
DEFINE_PROC_1(Close, ComponentInstance, ciSelf)
DEFINE_PROC_1(Open, ComponentInstance, ciSelf)

//////////////////////////////////////////////////////////////////////////
// Derived Image-compressor Range

DEFINE_PROC_1(GetCodecInfo, CodecInfo*, pInfo)
DEFINE_PROC_6(GetCompressionTime, PixMapHandle, src, const Rect*, pSrcRect, short, depth, 
	CodecQ*, pSpatialQuality, CodecQ*, pTemporalQuality, unsigned long*, pTime)
DEFINE_PROC_5(GetMaxCompressionSize, PixMapHandle, src, const Rect*, pSrcRect, short, depth, 
	CodecQ, quality, long*, pSize) 
DEFINE_PROC_1(PreCompress, CodecCompressParams*, pParams)
DEFINE_PROC_1(BandCompress, CodecCompressParams*, pParams)
DEFINE_PROC_1(PreDecompress, CodecDecompressParams*, pParams)
DEFINE_PROC_1(BandDecompress, CodecDecompressParams*, pParams)
// Place reserved for [:busy:]
DEFINE_PROC_5(GetCompressedImageSize, ImageDescriptionHandle, hDesc, Ptr, pData, long, bufferSize, 
	ICMDataProcRecordPtr, dataProc, long*, pDataSize) 
DEFINE_PROC_5(GetSimilarity, PixMapHandle, src, const Rect*, pSrcRect, ImageDescriptionHandle, hDesc, 
	Ptr, pData, Fixed*, pSimilarity) 
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
DEFINE_PROC_1(Preflight, CodecDecompressParams*, pParams)
DEFINE_PROC_1(Initialize, ImageSubCodecDecompressCapabilities*, pCap)
DEFINE_PROC_3(BeginBand, CodecDecompressParams*, pParams, ImageSubCodecDecompressRecord*, pDR, long, flags)
DEFINE_PROC_1(DrawBand, ImageSubCodecDecompressRecord*, pDR)
DEFINE_PROC_3(EndBand, ImageSubCodecDecompressRecord*, pDR, OSErr, result, long, flags)
DEFINE_PROC_0(QueueStarting)
DEFINE_PROC_0(QueueStopping)
// Place reserved for [:DroppingFrame:]
// Place reserved for [:ScheduleFrame:]
// Place reserved for [:CancelTrigger:]