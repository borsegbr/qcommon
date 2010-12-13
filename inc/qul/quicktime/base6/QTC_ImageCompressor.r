/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_ImageCompressor.r
 *  Author:     Lincoln Yu
 *  Purpose:    QuickTime Image-compressor resource file
 *  
 * == History ==        
 *	2006-03-04:	Created.
 *	2006-05-30:	Minor changes.
 *      2006-06-19:     Mac-IA32 support added.
 * == Comments == 
 *  The following parametric macros are needed:
 *		kQTC_IMC_FormatName
 *		kQTC_IMC_Manufacturer
 *		kQTC_IMDC_Flags
 *		kQTC_IMCO_Flags 
 *		kQTC_IMC_FormatFlags
 *		kQTC_IMC_Version
 *		kQTC_IMC_ComponentDescID
 *		kQTC_IMDC_ComponentResID
 *		kQTC_IMCO_ComponentResID
 *		[kQTC_IMDC_Accr]
 *		[kQTC_IMCO_Accr]
 *		[kQTC_IMDC_Spd]
 *		[kQTC_IMCO_Spd]
 *		[kQTC_IMC_Level]
 *		[kQTC_IMC_MinHeight]
 *		[kQTC_IMC_MinWidth]
 *		[kQTC_IMDC_PL]
 *		[kQTC_IMCO_PL]
 *		kQTC_IMDC_NameString
 *		kQTC_IMDC_InfoString
 *		kQTC_IMCO_NameString
 *		kQTC_IMCO_InfoString
 *		[kQTC_IMDC_NameStringResID]
 *		[kQTC_IMDC_InfoStringResID]
 *		[kQTC_IMCO_NameStringResID]
 *		[kQTC_IMCO_InfoStringResID]
 *		kQTC_IMC_Qualifier1
 *		kQTC_IMC_InternalName
 *		kQTC_IMC_ComponentDispatch
 *		
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */


#define thng_RezTemplateVersion		1

#define cfrg_RezTemplateVersion		1

#if TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32
    #include <Carbon/Carbon.r>
    #include <QuickTime/QuickTime.r>
	#undef __CARBON_R__
	#undef __CORESERVICES_R__
	#undef __CARBONCORE_R__
	#undef __COMPONENTS_R__
#else
    #include "ConditionalMacros.r"
    #include "MacTypes.r"
    #include "Components.r"
    #include "ImageCodec.r"
    #include "CodeFragments.r"
	#undef __COMPONENTS_R__
#endif


/* <block type = "Set default IDs"> */

#ifndef kQTC_IMDC_ComponentResID
	#define kQTC_IMDC_ComponentResID	kQTC_IMC_ComponentDescID
#endif

#ifndef kQTC_IMCO_ComponentResID
	#define kQTC_IMCO_ComponentResID	(kQTC_IMC_ComponentDescID)+4
#endif

#ifndef kQTC_IMDC_NameStringResID
	#define kQTC_IMDC_NameStringResID	kQTC_IMDC_ComponentResID
#endif

#ifndef kQTC_IMDC_InfoStringResID
	#ifdef kQTC_IMDC_InfoString
		#define kQTC_IMDC_InfoStringResID	(kQTC_IMDC_ComponentResID)+1
	#else
		#define kQTC_IMDC_InfoStringResID	0
	#endif
#endif

#ifndef kQTC_IMCO_NameStringResID
	#define kQTC_IMCO_NameStringResID	kQTC_IMCO_ComponentResID
#endif

#ifndef kQTC_IMCO_InfoStringResID
	#ifdef kQTC_IMCO_InfoString
		#define kQTC_IMCO_InfoStringResID	(kQTC_IMCO_ComponentResID)+1
	#else
		#define kQTC_IMCO_InfoStringResID	0
	#endif
#endif

#ifndef kQTC_IMC_CodeID
	#define kQTC_IMC_CodeID			kQTC_IMC_ComponentDescID
#endif

/* </block type = "Set default IDs"> */

/* <block type = "Set default values"> */

#ifndef kQTC_IMC_Version
	#define kQTC_IMC_Version		1
#endif

#ifndef kQTC_IMC_RevisionLevel
	#define kQTC_IMC_RevisionLevel	1
#endif

#ifndef kQTC_IMDC_Flags
	#define kQTC_IMDC_Flags			(codecInfoDoes32 | codecInfoDoesSpool)
#endif

#ifndef kQTC_IMCO_Flags
	#define kQTC_IMCO_Flags			(codecInfoDoes32)
#endif

#ifndef kQTC_IMC_FormatFlags
	#define kQTC_IMC_FormatFlags	(codecInfoDepth32)
#endif

#ifndef kQTC_IMCO_Accr
	#define kQTC_IMCO_Accr			128
#endif

