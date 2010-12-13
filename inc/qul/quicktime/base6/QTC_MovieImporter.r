/**
 * === GalaxyCodec ===
 *  Unit Name:  QTC_MovieImporter.r
 *  Author:     Lincoln Yu
 *  Purpose:    QuickTime Movie-importer component resource file
 * == History ==        
 *	2006-02-10:	Created.
 *      2006-06-19:     Mac-IA32 support added.
 * == Comments ==
 *  The following parametric macros are needed:
 *		kQTC_EAT_Name					- NOT USED, name of the media-handler, a string
 *		kQTC_EAT_Subtype				- subtype, a four char code
 *		kQTC_EAT_Manufacturer			- component manufacturer, a four char code
 *		kQTC_EAT_Flags					- traits, a 32-bit unsigned value
 *		kQTC_EAT_ComponentResID			- component resource ID
 *		[kQTC_EAT_ComponentAliasID]		- component resource ID, one bigger to component resource ID by default
 *		[kQTC_EAT_NameStringResID]		- name string resource ID, equal to component resource ID by default
 *		[kQTC_EAT_CodeID]				- code ID, equal to component resource ID by default
 *		[kQTC_EAT_MimeInfoID]			- mime info ID, equal to component resource ID by default
 *		[kQTC_EAT_ConfigID]				- media config ID, equal to component resource ID by default
 *		[kQTC_EAT_ThingResID]			- thing resource ID, equal to component resource ID by default
 *		kQTC_EAT_NameString				- name string
 *		kQTC_EAT_Version				- version number of the media-handler, a 32-bit unsigned value
 *		[kQTC_EAT_FileType]				- MacOS file type when saved, a four char code, equal to subtype by default
 *		kQTC_EAT_FileCreator			- MacOS file creator when saved, a four char code
 *		kQTC_EAT_FileExts				- a string indicating file extension(s), comma delimited if more than one
 *		kQTC_EAT_MissingSoftDesc		- missing software description for the missing software dialog
 *		kQTC_EAT_VendorInfo				- vendor info string (copyright, version, etc)
 *		kQTC_EAT_DefaultFileExt			- Default file extension (OSType) which must be in uppercase.
 *		kQTC_EAT_MimeMediaType			- media type description for MIME configuration panel and browser
 *		kQTC_EAT_MimeTypeTag(1~4)		- mime type
 *		kQTC_EAT_MimeFileExtTag(1~4)	- mime file extension
 *		kQTC_EAT_MimeDescTag(1~4)		- mime description
 *		kQTC_EAT_Qualifier1				- qualifier 1, hex value derived from the string matches [:kQTC_EAT_ComponentResID:]
 *		kQTC_EAT_MemberName				- member name, a string
 *		kQTC_EAT_InternalName			- internal name, a string
 *		kQTC_EAT_ComponentDispatch		- entry of component dispatch, a string 
 * == Statements ==
 *  Copyright 2005 HDIC. All rights reserved.
 *
 */


/**
 * == Comments ==
 * [:thng_RezTemplateVersion:]
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
    #include "MacTypes.r"
    #include "Components.r"
    #include "QuickTimeComponents.r"
    #include "CodeFragments.r"
	#undef __COMPONENTS_R__
#endif


/* Set default IDs */
#ifndef kQTC_EAT_NameStringResID
	#define kQTC_EAT_NameStringResID	kQTC_EAT_ComponentResID
#endif
#ifndef kQTC_EAT_ComponentAliasID
	#define kQTC_EAT_ComponentAliasID	kQTC_EAT_ComponentResID+1
#endif
#ifndef kQTC_EAT_CodeID
	#define kQTC_EAT_CodeID				kQTC_EAT_ComponentResID
#endif
#ifndef kQTC_EAT_MimeInfoID
	#define kQTC_EAT_MimeInfoID			kQTC_EAT_ComponentResID
#endif
#ifndef kQTC_EAT_ConfigID
	#define kQTC_EAT_ConfigID			kQTC_EAT_ComponentResID
#endif
#ifndef kQTC_EAT_ThingResID
	#define kQTC_EAT_ThingResID			kQTC_EAT_ComponentResID
#endif

#ifndef kQTC_EAT_FileType
	#define kQTC_EAT_FileType			kQTC_EAT_Subtype
#endif

#define ARRAY_SECTION

