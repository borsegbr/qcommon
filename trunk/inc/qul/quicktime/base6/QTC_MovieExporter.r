/**
 * === GalaxyCodec ===
 *  Unit Name:  QTC_MovieExporter.r
 *  Author:     Lincoln Yu
 *  Purpose:    QuickTime Movie-exporter component resource file
 * == History ==        
 *	2006-02-10:	Created.
 *      2006-06-19:     Mac-IA32 support added.
 * == Comments ==
 *  The following parametric macros are needed:
 *		kQTC_SPIT_Name					- NOT USED, name of the media-handler, a string
 *		kQTC_SPIT_Subtype				- subtype, a four char code
 *		kQTC_SPIT_Manufacturer			- component manufacturer, a four char code
 *		kQTC_SPIT_Flags					- traits, a 32-bit unsigned value
 *		kQTC_SPIT_ComponentResID		- component resource ID
 *		[kQTC_SPIT_ComponentAliasID]	- component alias ID, one bigger than component resouce ID by default
 *		[kQTC_SPIT_NameStringResID]		- name string resource ID, equal to component resouce ID by default
 *		[kQTC_SPIT_InfoStringResID]		- info string ID, one bigger than name stirng ID by default
 *		[kQTC_SPIT_CodeID]				- code ID, equal to component resource ID by default
 *		[kQTC_SPIT_MimeInfoID]			- mime info ID, equal to component resource ID by default
 *		[kQTC_SPIT_ConfigID]			- media config ID, equal to component resource ID by default
 *		[kQTC_SPIT_ThingResID]			- thing resource ID, equal to component resource ID by default
 *		[kQTC_SPIT_PresetListResID]		- preset list ID
 *		[kQTC_SPIT_PresetStringListID]	- ID of the string list for the preset list
 *		kQTC_SPIT_NameString			- name string
 *		[kQTC_SPIT_InfoString]			- info string
 *		kQTC_SPIT_Version				- version number of the media-handler, a 32-bit unsigned value
 *		[kQTC_SPIT_PresetString(01~)]	- preset strings, inducing the corresponding atom data
 *		[kQTC_SPIT_PresetDevider(01~)]	- devider in the preset list
 *		[kQTC_SPIT_AtomDataID(01~)]		- atom data ID
 *		[kQTC_SPIT_AtomDataName(01~)]	- atom data name
 *		[kQTC_SPIT_AtomData(01~)]		- string of hex values
 *		[kQTC_SPIT_SrcVideo]			- having video source, min and max value should be given, the same as following
 *		[kQTC_SPIT_SrcSound]			- having sound source
 *		[kQTC_SPIT_TrkVideo]			- tracks of video media type
 *		[kQTC_SPIT_TrkSound]			- tracks of sound media type
 *		[kQTC_SPIT_TrkMusic]			- tracks of music media type
 *		[kQTC_SPIT_TrkEyes]				- tracks of media characteristic 'eyes'
 *		[kQTC_SPIT_TrkEars]				- tracks of media characteristic 'ears'
 *		kQTC_SPIT_Qualifier1,			- qualifier 1, hex value derived from the string matches [:kQTC_SPIT_ComponentResID:]
 *		kQTC_SPIT_MemberName			- member name, a string
 *		kQTC_SPIT_InternalName			- internal name, a string
 *		kQTC_SPIT_ComponentDispatch		- entry of component dispatch, a string 
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */

/**
 * == Comments ==
 *  [:thng_RezTemplateVersion:]
 *	 0 - original 'thng' template	<-- default
 *	 1 - extended 'thng' template	<-- used for multiplatform things
 *	 2 - extended 'thng' template including resource map id
 */
#define thng_RezTemplateVersion		2

/**
 * == Comments ==
 *	[:cfrg_RezTemplateVersion:]
 *	 0 - original					<-- default
 *	 1 - extended
 */
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
	#include "Components.r"
	#include "QuickTimeComponents.r"
	#include "MacTypes.r"
	#include "CodeFragments.r"
	#undef __COMPONENTS_R__