#ifndef kQTC_IMDC_Accr
	#define kQTC_IMDC_Accr			128
#endif

#ifndef kQTC_IMCO_Spd
	#define kQTC_IMCO_Spd			200
#endif

#ifndef kQTC_IMDC_Spd
	#define kQTC_IMDC_Spd			200
#endif

#ifndef kQTC_IMC_Level
	#define kQTC_IMC_Level			128
#endif

#ifndef kQTC_IMC_MinHeight
	#define kQTC_IMC_MinHeight		1
#endif

#ifndef kQTC_IMC_MinWidth
	#define kQTC_IMC_MinWidth		1
#endif

#ifndef kQTC_IMDC_PL
	#define kQTC_IMDC_PL			0
#endif

#ifndef kQTC_IMCO_PL
	#define kQTC_IMCO_PL			0
#endif

/* </block title = "Set default IDs"> */


#define ARRAY_SECTION

// Component Description
resource 'cdci' (kQTC_IMC_ComponentDescID) 
	{
	kQTC_IMC_FormatName,							// Type
	kQTC_IMC_Version,								// Version
	kQTC_IMC_RevisionLevel,							// Revision level
	kQTC_IMC_Manufacturer,							// Manufacturer
	kQTC_IMDC_Flags,								// Decompression Flags
	kQTC_IMCO_Flags,								// Compression Flags
	kQTC_IMC_FormatFlags,							// Format Flags
	kQTC_IMCO_Accr,									// Compression Accuracy
	kQTC_IMDC_Accr,									// Decomression Accuracy
	kQTC_IMCO_Spd,									// Compression Speed
	kQTC_IMDC_Spd,									// Decompression Speed
	kQTC_IMC_Level,									// Compression Level
	0,												// Reserved
	kQTC_IMC_MinHeight,								// Minimum Height
	kQTC_IMC_MinWidth,								// Minimum Width
	kQTC_IMDC_PL,									// Decompression Pipeline Latency
	kQTC_IMCO_PL,									// Compression Pipeline Latency
	0												// Private Data
	};

/* Deompressor */
resource 'thng' (kQTC_IMDC_ComponentResID) 
	{
	decompressorComponentType,						// Type			
	kQTC_IMC_Subtype,								// SubType
	kQTC_IMC_Manufacturer,							// Manufacturer
	#if TARGET_REZ_MAC_68K
		kQTC_IMDC_Flags,							// Component flags
		0,											// Component flags Mask
		'cdec',										// Code Type
		kQTC_IMC_CodeID,							// Code ID
	#else
		0,											// - use componentHasMultiplePlatforms
		0,
		0,
		0,
	#endif
	'STR ',											// Name Type
	kQTC_IMDC_NameStringResID,						// Name ID
	'STR ',											// Info Type
	kQTC_IMDC_InfoStringResID,						// Info ID
	0,												// Icon Type
	0,												// Icon ID
	kQTC_IMC_Version,								// Codec version
	componentHasMultiplePlatforms |					// Registration Flags 
		componentDoAutoVersion,
	0,												// Resource ID of Icon Family
	ARRAY_SECTION
		{
		#if TARGET_OS_MAC
			#if TARGET_REZ_CARBON_CFM
				kQTC_IMDC_Flags,					// Component Flags 						
				'cfrg',								// Special Case: data-fork based code fragment
				kQTC_IMC_CodeID,	 				// Code ID usage for CFM components:
				platformPowerPCNativeEntryPoint,	// Platform Type (response from gestaltComponentPlatform or failing that, gestaltSysArchitecture)
			#elif TARGET_REZ_CARBON_MACHO
				kQTC_IMDC_Flags, 
				'dlle',								// Code Resource type - Entry point found by symbol name 'dlle' resource
				kQTC_IMC_CodeID,					// ID of 'dlle' resource
				platformPowerPCNativeEntryPoint,
			#elif TARGET_REZ_MAC_PPC
				kQTC_IMDC_Flags, 
				'cdek',								// Code Type
				kQTC_IMC_CodeID,					// Code ID
				platformPowerPC,
			#elif TARGET_REZ_MAC_68K
				kQTC_IMDC_Flags,
				'cdec',
				kQTC_IMC_CodeID,
				platform68k,
                        #elif TARGET_REZ_MAC_IA32
				kQTC_IMDC_Flags,
				'dlle',
				kQTC_IMC_CodeID,
				platformIA32NativeEntryPoint,
			#else
				#error "At least one TARGET_REZ_XXX_XXX platform must be defined."
			#endif
		#endif
		#if TARGET_OS_WIN32
			kQTC_IMDC_Flags, 
			'dlle',
			kQTC_IMC_CodeID,
			platformWin32,
		#endif
		};
	};

