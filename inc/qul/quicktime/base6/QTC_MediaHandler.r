/**
 * === QuickTime Utility ===
 *  Unit Name:  QTC_MediaHandler.r
 *  Author:     Lincoln Yu
 *  Purpose:    QuickDraw Media-handler resource file
 *  
 * == History ==        
 *	2006-02-10:	Created.
 *      2006-06-19:     Mac-IA32 support added.
 * == Comments == 
 *  The following parametric macros are needed:
 *		kQTC_MH_Name					- name of the media-handler, a string
 *		kQTC_MH_Subtype					- subtype, a four char code
 *		kQTC_MH_Manufacturer			- component manufacturer, a four char code
 *		kQTC_MH_Flags					- traits, a 32-bit unsigned value
 *		kQTC_MH_ComponentResID			- component resource ID
 *		kQTC_MH_NameStringResID			- name string resource ID
 *		kQTC_MH_NameString				- name string
 *		[kQTC_MH_CodeID]				- code ID, equal to resource ID by default
 *		[kQTC_MH_InfoStringResID]		- info string resource ID, 0 for N/A by default
 *		[kQTC_MH_InfoString]			- info string, available when resource ID is nonzero
 *		[kQTC_MH_IconResID]				- icon resource ID, 0 for N/A by default
 *		[kQTC_MH_IconData]				- icon data file, might be avaiable when resource ID is nonzero
 *		kQTC_MH_Version					- version number of the media-handler, a 32-bit unsigned value
 *		kQTC_MH_MemberName				- member name, a string
 *		kQTC_MH_InternalName			- internal name, a string
 *		kQTC_MH_ComponentDispatch		- entry of component dispatch, a string
 *
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */


// Use the platform info array in the 'thng' resource for 68K and PowerPC, or Windows
#define thng_RezTemplateVersion			1

// Header files
#if TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32
    #include <Carbon/Carbon.r>
    #include <QuickTime/QuickTime.r>
#else
	#include "MacTypes.r"
	#include "Components.r"
	#include "Icons.r"
#endif

/* Set default IDs */
#ifndef kQTC_MH_NameStringResID
	#define kQTC_MH_NameStringResID		kQTC_MH_ComponentResID
#endif
#ifndef kQTC_MH_CodeID
	#define kQTC_MH_CodeID				kQTC_MH_ComponentResID
#endif
#ifndef kQTC_MH_InfoStringResID
	#ifdef kQTC_MH_InfoString
		#define kQTC_MH_InfoStringResID	(kQTC_MH_NameStringResID+1)
	#else	
		#define kQTC_MH_InfoStringResID	0
	#endif
#endif
#ifndef kQTC_MH_IconResID
	#ifdef kQTC_MH_IconData
		#define kQTC_MH_IconResID		kQTC_MH_ComponentResID
	#else
		#define kQTC_MH_IconResID		0
	#endif
#endif

#define ARRAY_SECTION