#endif


#ifndef kQTC_SPIT_NameStringResID
	#define kQTC_SPIT_NameStringResID	kQTC_SPIT_ComponentResID
#endif
#ifndef kQTC_SPIT_ComponentAliasID
	#define kQTC_SPIT_ComponentAliasID	(kQTC_SPIT_ComponentResID)+1
#endif
#ifndef kQTC_SPIT_CodeID
	#define kQTC_SPIT_CodeID			kQTC_SPIT_ComponentResID
#endif
#ifndef kQTC_SPIT_ThingResID
	#define kQTC_SPIT_ThingResID		kQTC_SPIT_ComponentResID
#endif
#ifndef kQTC_SPIT_InfoStringResID
	#ifdef kQTC_SPIT_InfoString
		#define kQTC_SPIT_InfoStringResID	(kQTC_SPIT_NameStringResID)+1
	#else
		#define kQTC_SPIT_InfoStringResID	0
	#endif
#endif
#ifndef kQTC_SPIT_FileType
	#define kQTC_SPIT_FileType			kQTC_SPIT_Subtype
#endif

#ifdef kQTC_SPIT_PresetString01
	#ifndef kQTC_SPIT_PresetListResID
		#define kQTC_SPIT_PresetListResID	kQTC_SPIT_ComponentResID
	#endif
	#ifndef kQTC_SPIT_PresetStringListID
		#define kQTC_SPIT_PresetStringListID	kQTC_SPIT_ComponentResID
	#endif

	#ifndef kQTC_SPIT_AtomDataID01
		#define kQTC_SPIT_AtomDataID01	kQTC_SPIT_ComponentResID
	#endif
	#ifndef kQTC_SPIT_AtomDataName01
		#define kQTC_SPIT_AtomDataName01	"kQTC_SPIT_AtomDataName01"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString02
	#ifndef kQTC_SPIT_AtomDataID02
		#define kQTC_SPIT_AtomDataID02	(kQTC_SPIT_ComponentResID)+1
	#endif
	#ifndef kQTC_SPIT_AtomDataName02
		#define kQTC_SPIT_AtomDataName02	"kQTC_SPIT_AtomDataName02"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString03
	#ifndef kQTC_SPIT_AtomDataID03
		#define kQTC_SPIT_AtomDataID03	(kQTC_SPIT_ComponentResID)+2
	#endif
	#ifndef kQTC_SPIT_AtomDataName03
		#define kQTC_SPIT_AtomDataName03	"kQTC_SPIT_AtomDataName03"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString04
	#ifndef kQTC_SPIT_AtomDataID04
		#define kQTC_SPIT_AtomDataID04	(kQTC_SPIT_ComponentResID)+3
	#endif
	#ifndef kQTC_SPIT_AtomDataName04
		#define kQTC_SPIT_AtomDataName04	"kQTC_SPIT_AtomDataName04"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString05
	#ifndef kQTC_SPIT_AtomDataID05
		#define kQTC_SPIT_AtomDataID05	(kQTC_SPIT_ComponentResID)+4
	#endif
	#ifndef kQTC_SPIT_AtomDataName05
		#define kQTC_SPIT_AtomDataName05	"kQTC_SPIT_AtomDataName05"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString06
	#ifndef kQTC_SPIT_AtomDataID06
		#define kQTC_SPIT_AtomDataID06	(kQTC_SPIT_ComponentResID)+5
	#endif
	#ifndef kQTC_SPIT_AtomDataName06
		#define kQTC_SPIT_AtomDataName06	"kQTC_SPIT_AtomDataName06"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString07
	#ifndef kQTC_SPIT_AtomDataID07
		#define kQTC_SPIT_AtomDataID07	(kQTC_SPIT_ComponentResID)+6
	#endif
	#ifndef kQTC_SPIT_AtomDataName07
		#define kQTC_SPIT_AtomDataName07	"kQTC_SPIT_AtomDataName07"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString08
	#ifndef kQTC_SPIT_AtomDataID08
		#define kQTC_SPIT_AtomDataID08	(kQTC_SPIT_ComponentResID)+7
	#endif
	#ifndef kQTC_SPIT_AtomDataName08
		#define kQTC_SPIT_AtomDataName08	"kQTC_SPIT_AtomDataName08"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString09
	#ifndef kQTC_SPIT_AtomDataID09
		#define kQTC_SPIT_AtomDataID09	(kQTC_SPIT_ComponentResID)+8
	#endif
	#ifndef kQTC_SPIT_AtomDataName09
		#define kQTC_SPIT_AtomDataName09	"kQTC_SPIT_AtomDataName09"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString10
	#ifndef kQTC_SPIT_AtomDataID10
		#define kQTC_SPIT_AtomDataID10	(kQTC_SPIT_ComponentResID)+9
	#endif
	#ifndef kQTC_SPIT_AtomDataName10
		#define kQTC_SPIT_AtomDataName10	"kQTC_SPIT_AtomDataName10"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString11
	#ifndef kQTC_SPIT_AtomDataID11
		#define kQTC_SPIT_AtomDataID11	(kQTC_SPIT_ComponentResID)+10
	#endif
	#ifndef kQTC_SPIT_AtomDataName11
		#define kQTC_SPIT_AtomDataName11	"kQTC_SPIT_AtomDataName11"
	#endif