/* Compressor */
resource 'thng' (kQTC_IMCO_ComponentResID) 
	{
	compressorComponentType,						// Type			
	kQTC_IMC_Subtype,								// SubType
	kQTC_IMC_Manufacturer,							// Manufacturer
	#if TARGET_REZ_MAC_68K
		kQTC_IMCO_Flags,							// Component flags
		0,											// Component flags Mask
		'cdec',										// Code Type
		kQTC_IMC_CodeID,							// Code ID
	#else
		0,											// - use componentHasMultiplePlatforms
		0,
		0,
		0,
	#endif
	'STR ',											// Name Type
	kQTC_IMCO_NameStringResID,						// Name ID
	'STR ',											// Info Type
	kQTC_IMCO_InfoStringResID,						// Info ID
	0,												// Icon Type
	0,												// Icon ID
	kQTC_IMC_Version,								// Codec version
	componentHasMultiplePlatforms |					// Registration Flags 
		componentDoAutoVersion,
	0,												// Resource ID of Icon Family
	ARRAY_SECTION	
		{
		#if TARGET_OS_MAC
			#if TARGET_REZ_CARBON_CFM
				kQTC_IMCO_Flags,					// Component Flags 						
				'cfrg',								// Special Case: data-fork based code fragment
				kQTC_IMC_CodeID,	 				// Code ID usage for CFM components:
				platformPowerPCNativeEntryPoint,	// Platform Type (response from gestaltComponentPlatform or failing that, gestaltSysArchitecture)
			#elif TARGET_REZ_CARBON_MACHO
				kQTC_IMCO_Flags, 
				'dlle',								// Code Resource type - Entry point found by symbol name 'dlle' resource
				kQTC_IMC_CodeID,					// ID of 'dlle' resource
				platformPowerPCNativeEntryPoint,
			#elif TARGET_REZ_MAC_PPC
				kQTC_IMCO_Flags, 
				'cdek',								// Code Type
				kQTC_IMC_CodeID,					// Code ID
				platformPowerPC,
			#elif TARGET_REZ_MAC_68K
				kQTC_IMCO_Flags,
				'cdec',
				kQTC_IMC_CodeID,
				platform68k,
                        #elif TARGET_REZ_MAC_IA32
				kQTC_IMCO_Flags,
				'dlle',
				kQTC_IMC_CodeID,
				platformIA32NativeEntryPoint,
			#else
				#error "At least one TARGET_REZ_XXX_XXX platform must be defined."
			#endif
		#endif
		#if TARGET_OS_WIN32
			kQTC_IMCO_Flags, 
			'dlle',
			kQTC_IMC_CodeID,
			platformWin32,
		#endif
		};
	};

// Deompressor Name
resource 'STR ' (kQTC_IMDC_NameStringResID)
	{
	kQTC_IMDC_NameString
	};

#if kQTC_IMDC_InfoStringResID
	// Deompressor Information
	resource 'STR ' (kQTC_IMDC_InfoStringResID)
		{
		kQTC_IMDC_InfoString
		};
#endif

// Compressor Name
resource 'STR ' (kQTC_IMCO_NameStringResID)
	{
	kQTC_IMCO_NameString
	};

#if kQTC_IMCO_InfoStringResID
	// Compressor Information
	resource 'STR ' (kQTC_IMCO_InfoStringResID)
		{
		kQTC_IMCO_InfoString
		};
#endif		

#if TARGET_REZ_CARBON_CFM
	// Custom extended code fragment resource
	// CodeWarrior will correctly adjust the offset and length of each
	// code fragment when building a MacOS Merge target
	resource 'cfrg' (0) 
		{
		ARRAY_SECTION
			{		
			extendedEntry 
				{
				kPowerPCCFragArch,					// archType
				kIsCompleteCFrag,					// updateLevel
				kNoVersionNum,						// currentVersion
				kNoVersionNum,						// oldDefVersion
				kDefaultStackSize,					// appStackSize
				kNoAppSubFolder,					// appSubFolderID
				kImportLibraryCFrag,				// usage
				kDataForkCFragLocator,				// where
				kZeroOffset,						// offset
				kCFragGoesToEOF,					// length
				kQTC_IMC_MemberName,				// member name
				
				// Start of extended info.
				
				'cpnt',								// libKind (not kFragComponentMgrComponent == 'comp' as you might expect)
				kQTC_IMC_Qualifier1,				// qualifier 1 - hex 0x0100 (256) matches Code ID in 'thng'
				"",									// qualifier 2
				"",									// qualifier 3
				kQTC_IMC_InternalName,				// intlName, localised
				};
			};
		};
#endif

#if	TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32 || TARGET_REZ_WIN32
	// Code Entry Point for Mach-O and Windows
	resource 'dlle' (kQTC_IMC_CodeID) 
		{
		kQTC_IMC_ComponentDispatch
		};
#endif