// Component Manager Thing
resource 'thng' (kQTC_EAT_ComponentResID) 
	{
	'eat ',											// Type
	kQTC_EAT_Subtype,								// SubType
	kQTC_EAT_Manufacturer,							// Manufacturer
	
	#if TARGET_REZ_MAC_68K
		kQTC_EAT_Flags,								// Component flags
		0,											// Component flags Mask
		'eat ',										// Code Type
		kQTC_EAT_CodeID,							// Code ID
	#else
		0,											// - use componentHasMultiplePlatforms
		0,
		0,
		0,
	#endif

	'STR ',											// Name Type
	kQTC_EAT_NameStringResID,						// Name ID
	
	0,												// Info Type
	0,												// Info ID
	0,												// Icon Type
	0,												// Icon ID
	
	kQTC_EAT_Version,
	
	componentHasMultiplePlatforms|					// Registration Flags
		componentDoAutoVersion,
	0,												// Resource ID of Icon Family
	
	ARRAY_SECTION
		{
		#if TARGET_OS_MAC							// COMPONENT PLATFORM INFORMATION ----------------------
			#if TARGET_REZ_CARBON_CFM
				kQTC_EAT_Flags,						// Component Flags
				'cfrg',								// Special Case: data-fork based code fragment
				kQTC_EAT_CodeID,	 				// Code ID usage for CFM components
				platformPowerPCNativeEntryPoint,	// Platform Type (response from gestaltComponentPlatform or failing that, gestaltSysArchitecture)
			#elif TARGET_REZ_CARBON_MACHO
				kQTC_EAT_Flags, 
				'dlle',								// Code Resource type - Entry point found by symbol name 'dlle' resource
				kQTC_EAT_CodeID,					// ID of 'dlle' resource
				platformPowerPCNativeEntryPoint,
			#elif TARGET_REZ_MAC_PPC
				kQTC_EAT_Flags, 
				'eat ',								// Code Type
				kQTC_EAT_CodeID,					// Code ID
				platformPowerPC,
			#elif TARGET_REZ_MAC_68K
				kQTC_EAT_Flags,
				'eat ',
				kQTC_EAT_CodeID,
				platform68k,
			#elif TARGET_REZ_MAC_IA32
				kQTC_EAT_Flags,
				'dlle',
				kQTC_EAT_CodeID,
				platformIA32NativeEntryPoint,
			#else
				#error "At least one TARGET_REZ_XXX_XXX platform must be defined."
			#endif
		#endif
		
		#if TARGET_OS_WIN32
			kQTC_EAT_Flags, 
			'dlle',
			kQTC_EAT_CodeID,
			platformWin32,
		#endif
		},
		
	'thnr', 
	kQTC_EAT_ThingResID								// Component public resource ID
	
	};

// Component Alias
resource 'thga' (kQTC_EAT_ComponentAliasID) 
	{
	'eat ',											// Type
	kQTC_EAT_DefaultFileExt, 						// Subtype - this must be in uppercase. It will match the suffix case-insensitively.
	kQTC_EAT_Manufacturer,							// Manufaturer - for 'eat ' the media type supported by the component
	kQTC_EAT_Flags |								// Component Flags
		movieImportSubTypeIsFileExtension,			// The subtype is a file name suffix
	0,												// Component Flags Mask
	0, 												// Code Type
	0,												// Code ID
	'STR ',											// Name Type
	kQTC_EAT_NameStringResID,						// Name ID
	0,												// Info Type
	0,												// Info ID 
	0,												// Icon Type 
	0,												// Icon ID
		
													// ---------- TARGET COMPONENT ----------
	'eat ',											// Type
	kQTC_EAT_Subtype,								// SubType
	kQTC_EAT_Manufacturer,							// Manufaturer
	0,												// Component Flags
	0,												// Component Flags Mask
	'thnr',
	kQTC_EAT_ThingResID,							// Component public resource ID
	cmpAliasOnlyThisFile

	};

resource 'thnr' (kQTC_EAT_ThingResID) 
	{
	ARRAY_SECTION
		{
		'mime', 1, 0, 
		'mime', kQTC_EAT_MimeInfoID, 0,
		
		'mcfg', 1, 0,
		'mcfg', kQTC_EAT_ConfigID, 0
		} 
	};


