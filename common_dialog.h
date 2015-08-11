#ifdef		INCLUDE_DIALOG

#define		NO_DELETE_GROUPS	1
#define		NO_DELETE_ITEMS		2
#define		NO_EDIT_GROUPS		4
#define		NO_EDIT_ITEMS		8
#define		NO_EDIT_VALUES		16
#define		NO_REFRESH			32
#define		NO_ADD_GROUP		64
#define		NO_ADD_ITEM			128
#define		NO_AUTO_REFRESH		256
#define		PAUSE_MMF			512

#define		MAX_FORMAT_MODE		7
std::string FormatTreeItem( int setting , std::string itemName , std::string data );

#define		DEBUG_MODE_TITLE	1024

//Display modes.
#define		INI_DISPLAY			0
#define		ITEM_NAME_ONLY		1
#define		BRACKET_DISPLAY		2
#define		VALUE_ONLY			3
#define		ENGLISH_STYLE1		4
#define		ENGLISH_STYLE2		5
#define		ITEM_COLON_VALUE	6
#define		FRENCH_STYLE		7

#endif