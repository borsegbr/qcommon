/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MediaHandlerDispatch
 *  Author:     Lincoln Yu
 *  Purpose:    Selector dispatcher for QuickDraw Media-handler
 * == History ==        
 *	2006-01-25:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

ComponentComment ("Count of selectors in range 0")
ComponentSelectorOffset		(6)

ComponentComment ("Last selector range of this component")
ComponentRangeCount			(6)

ComponentComment ("Size of each selector range in bits")
ComponentRangeShift			(8)
ComponentRangeMask			(FF)

ComponentComment ("Core Selector Range")
ComponentRangeBegin			(0)
	StdComponentCall		(Target)
	StdComponentCall		(Register)
	StdComponentCall		(Version)
	StdComponentCall		(CanDo)
	StdComponentCall		(Close)
	StdComponentCall		(Open)
ComponentRangeEnd			(0)

ComponentRangeUnused		(1)
ComponentRangeUnused		(2)
ComponentRangeUnused		(3)
ComponentRangeUnused		(4)
ComponentRangeUnused		(5)

ComponentComment			("Derived Media-handler Range")
ComponentRangeBegin			(6)
	ComponentError			(0)
	#ifdef _DEFINE_Initialize
		ComponentCall		(Initialize)
	#else
		ComponentDelegate	(Initialize)
	#endif
	ComponentDelegate		(SetHandlerCapabilities)
	#ifdef _DEFINE_Idle
		ComponentCall		(Idle)
	#else
		ComponentDelegate	(Idle)
	#endif
	#ifdef _DEFINE_GetMediaInfo
		ComponentCall   	(GetMediaInfo)
	#else
		ComponentDelegate	(GetMediaInfo)
	#endif
	ComponentDelegate		(PutMediaInfo)
	#ifdef _DEFINE_SetActive
		ComponentCall   	(SetActive)
	#else
		ComponentDelegate	(SetActive)
	#endif
	#ifdef _DEFINE_SetRate
		ComponentCall   	(SetRate)
	#else
		ComponentDelegate	(SetRate)
	#endif
	ComponentDelegate		(GGetStatus)
	#ifdef _DEFINE_TrackEdited
		ComponentCall   	(TrackEdited)
	#else
		ComponentDelegate	(TrackEdited)
	#endif
	ComponentDelegate		(SetMediaTimeScale)
	ComponentDelegate		(SetMovieTimeScale)
	#ifdef _DEFINE_SetGWorld
		ComponentCall		(SetGWorld)
	#else
		ComponentDelegate	(SetGWorld)
	#endif
	#ifdef _DEFINE_SetDimensions
		ComponentCall		(SetDimensions)
	#else
		ComponentDelegate	(SetDimensions)
	#endif
	ComponentDelegate		(SetClip)
	#ifdef _DEFINE_SetMatrix
		ComponentCall		(SetMatrix)
	#else
		ComponentDelegate	(SetMatrix)
	#endif
	ComponentDelegate		(GetTrackOpaque)
	ComponentDelegate		(SetGraphicsMode)
	ComponentDelegate		(GetGraphicsMode)
	ComponentDelegate		(GSetVolume)
	ComponentDelegate		(SetSoundBalance)
	ComponentDelegate		(GetSoundBalance)
	ComponentDelegate		(GetNextBoundsChange)
	ComponentDelegate		(GetSrcRgn)
	#ifdef _DEFINE_Preroll
		ComponentCall		(Preroll)
	#else
		ComponentDelegate	(Preroll)
	#endif
	#ifdef _DEFINE_SampleDescriptionChanged
		ComponentCall		(SampleDescriptionChanged)
	#else
		ComponentDelegate	(SampleDescriptionChanged)
	#endif
	#ifdef _DEFINE_HasCharacteristic
		ComponentCall		(HasCharacteristic)
	#else
		ComponentDelegate	(HasCharacteristic)
	#endif
	ComponentDelegate		(GetOffscreenBufferSize)
	ComponentDelegate		(SetHints)
	ComponentDelegate		(GetName)
	ComponentDelegate		(ForceUpdate)
	ComponentDelegate		(GetDrawingRgn)
	ComponentDelegate		(GSetActiveSegment)
	ComponentDelegate		(InvalidateRegion)
	#ifdef _DEFINE_GetNextStepTime
		ComponentCall		(GetNextStepTime)
	#else
		ComponentDelegate	(GetNextStepTime)
	#endif
	ComponentDelegate		(SetNonPrimarySourceData)
	ComponentDelegate		(ChangedNonPrimarySource)
	ComponentDelegate		(TrackReferencesChanged)
	ComponentDelegate		(GetSampleDataPointer)
	ComponentDelegate		(ReleaseSampleDataPointer)
	ComponentDelegate		(TrackPropertyAtomChanged)
	ComponentDelegate		(SetTrackInputMapReference)
	ComponentDelegate		(SetVideoParam)
	ComponentDelegate		(GetVideoParam)
	ComponentDelegate		(Compare)
	ComponentDelegate		(GetClock)
	ComponentDelegate		(SetSoundOutputComponent)
	ComponentDelegate		(GetSoundOutputComponent)
	ComponentDelegate		(SetSoundLocalizationData)
	ComponentDelegate		(GetInvalidRegion)
	#if HANDLER_SWAPS_SAMPLE_DESC
		ComponentCall		(SampleDescriptionB2N)
		ComponentCall		(SampleDescriptionN2B)
	#else
		ComponentDelegate	(SampleDescriptionB2N)
		ComponentDelegate	(SampleDescriptionN2B)
	#endif
	ComponentDelegate		(QueueNonPrimarySourceData)
	ComponentDelegate		(FlushNonPrimarySourceData)
	ComponentDelegate		(GetURLLink)
	ComponentDelegate		(MakeMediaTimeTable)
	ComponentDelegate		(HitTestForTargetRefCon)
	ComponentDelegate		(HitTestTargetRefCon)
	ComponentDelegate		(GetActionsForQTEvent)
	ComponentDelegate		(DisposeTargetRefCon)
	ComponentDelegate		(TargetRefConsEqual)
	ComponentDelegate		(SetActionsCallback)
	ComponentDelegate		(PrePrerollBegin)
	ComponentDelegate		(PrePrerollCancel)
	ComponentDelegate		(EnterEmptyEdit)
	ComponentDelegate		(CurrentMediaQueuedData)
	ComponentDelegate		(GetEffectiveVolume)
	ComponentDelegate		(ResolveTargetRefCon)
	ComponentDelegate		(GetSoundLevelMeteringEnabled)
	ComponentDelegate		(SetSoundLevelMeteringEnabled)
	ComponentDelegate		(GetSoundLevelMeterInfo)
	ComponentDelegate		(GetEffectiveSoundBalance)
	ComponentDelegate		(SetScreenLock)
	ComponentDelegate		(SetDoMCActionCallback)
	ComponentDelegate		(GetErrorString)
	ComponentDelegate		(GetSoundEqualizerBands)
	ComponentDelegate		(SetSoundEqualizerBands)
	ComponentDelegate		(GetSoundEqualizerBandLevels)
	ComponentDelegate		(DoIdleActions)
	ComponentDelegate		(SetSoundBassAndTreble)
	ComponentDelegate		(GetSoundBassAndTreble)
	ComponentDelegate		(TimeBaseChanged)
ComponentRangeEnd			(6)