// QuickTime Media Configuration Resources 
resource 'mcfg' (kQTC_EAT_ConfigID)
	{
	kVersionDoesntMatter,							// Version of the component this applies to
	
	ARRAY_SECTION
		{
		kQTMediaConfigVideoGroupID,					// The ID of the group this belongs with
		
		// MIME config flags
		kQTMediaConfigUseAppByDefault				// By default, associate with application specified below instead of the QuickTime plug-in
			| kQTMediaConfigCanUseApp				// This type can be associated with an application
			| kQTMediaConfigCanUsePlugin			// This type can be associated with the QuickTime plug-in
			| kQTMediaConfigBinaryFile,				// The file is binary, not just text

		kQTC_EAT_FileType,							// MacOS file type when saved (OSType)
		kQTC_EAT_FileCreator,						// MacOS file creator when saved (OSType)

		// Component information, used by the QuickTime plug-in to find the component to open this type of file
		'eat ',										// Component type (OSType)
		kQTC_EAT_Subtype,							// Component subtype (OSType)
		kQTC_EAT_Manufacturer,						// Component manufacturer (OSType)
		kQTC_EAT_Flags,								// Component flags
		0, 											// Flags mask

		kQTC_EAT_DefaultFileExt,					// Default file extension (OSType) - this must be in uppercase. It will match the suffix case-insensitively.
		kQTMediaInfoNetGroup,						// QT file type group (OSType, one of kQTMediaInfoNetGroup, etc.)

		// Media type synonyms, an array of zero or more Pascal strings - none here
		ARRAY_SECTION
			{
			},

		ARRAY_SECTION
			{
			kQTC_EAT_MimeMediaType,					// Media type description for MIME configuration panel and browser
			kQTC_EAT_FileExts,						// File extension(s), comma delimited if more than one
			"QuickTime Player",						// Opening application name for MIME configuration panel and browser
			kQTC_EAT_MissingSoftDesc,				// Missing software description for the missing software dialog
			kQTC_EAT_VendorInfo,					// Vendor info string (copyright, version, etc)
			},
		
		// Array of one or more MIME types that describe this media type (eg. audio/mpeg, audio/x-mpeg, etc.)
		ARRAY_SECTION
			{
			kQTC_EAT_MimeTypeTag1,
			#ifdef kQTC_EAT_MimeTypeTag2			
				kQTC_EAT_MimeTypeTag2,	
				#ifdef kQTC_EAT_MimeTypeTag3			
					kQTC_EAT_MimeTypeTag3,	
					#ifdef kQTC_EAT_MimeTypeTag4			
						kQTC_EAT_MimeTypeTag4,	
					#endif
				#endif
			#endif
			},
		}
	};

// Component name
resource 'STR ' (kQTC_EAT_NameStringResID) 
	{
	kQTC_EAT_NameString
	};

// MIME info
resource 'mime' (kQTC_EAT_MimeInfoID) 
	{
	ARRAY_SECTION
		{
		kMimeInfoMimeTypeTag, 1, kQTC_EAT_MimeTypeTag1;
		#ifdef kQTC_EAT_MimeTypeTag2
			kMimeInfoMimeTypeTag, 2, kQTC_EAT_MimeTypeTag2;
			#ifdef kQTC_EAT_MimeTypeTag3
				kMimeInfoMimeTypeTag, 3, kQTC_EAT_MimeTypeTag3;
				#ifdef kQTC_EAT_MimeTypeTag4
					kMimeInfoMimeTypeTag, 4, kQTC_EAT_MimeTypeTag4;
				#endif
			#endif
		#endif
		
		kMimeInfoFileExtensionTag, 1, kQTC_EAT_MimeFileExtTag1;
		#ifdef kQTC_EAT_MimeFileExtTag2
			kMimeInfoFileExtensionTag, 2, kQTC_EAT_MimeFileExtTag2;
			#ifdef kQTC_EAT_MimeFileExtTag3
				kMimeInfoFileExtensionTag, 3, kQTC_EAT_MimeFileExtTag3;
				#ifdef kQTC_EAT_MimeFileExtTag4
					kMimeInfoFileExtensionTag, 4, kQTC_EAT_MimeFileExtTag4;
				#endif				
			#endif
		#endif
		
		kMimeInfoDescriptionTag, 1, kQTC_EAT_MimeDescTag1;
		#ifdef kQTC_EAT_MimeDescTag2
			kMimeInfoDescriptionTag, 2, kQTC_EAT_MimeDescTag2;
			#ifdef kQTC_EAT_MimeDescTag3
				kMimeInfoDescriptionTag, 3, kQTC_EAT_MimeDescTag3;
				#ifdef kQTC_EAT_MimeDescTag4
					kMimeInfoDescriptionTag, 4, kQTC_EAT_MimeDescTag4;
				#endif
			#endif
		#endif
		};
	};


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
				kQTC_EAT_MemberName,				// member name
				
				// Start of extended info.
				
				'cpnt',								// libKind (not kFragComponentMgrComponent == 'comp' as you might expect)
				kQTC_EAT_Qualifier1,				// qualifier 1 - hex value derived from the string matches Code ID in 'thng'
				"",									// qualifier 2
				"",									// qualifier 3
				kQTC_EAT_InternalName,				// intlName, localised
				};
			};
		};
#endif


#if	TARGET_REZ_CARBON_MACHO || TARGET_REZ_MAC_IA32 || TARGET_REZ_WIN32
	// Code Entry Point for Mach-O and Windows
	resource 'dlle' (kQTC_EAT_CodeID) 
		{
		kQTC_EAT_ComponentDispatch
		};
#endif