#endif
#ifdef kQTC_SPIT_PresetString12
	#ifndef kQTC_SPIT_AtomDataID12
		#define kQTC_SPIT_AtomDataID12	(kQTC_SPIT_ComponentResID)+11
	#endif
	#ifndef kQTC_SPIT_AtomDataName12
		#define kQTC_SPIT_AtomDataName12	"kQTC_SPIT_AtomDataName12"
	#endif
#endif


#ifdef kQTC_SPIT_SrcVideo
	#ifndef kQTC_SPIT_SrcVideoMin
		#define kQTC_SPIT_SrcVideoMin	0
	#endif
	#ifndef kQTC_SPIT_SrcVideoMax
		#define kQTC_SPIT_SrcVideoMax	1
	#endif
#endif
#ifdef kQTC_SPIT_SrcSound
	#ifndef kQTC_SPIT_SrcSoundMin
		#define kQTC_SPIT_SrcSoundMin	0
	#endif
	#ifndef kQTC_SPIT_SrcSoundMax
		#define kQTC_SPIT_SrcSoundMax	1
	#endif
#endif

#ifdef kQTC_SPIT_TrkVideo
	#ifndef kQTC_SPIT_TrkVideoMin
		#define kQTC_SPIT_TrkVideoMin	0
	#endif
	#ifndef kQTC_SPIT_TrkVideoMax
		#define kQTC_SPIT_TrkVideoMax	65535
	#endif
#endif
#ifdef kQTC_SPIT_TrkSound
	#ifndef kQTC_SPIT_TrkSoundMin
		#define kQTC_SPIT_TrkSoundMin	0
	#endif
	#ifndef kQTC_SPIT_TrkSoundMax
		#define kQTC_SPIT_TrkSoundMax	65535
	#endif
#endif
#ifdef kQTC_SPIT_TrkMusic
	#ifndef kQTC_SPIT_TrkMusicMin
		#define kQTC_SPIT_TrkMusicMin	0
	#endif
	#ifndef kQTC_SPIT_TrkMusicMax
		#define kQTC_SPIT_TrkMusicMax	65535
	#endif
#endif
#ifdef kQTC_SPIT_TrkEyes
	#ifndef kQTC_SPIT_TrkEyesMin
		#define kQTC_SPIT_TrkEyesMin	0
	#endif
	#ifndef kQTC_SPIT_TrkEyesMax
		#define kQTC_SPIT_TrkEyesMax	65535
	#endif
