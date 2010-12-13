/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MovieExporterDispatch
 *  Author:     Lincoln Yu
 *  Purpose:    Selector dispatch for QuickTime Movie-exporter
 * == History ==        
 *	2006-02-11:	Created.
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

ComponentSelectorOffset (-kComponentTargetSelect)

ComponentRangeCount (3)
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

ComponentRangeUnused			(1)

ComponentRangeBegin				(2)
	ComponentError				(ToHandle)
	#ifdef _DEFINE_ToFile
		ComponentCall			(ToFile)
	#else
		ComponentError			(ToFile)
	#endif
	ComponentError				(130)
	ComponentError				(GetAuxiliaryData)
	#ifdef _DEFINE_SetProgressProc
		ComponentCall			(SetProgressProc)
	#else
		CompoenntError			(SetProgressProc)
	#endif
	ComponentError				(SetSampleDescription)
	ComponentError				(DoUserDialog)
	ComponentError				(GetCreatorType)
	#ifdef _DEFINE_ToDataRef
		ComponentCall			(ToDataRef)
	#else
		ComponentError			(ToDataRef)
	#endif
	#ifdef _DEFINE_FromProceduresToDataRef
		ComponentCall			(FromProceduresToDataRef)
	#else
		ComponentError			(FromProceduresToDataRef)
	#endif
	#ifdef _DEFINE_AddDataSource
		ComponentCall			(AddDataSource)
	#else
		ComponentError			(AddDataSource)
	#endif
	#ifdef _DEFINE_Validate
		ComponentCall			(Validate)
	#else
		ComponentError			(Validate)
	#endif
	#ifdef _DEFINE_GetSettingsAsAtomContainer
		ComponentCall			(GetSettingsAsAtomContainer)
	#else
		ComponentError			(GetSettingsAsAtomContainer)
	#endif
	#ifdef _DEFINE_SetSettingsFromAtomContainer
		ComponentCall			(SetSettingsFromAtomContainer)
	#else
		ComponentError			(SetSettingsFromAtomContainer)
	#endif
	#ifdef _DEFINE_GetFileNameExtension
			ComponentCall		(GetFileNameExtension)
	#else
			ComponentError		(GetFileNameExtension)
	#endif
	#ifdef _DEFINE_GetShortFileTypeString
			ComponentCall		(GetShortFileTypeString)
	#else
			ComponentError		(GetShortFileTypeString)
	#endif
	#ifdef _DEFINE_GetSourceMediaType
			ComponentCall		(GetSourceMediaType)
	#else
			ComponentError		(GetSourceMediaType)
	#endif
	ComponentError				(SetGetMoviePropertyProc)
ComponentRangeEnd				(2)

ComponentRangeBegin				(3)
	ComponentError				(NewGetDataAndPropertiesProcs)
	ComponentError				(DisposeGetDataAndPropertiesProcs)
ComponentRangeEnd				(3)