// Component resource for QuickDraw Media-handler
resource 'thng' (kQTC_MH_ComponentResID, kQTC_MH_Name, purgeable) 
	{
	'mhlr',											// Component type
	kQTC_MH_Subtype,								// Component subtype
	kQTC_MH_Manufacturer,							// Component manufacturer
		
	#if TARGET_REZ_MAC_68K
		kQTC_MH_Flags,								// Component flags
		0,											// Component flags mask
		'mhlr',										// Code type
		kQTC_MH_CodeID,								// Code ID
	#else
		kQTC_MH_Flags,										
		0x00000000,										
		0,											// No 68K code
		0,											// No 68K code
	#endif

	'STR ',											// Component name resource type
	kQTC_MH_NameStringResID,						// Component name resource ID
	
	#if kQTC_MH_InfoStringResID
		'STR ',										// Component info resource type
	#else
		0,
	#endif
	kQTC_MH_InfoStringResID,						// Component info resource ID

	#if kQTC_MH_IconResID
		'ICON',										// Component icon resource type
	#else
		0,
	#endif
	kQTC_MH_IconResID,								// Component icon resource ID
	
	kQTC_MH_Version,
	
	componentHasMultiplePlatforms|					// Registration Flags
		componentDoAutoVersion,
	0,												// Resource ID of Icon Family

	ARRAY_SECTION
		{
		#if TARGET_OS_MAC
			#if TARGET_REZ_CARBON_CFM
				kQTC_MH_Flags, 'cfrg', kQTC_MH_CodeID, platformPowerPC
			#elif TARGET_REZ_CARBON_MACHO
				kQTC_MH_Flags, 'dlle', kQTC_MH_CodeID, platformPowerPCNativeEntryPoint
			#elif TARGET_REZ_MAC_PPC
				kQTC_MH_Flags, 'mhlr', kQTC_MH_CodeID, platformPowerPC
			#elif TARGET_REZ_MAC_68K
				kQTC_MH_Flags, 'mhlr', kQTC_MH_CodeID, platform68k
                        #elif TARGET_REZ_MAC_IA32
				kQTC_MH_Flags, 'dlle', kQTC_MH_CodeID, platformIA32NativeEntryPoint,
			#else
				#error "At least one TARGET_REZ_XXX_XXX platform must be defined."
			#endif
		#else
			kQTC_MH_Flags, 'dlle', kQTC_MH_CodeID, platformWin32
		#endif
		}
	};


// Component name
resource 'STR ' (kQTC_MH_NameStringResID, kQTC_MH_Name, purgeable) 
	{
	kQTC_MH_NameString
	};


#if kQTC_MH_InfoStringResID
	// Component info
	resource 'STR ' (kQTC_MH_InfoStringResID, kQTC_MH_Name, purgeable) 
		{
		kQTC_MH_InfoString
		};
#endif


#if kQTC_MH_IconResID
	// Icon
	resource 'ICON' (kQTC_MH_IconResID, kQTC_MH_Name, purgeable) 
		{
		#ifdef kQTC_MH_IconData
			#include kQTC_MH_IconData
		#else 
			$"00000000000000000000000000000000"
			$"00000000003FC00001C0380002040400"
			$"022A840003915F80027FE44002000720"
			$"020004A0020004A0020004A0020004A0"
			$"020004A0020007200200044002000780"
			$"0200040001C03800003FC00000001000"
			$"00888000002020000085400000100000"
			$"0042"
		#endif
		};
#endif


#if TARGET_REZ_CARBON_CFM
	// Custom extended code fragment resource
	// CodeWarrior will correctly adjust the offset and length of each
	// Code fragment when building a MacOS Merge target
	resource 'cfrg' (0) 
		{
		ARRAY_SECTION
			{		
			extendedEntry 
				{
				kPowerPCCFragArch,					// ArchType
				kIsCompleteCFrag,					// UpdateLevel
				kNoVersionNum,						// CurrentVersion
				kNoVersionNum,						// OldDefVersion
				kDefaultStackSize,					// AppStackSize
				kNoAppSubFolder,					// AppSubFolderID
				kImportLibraryCFrag,				// Usage
				kDataForkCFragLocator,				// Where
				kZeroOffset,						// Offset
				kCFragGoesToEOF,					// Length
				kQTC_MH_MemberName,					// Member name
				
				// Start of extended info.
				
				'cpnt',								// LibKind (not kFragComponentMgrComponent == 'comp' as you might expect)
				"\0x02\0x00",						// Qualifier 1 - hex 0x0200 (512) matches Code ID in 'thng'
				"",									// Qualifier 2
				"",									// Qualifier 3
				kQTC_MH_InternalName,				// IntlName, localised
				};
			};
		};
#endif


#if	TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32 || TARGET_REZ_WIN32
	resource 'dlle' (kQTC_MH_CodeID, kQTC_MH_Name) 
		{
		kQTC_MH_ComponentDispatch
		};
#endif