#endif
#ifdef kQTC_SPIT_TrkEars
	#ifndef kQTC_SPIT_TrkEarsMin
		#define kQTC_SPIT_TrkEarsMin	0
	#endif
	#ifndef kQTC_SPIT_TrkEarsMax
		#define kQTC_SPIT_TrkEarsMax	65535
	#endif
#endif

#ifdef kQTC_SPIT_ShortFileTypeName
	#ifndef kQTC_SPIT_ShortFileTypeNameID
		#define kQTC_SPIT_ShortFileTypeName	kQTC_SPIT_ComponentResID+1
	#endif
#endif

#define ARRAY_SECTION


// Component Manager Thing
resource 'thng' (kQTC_SPIT_ComponentResID) 
	{
	'spit',											// Type
	kQTC_SPIT_Subtype,								// SubType
	kQTC_SPIT_Manufacturer,							// Manufacturer
	
	#if TARGET_REZ_MAC_68K
		kQTC_SPIT_Flags,							// Component flags
		0,											// Component flags Mask
		'spit',										// Code Type
		kQTC_SPIT_CodeID,							// Code ID
	#else
		0,											// - use componentHasMultiplePlatforms
		0,
		0,
		0,
	#endif

	'STR ',											// Name Type
	kQTC_SPIT_NameStringResID,						// Name ID
	
	
	'STR ',											// Info Type
	kQTC_SPIT_InfoStringResID,						// Info ID
	0,												// Icon Type
	0,												// Icon ID
	
	kQTC_SPIT_Version,
	
	componentHasMultiplePlatforms|					// Registration Flags
		componentDoAutoVersion,
	0,												// Resource ID of Icon Family
	
	ARRAY_SECTION
		{
		#if TARGET_OS_MAC							// COMPONENT PLATFORM INFORMATION ----------------------
			#if TARGET_REZ_CARBON_CFM
				kQTC_SPIT_Flags,					// Component Flags
				'cfrg',								// Special Case: data-fork based code fragment
				kQTC_SPIT_CodeID,	 				// Code ID usage for CFM components
				platformPowerPCNativeEntryPoint,	// Platform Type (response from gestaltComponentPlatform or failing that, gestaltSysArchitecture)
			#elif TARGET_REZ_CARBON_MACHO
				kQTC_SPIT_Flags, 
				'dlle',								// Code Resource type - Entry point found by symbol name 'dlle' resource
				kQTC_SPIT_CodeID,					// ID of 'dlle' resource
				platformPowerPCNativeEntryPoint,
			#elif TARGET_REZ_MAC_PPC
				kQTC_SPIT_Flags, 
				'spit',								// Code Type
				kQTC_SPIT_CodeID,					// Code ID
				platformPowerPC,
			#elif TARGET_REZ_MAC_68K
				kQTC_SPIT_Flags,
				'spit',
				kQTC_SPIT_CodeID,
				platform68k,
                        #elif TARGET_REZ_MAC_IA32
				kQTC_SPIT_Flags,
				'dlle',
				kQTC_SPIT_CodeID,
				platformIA32NativeEntryPoint,
			#else
				#error "At least one TARGET_REZ_XXX_XXX platform must be defined."
			#endif
		#endif
		
		#if TARGET_OS_WIN32
			kQTC_SPIT_Flags, 
			'dlle',
			kQTC_SPIT_CodeID,
			platformWin32,
		#endif
		},
		
	'thnr', 
	kQTC_SPIT_ThingResID							// Component public resource ID
	
	};

// Component name
resource 'STR ' (kQTC_SPIT_NameStringResID) 
	{
	kQTC_SPIT_NameString
	};

#if kQTC_SPIT_InfoStringResID
	// Component information
	resource 'STR ' (kQTC_SPIT_InfoStringResID) 
		{
		kQTC_SPIT_InfoString
		};
