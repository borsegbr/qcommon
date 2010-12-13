/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_ImageCompressorDispatch
 *  Author:			Lincoln Yu
 *  Purpose:    Selector dispatcher for QuickDraw Image-compressor
 * == History ==			   
 *	2006-03-04:	Created.
 *	2006-06-21:	Selector [:DecodeBand:] added.
 * == Notice ==
 *	The selector [:DecodeBand:] is only supported by QT7 or later.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

#ifndef _TARGET_OS_MAC68K
	ComponentSelectorOffset (8)
#else
	ComponentSelectorOffset (6)
#endif

ComponentRangeCount			(3)
ComponentRangeShift			(8)
ComponentRangeMask			(FF)

ComponentRangeBegin			(0)
	#ifndef _TARGET_OS_MAC68K
		#ifdef _DEFINE_GetMPWorkFunction
			StdComponentCall(GetMPWorkFunction)
		#else
			ComponentError	(GetMPWorkFunction)
		#endif
		ComponentError		(Unregister)
	#endif
	StdComponentCall		(Target)
	ComponentError			(Register)
	StdComponentCall		(Version)
	StdComponentCall		(CanDo)
	StdComponentCall		(Close)
	StdComponentCall		(Open)
ComponentRangeEnd			(0)
	
ComponentRangeBegin			(1)
#ifdef _DEFINE_GetCodecInfo
	ComponentCall			(GetCodecInfo)
#else
	ComponentDelegate		(GetCodecInfo)
#endif
#ifdef _DEFINE_GetCompressionTime
	ComponentCall			(GetCompressionTime)
#else
	ComponentDelegate		(GetCompressionTime)
#endif
#ifdef _DEFINE_GetMaxCompressionSize
	ComponentCall			(GetMaxCompressionSize)
#else
	ComponentDelegate		(GetMaxCompressionSize)
#endif
#ifdef _DEFINE_PreCompress
	ComponentCall			(PreCompress)
#else
	ComponentDelegate		(PreCompress)
#endif
#ifdef _DEFINE_BandCompress
	ComponentCall			(BandCompress)
#else
	ComponentDelegate		(BandCompress)
#endif
#ifdef _DEFINE_PreDecompress
	ComponentCall			(PreDecompress)
#else
	ComponentDelegate		(PreDecompress)
#endif
#ifdef _DEFINE_BandDecompress
	ComponentCall			(BandDecompress)
#else
	ComponentDelegate		(BandDecompress)
#endif
	ComponentDelegate		(Busy)
#ifdef _DEFINE_GetCompressedImageSize
	ComponentCall			(GetCompressedImageSize)
#else
	ComponentDelegate		(GetCompressedImageSize)
#endif
#ifdef _DEFINE_GetSimilarity
	ComponentCall			(GetSimilarity)
#else
	ComponentDelegate		(GetSimilarity)
#endif
	ComponentDelegate		(TrimImage)
	ComponentDelegate		(RequestSettings)
	ComponentDelegate		(GetSettings)
	ComponentDelegate		(SetSettings)
#ifdef _DEFINE_Flush
	ComponentCall			(Flush)
#else
	ComponentDelegate		(Flush)
#endif
	ComponentDelegate		(SetTimeCode)
	ComponentDelegate		(IsImageDescriptionEquivalent)
	ComponentDelegate		(NewMemory)
	ComponentDelegate		(DisposeMemory)
	ComponentDelegate		(HitTestData)
	ComponentDelegate		(NewImageBufferMemory)
	ComponentDelegate		(ExtractAndCombineFields)
	ComponentDelegate		(GetMaxCompressionSizeWithSources)
	ComponentDelegate		(SetTimeBase)
	ComponentDelegate		(SourceChanged)
	ComponentDelegate		(FlushLastFrame)
	ComponentDelegate		(GetSettingsAsText)
	ComponentDelegate		(GetParameterListHandle)
	ComponentDelegate		(GetParameterList)
	ComponentDelegate		(CreateStandardParameterDialog)
	ComponentDelegate		(IsStandardParameterDialogEvent)
	ComponentDelegate		(DismissStandardParameterDialog)
	ComponentDelegate		(StandardParameterDialogDoAction)
	ComponentDelegate		(NewImageGWorld)
	ComponentDelegate		(DisposeImageGWorld)
	ComponentDelegate		(HitTestDataWithFlags)
	ComponentDelegate		(ValidateParameters)
	ComponentDelegate		(GetBaseMPWorkFunction)
	ComponentDelegate		(0x0026)
	ComponentDelegate		(0x0027)
	ComponentDelegate		(RequestGammaLevel)
	ComponentDelegate		(GetSourceDataGammaLevel)
	ComponentDelegate		(0x002A)
	ComponentDelegate		(GetDecompressLatency)
	ComponentDelegate		(MergeFloatingImageOntoWindow)
	ComponentDelegate		(RemoveFloatingImage)
	ComponentDelegate		(GetDITLForSize)
	ComponentDelegate		(DITLInstall)
	ComponentDelegate		(DITLEvent)
	ComponentDelegate		(DITLItem)
	ComponentDelegate		(DITLRemove)
	ComponentDelegate		(DITLValidateInput)
	ComponentDelegate		(0x0034)
	ComponentDelegate		(0x0035)
	ComponentDelegate		(GetPreferredChunkSizeAndAlignment)
	ComponentDelegate		(PrepareToCompressFrames)
	ComponentDelegate		(EncodeFrame)
	ComponentDelegate		(CompleteFrame)
	ComponentDelegate		(BeginPass)
	ComponentDelegate		(EndPass)
	ComponentDelegate		(ProcessBetweenPasses)	
ComponentRangeEnd			(1)

ComponentRangeUnused		(2)

ComponentRangeBegin			(3)
#ifdef _DEFINE_Preflight
	ComponentCall			(Preflight)
#else
	ComponentDelegate		(Preflight)
#endif
#ifdef _DEFINE_Initialize
	ComponentCall			(Initialize)
#else
	ComponentDelegate		(Initialize)
#endif
#ifdef _DEFINE_BeginBand
	ComponentCall			(BeginBand)
#else
	ComponentDelegate		(BeginBand)
#endif
#ifdef _DEFINE_DrawBand
	ComponentCall			(DrawBand)
#else
	ComponentDelegate		(DrawBand)
#endif
#ifdef _DEFINE_EndBand
	ComponentCall			(EndBand)
#else
	ComponentDelegate		(EndBand)
#endif
#ifdef _DEFINE_QueueStarting
	ComponentCall			(QueueStarting)
#else
	ComponentDelegate		(QueueStarting)
#endif
#ifdef _DEFINE_QueueStopping
	ComponentCall			(QueueStopping)
#else
	ComponentDelegate		(QueueStopping)
#endif
	ComponentDelegate		(DroppingFrame)
	ComponentDelegate		(ScheduleFrame)
	ComponentDelegate		(CancelTrigger)
	ComponentDelegate		(10)
	ComponentDelegate		(11)
	ComponentDelegate		(12)
	ComponentDelegate		(13)
	ComponentDelegate		(14)
#ifdef _DEFINE_DecodeBand
	ComponentCall			(DecodeBand)
#else
	ComponentDelegate		(DecodeBand)
#endif
ComponentRangeEnd			(3)
