/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieImporterDispatch
 *  Author:     Lincoln Yu
 *  Purpose:    Selector dispatch for QuickTime Movie-importer
 * == History ==        
 *	2006-02-09:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

ComponentSelectorOffset			(6)

ComponentRangeCount				(1)
ComponentRangeShift				(7)
ComponentRangeMask				(7F)

ComponentRangeBegin				(0)
	ComponentError				(Target)
	ComponentError				(Register)
	StdComponentCall			(Version)
	StdComponentCall			(CanDo)
	StdComponentCall			(Close)
	StdComponentCall			(Open)
ComponentRangeEnd				(0)

ComponentRangeBegin				(1)
	ComponentError				(0)
	ComponentError				(Handle)
	#ifdef _DEFINE_File
		ComponentCall 			(File)
	#else
		ComponentError			(File)
	#endif
	ComponentError				(SetSampleDuration)
	ComponentError				(SetSampleDescription)
	ComponentError				(SetMediaFile)
	ComponentError				(SetDimensions)
	ComponentError				(SetChunkSize)
	ComponentError				(SetProgressProc)
	ComponentError				(SetAuxiliaryData)
	ComponentError				(SetFromScrap)
	#ifdef _DEFINE_DoUserDialog
		ComponentCall 			(DoUserDialog)
	#else
		ComponentError			(DoUserDialog)
	#endif
	ComponentError				(SetDuration)
	ComponentError				(GetAuxiliaryDataType)
	#ifdef _DEFINE_Validate
		ComponentCall 			(Validate)
	#else
		ComponentError			(Validate)
	#endif
	ComponentError				(GetFileType)
	#ifdef _DEFINE_DataRef
		ComponentCall 			(DataRef)
	#else
		ComponentError 			(DataRef)
	#endif
	ComponentError				(GetSampleDescription)
	#ifdef _DEFINE_GetMIMETypeList
		ComponentCall 			(GetMIMETypeList)
	#else
		ComponentError			(GetMIMETypeList)
	#endif
	ComponentError				(SetOffsetAndLimit)
	ComponentError				(GetSettingsAsAtomContainer)
	ComponentError				(SetSettingsFromAtomContainer)
	ComponentError				(SetOffsetAndLimit64)
	ComponentError				(Idle)
	#ifdef _DEFINE_ValidateDataRef
		ComponentCall 			(ValidateDataRef)
	#else
		ComponentError			(ValidateDataRef)
	#endif
	ComponentError				(GetLoadState)
	ComponentError				(GetMaxLoadedTime)
	ComponentError				(EstimateCompletionTime)
	ComponentError				(SetDontBlock)
	ComponentError				(GetDontBlock)
	ComponentError				(SetIdleManager)
	ComponentError				(SetNewMovieFlags)
	ComponentError				(GetDestinationMediaType)
ComponentRangeEnd				(1)