#endif

resource 'thnr' (kQTC_SPIT_ThingResID) 
	{
	ARRAY_SECTION
		{
		'src#', 1, 0,
		'src#', kQTC_SPIT_ThingResID, 0,

		'src#', 2, 0,
		'trk#', kQTC_SPIT_ThingResID, 0,

		#ifdef kQTC_SPIT_PresetListResID
			'stg#', 1, 0,
			'stg#', kQTC_SPIT_PresetListResID, 0,
		#endif
		
		#ifdef kQTC_SPIT_PresetStringListID
			'STR#', kQTC_SPIT_PresetStringListID, 0,
			'STR#', kQTC_SPIT_PresetStringListID, 0,
		#endif

		#ifdef kQTC_SPIT_AtomDataID01
			'sttg', 1, 0,
			'sttg', kQTC_SPIT_AtomDataID01, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID02
			'sttg', 2, 0,
			'sttg', kQTC_SPIT_AtomDataID02, 0,
		#endif		
		#ifdef kQTC_SPIT_AtomDataID03
			'sttg', 3, 0,
			'sttg', kQTC_SPIT_AtomDataID03, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID04
			'sttg', 4, 0,
			'sttg', kQTC_SPIT_AtomDataID04, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID05
			'sttg', 5, 0,
			'sttg', kQTC_SPIT_AtomDataID05, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID06
			'sttg', 6, 0,
			'sttg', kQTC_SPIT_AtomDataID06, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID07
			'sttg', 7, 0,
			'sttg', kQTC_SPIT_AtomDataID07, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID08
			'sttg', 8, 0,
			'sttg', kQTC_SPIT_AtomDataID08, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID09
			'sttg', 9, 0,
			'sttg', kQTC_SPIT_AtomDataID09, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID10
			'sttg', 10, 0,
			'sttg', kQTC_SPIT_AtomDataID10, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID11
			'sttg', 11, 0,
			'sttg', kQTC_SPIT_AtomDataID11, 0,
		#endif
		#ifdef kQTC_SPIT_AtomDataID12
			'sttg', 12, 0,
			'sttg', kQTC_SPIT_AtomDataID12, 0,
		#endif
		
		} 
	};
	

resource 'src#' (kQTC_SPIT_ThingResID) 
	{
	ARRAY_SECTION
		{
		#ifdef kQTC_SPIT_SrcVideo
			'vide', kQTC_SPIT_SrcVideoMin, kQTC_SPIT_SrcVideoMax, isSourceType,
		#endif
		#ifdef kQTC_SPIT_SrcSound
			'soun', kQTC_SPIT_SrcSoundMin, kQTC_SPIT_SrcSoundMax, isSourceType,
		#endif
		}
	};
	
resource 'trk#' (kQTC_SPIT_ThingResID)
	{
	ARRAY_SECTION
		{
		#ifdef kQTC_SPIT_TrkVideo
			'vide', kQTC_SPIT_TrkVideoMin, kQTC_SPIT_TrkVideoMax, isMediaType,
		#endif
		#ifdef kQTC_SPIT_TrkSound
			'soun', kQTC_SPIT_TrkVideoMin, kQTC_SPIT_TrkVideoMax, isMediaType,
		#endif
		#ifdef kQTC_SPIT_TrkMusic
			'musi', kQTC_SPIT_TrkVideoMin, kQTC_SPIT_TrkVideoMax, isMediaType,
		#endif
		#ifdef kQTC_SPIT_TrkEyes
			'eyes', kQTC_SPIT_TrkEyesMin, kQTC_SPIT_TrkEyesMax, isMediaCharacteristic,
		#endif
		#ifdef kQTC_SPIT_TrkEars
			'ears', kQTC_SPIT_TrkEarsMin, kQTC_SPIT_TrkEarsMax, isMediaCharacteristic,
		#endif
		}
	};
	
