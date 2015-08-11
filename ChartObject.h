namespace ChartObject
{

	const int Number_Of_Dataslots = 9;

	enum ChartType
	{BAR = 0,THINBAR,PICTOGRAPH,XY_DISCRETE,XY_CTS,	RADAR,PIE};

	enum AutoType
	{RANGE_USERDEFINED = 0,RANGE_AUTO,RANGE_AUTOZERO,RANGE_AUTOCUSTOM,RANGE_SCROLL};

	enum LineType
	{NONE = 0,STRAIGHT,CURVE,SQUARETOOTH,LINEOFBESTFIT
	};
	enum MarkerType
	{None = 0,Image,Cross,Circle,Dot,Square,Box,HDash,VDash	};

struct ChartProperties
	{
		ChartType	chartType;
		//General Display
		bool			horizontal;	
		bool			antialias;
		bool			transparent;

		//Scale values
		float			minValueUser;
		float			maxValueUser;
		AutoType		autoValues;

		//Continuous only scale values
		float			minSeriesUser;	
		float			maxSeriesUser;	
		AutoType		autoSeries;

		//These are the ones that are used at runtime
		//(It is only 16 bytes to have it here at edittime too)
		float			minValue;
		float			maxValue;
		float			minSeries;	
		float			maxSeries;	


		//Custom markers and lines
		MarkerType		markerType			[Number_Of_Dataslots];
		COLORREF		markerColour		[Number_Of_Dataslots];
		int				markerSize			[Number_Of_Dataslots];
		int				markerThickness		[Number_Of_Dataslots];
		bool			markerOverAxis;
		LineType		lineType			[Number_Of_Dataslots];
		COLORREF		lineColour			[Number_Of_Dataslots];
		int				lineThickness		[Number_Of_Dataslots];


		//Scale intervals
		int				labelValueInterval;			//cts only
		int				labelSeriesInterval;
		float			tickSeriesInterval;	// for line graphs
		float			tickValueInterval;

		//Floating point decimal places
		int				labelValuePrecision;
		int				labelSeriesPrecision;	//cts only
		int				labelInvValuePrecision;	//For individual labels

		//Rendering and display
		int				axisThickness;
		int				tickSeriesSize;
		int				tickValueSize;
		int				barWidth;
		float			paddingWidth;
		bool			displayValue;	// bar/line/picto, display value alongside bar
		LOGFONT			displayValueFont;
		COLORREF		displayValueColour;
		int				labelValuePadding;

		//Title
		bool			titleShow;
		char			titleText[128];
		COLORREF		titleColour;
		LOGFONT			titleFont;
		bool			titleShadow;
		int				titlePadding;

		//Axis titles	(not pie chart, etc.)
		bool			axisTitleSeries;
		bool			axisTitleValue;
		char			axisTitleSeriesText[128];
		char			axisTitleValueText[128];
		COLORREF		axisTitleColour;
		LOGFONT			axisTitleFont;

		//Padding
		int				padLeft;
		int				padTop;
		int				padRight;
		int				padBottom;

		//Fonts and colours
		COLORREF		backgroundColour;
		COLORREF		axisColour;
		COLORREF		radarColour;		// For radar only
		int				radarOpacity;		// For radar only
		COLORREF		gridSeriesColour;
		COLORREF		gridValueColour;
		int				valueOutlineThickness;
		COLORREF		valueOutlineColour;
		COLORREF		labelSeriesColour;
		COLORREF		labelValueColour;
		LOGFONT			labelSeriesFont;
		LOGFONT			labelValueFont;
		bool			labelSeries;
		bool			labelValue;

		//Pie-chart only
		int				pie3DHeight;
		float			rotation;

		//Grid lines
		float			gridXInterval;		//cts only
		float			gridYInterval;
		int				gridSeriesThickness;
		int				gridValueThickness;

		//Borders
		int				borderThickness;
		COLORREF		borderColour;

		//New features
		bool			autoTicks;
		bool			vPadding;
		bool			hPadding;
		bool			gridXShow;
		bool			gridYShow;
		
		//Even newer feature	
		bool			autoTicksX;
		bool			rev2;
		bool			rev3;

		int				labelValuePrecisionUser;
		int				labelSeriesPrecisionUser;	//cts only

		//Reserved for future use
		int				reserved[60];
	};


//======================================================================================
struct ItemInfo
{
	int flags;
	int number;
	float y;
	float x;
	COLORREF col1;	//Flag: 1
	COLORREF col2;	//Flag: 2
	char* label;	//Flag:	4
};

typedef bool (*Function2) (headerObject *rdPtr , int x , int y , COLORREF color , COLORREF color2, const char* label , int flags );
typedef bool (*Function3) (headerObject *rdPtr , int num , ItemInfo* dest );
typedef bool (*Function4) (headerObject *rdPtr , bool getset , ChartObject::ChartProperties* props );


struct CHART_RUNDATA
{
	// Main header - required
	headerObject	rHo;					// Header

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
	rCom			rc;				// Common structure for movements & animations
	rMvt			rm;				// Movements
	rSpr			rs;				// Sprite (displayable objects)

	//=======================================
	// Chart Object API
	//=======================================
	Function2		setPoint;
	Function3		getPoint;
	Function4		getSetProperties;


	// Object's runtime data
	void			*c;

	int				imageCount;
	WORD			imageID[50];

};

#define MAKEID(a,b,c,d) ((#@a << 24)|(#@b << 16)|(#@c << 8)|(#@d)) 
#define CHARTID MAKEID(C,H,R,T)


CHART_RUNDATA* FindChartObject(LPCSTR pName , LPRDATA rdPtr )
{
	LPRDATA r = NULL;
	#ifdef RUN_ONLY
		r = FindObject(pName,rdPtr,CHARTID,NULL);
	#else
		r = FindObject(pName,rdPtr,CHARTID,"Error: The Chart object you have selected does not exist. The action will fail.\n\nThis error message will not be shown when the program is built." );
	#endif
	if ( r != 0 )
		return (CHART_RUNDATA*)r;
	return NULL;
}

#undef MAKEID
#undef CHARTID








};