#ifdef kQTC_SPIT_PresetListResID
	// List of presets
	resource 'stg#' (kQTC_SPIT_PresetListResID) 
		{
		0,
		
		ARRAY_SECTION
			{
			#ifdef kQTC_SPIT_PresetString01
				'ps01', 0, 
				'sttg', 1, 
				kQTC_SPIT_PresetStringListID, 1, 
				0, 0,	
			#endif	
			#ifdef kQTC_SPIT_PresetDevider01
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString02
				'ps02', 0, 
				'sttg', 2, 
				kQTC_SPIT_PresetStringListID, 2, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider02
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString03
				'ps03', 0, 
				'sttg', 3, 
				kQTC_SPIT_PresetStringListID, 3, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider03
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif

			#ifdef kQTC_SPIT_PresetString04
				'ps04', 0, 
				'sttg', 4, 
				kQTC_SPIT_PresetStringListID, 4, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider04
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString05
				'ps05', 0, 
				'sttg', 5, 
				kQTC_SPIT_PresetStringListID, 5, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider05
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString06
				'ps06', 0, 
				'sttg', 6, 
				kQTC_SPIT_PresetStringListID, 6, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider06
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString07
				'ps07', 0, 
				'sttg', 7, 
				kQTC_SPIT_PresetStringListID, 7, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider07
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString08
				'ps08', 0, 
				'sttg', 8, 
				kQTC_SPIT_PresetStringListID, 8, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider08
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString09
				'ps09', 0, 
				'sttg', 9, 
				kQTC_SPIT_PresetStringListID, 9, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider09
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString10
				'ps10', 0, 
				'sttg', 10, 
				kQTC_SPIT_PresetStringListID, 10, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider10
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString11
				'ps11', 0, 
				'sttg', 11, 
				kQTC_SPIT_PresetStringListID, 11, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider11
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif
			
			#ifdef kQTC_SPIT_PresetString12
				'ps12', 0, 
				'sttg', 12, 
				kQTC_SPIT_PresetStringListID, 12, 
				0, 0,	
			#endif
			#ifdef kQTC_SPIT_PresetDevider12
				'----', 0,	0, 0,	0, 0,	0, 0,
			#endif			
			}		
		};
#endif	
	
#ifdef kQTC_SPIT_PresetStringListID
	// Strings for preset menu
	resource 'STR#' (kQTC_SPIT_PresetStringListID) 
		{
			{
			#ifdef kQTC_SPIT_PresetString01
				kQTC_SPIT_PresetString01,
			#endif
			#ifdef kQTC_SPIT_PresetString02
				kQTC_SPIT_PresetString02,
			#endif
			#ifdef kQTC_SPIT_PresetString03
				kQTC_SPIT_PresetString03,
			#endif
			#ifdef kQTC_SPIT_PresetString04
				kQTC_SPIT_PresetString04,
			#endif
			#ifdef kQTC_SPIT_PresetString05
				kQTC_SPIT_PresetString05,
			#endif
			#ifdef kQTC_SPIT_PresetString06
				kQTC_SPIT_PresetString06,
			#endif
			#ifdef kQTC_SPIT_PresetString07
				kQTC_SPIT_PresetString07,
			#endif
			#ifdef kQTC_SPIT_PresetString08
				kQTC_SPIT_PresetString08,
			#endif
			#ifdef kQTC_SPIT_PresetString09
				kQTC_SPIT_PresetString09,
			#endif
			#ifdef kQTC_SPIT_PresetString10
				kQTC_SPIT_PresetString10,
			#endif
			#ifdef kQTC_SPIT_PresetString11
				kQTC_SPIT_PresetString11,
			#endif
			#ifdef kQTC_SPIT_PresetString12
				kQTC_SPIT_PresetString12,
			#endif
			}
		};
#endif
	
// Atom data
#ifdef kQTC_SPIT_AtomDataID01
	data 'sttg' (kQTC_SPIT_AtomDataID01, kQTC_SPIT_AtomDataName01)
		{
		kQTC_SPIT_AtomData01
		};
#endif
#ifdef kQTC_SPIT_AtomDataID02
	data 'sttg' (kQTC_SPIT_AtomDataID02, kQTC_SPIT_AtomDataName02)
		{
		kQTC_SPIT_AtomData02
		};
#endif
#ifdef kQTC_SPIT_AtomDataID03
	data 'sttg' (kQTC_SPIT_AtomDataID03, kQTC_SPIT_AtomDataName03)
		{
		kQTC_SPIT_AtomData03
		};	
#endif
#ifdef kQTC_SPIT_AtomDataID04
	data 'sttg' (kQTC_SPIT_AtomDataID04, kQTC_SPIT_AtomDataName04)
		{
		kQTC_SPIT_AtomData04
		};
#endif
#ifdef kQTC_SPIT_AtomDataID05
	data 'sttg' (kQTC_SPIT_AtomDataID01, kQTC_SPIT_AtomDataName05)
		{
		kQTC_SPIT_AtomData05
		};
#endif
#ifdef kQTC_SPIT_AtomDataID06
	data 'sttg' (kQTC_SPIT_AtomDataID01, kQTC_SPIT_AtomDataName06)
		{
		kQTC_SPIT_AtomData06
		};
#endif
#ifdef kQTC_SPIT_AtomDataID07
	data 'sttg' (kQTC_SPIT_AtomDataID07, kQTC_SPIT_AtomDataName07)
		{
		kQTC_SPIT_AtomData07
		};
#endif
#ifdef kQTC_SPIT_AtomDataID08
	data 'sttg' (kQTC_SPIT_AtomDataID08, kQTC_SPIT_AtomDataName08)
		{
		kQTC_SPIT_AtomData08
		};
#endif
#ifdef kQTC_SPIT_AtomDataID09
	data 'sttg' (kQTC_SPIT_AtomDataID09, kQTC_SPIT_AtomDataName09)
		{
		kQTC_SPIT_AtomData09
		};
#endif
#ifdef kQTC_SPIT_AtomDataID10
	data 'sttg' (kQTC_SPIT_AtomDataID10, kQTC_SPIT_AtomDataName10)
		{
		kQTC_SPIT_AtomData10
		};
#endif
#ifdef kQTC_SPIT_AtomDataID11
	data 'sttg' (kQTC_SPIT_AtomDataID01, kQTC_SPIT_AtomDataName11)
		{
		kQTC_SPIT_AtomData11
		};
#endif
#ifdef kQTC_SPIT_AtomDataID12
	data 'sttg' (kQTC_SPIT_AtomDataID01, kQTC_SPIT_AtomDataName12)
		{
		kQTC_SPIT_AtomData12
		};
#endif
	
#ifdef kQTC_SPIT_ShortFileTypeNameID
	resource 'STR#' (kQTC_SPIT_ShortFileTypeNameID)
		{
			{
			kQTC_SPIT_ShortFileTypeName
			}
		};
#endif	
	
#if TARGET_REZ_CARBON_CFM
	// Custom extended code fragment resource
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
				kQTC_SPIT_MemberName,				// member name
				
				// Start of extended info.
				
				'cpnt',								// libKind (not kFragComponentMgrComponent == 'comp' as you might expect)
				kQTC_SPIT_Qualifier1,				// qualifier 1 - hex value derived from the string matches Code ID in 'thng'
				"",									// qualifier 2
				"",									// qualifier 3
				kQTC_SPIT_InternalName,				// intlName, localised
				};
			};
		};
#endif


#if	TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32 || TARGET_REZ_WIN32
	// Code Entry Point for Mach-O and Windows
	resource 'dlle' (kQTC_SPIT_CodeID) 
		{
		kQTC_SPIT_ComponentDispatch
		};
#endif	
	
	

