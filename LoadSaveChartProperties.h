#define BAR				(p.chartType == ChartObject::BAR )
#define PIE				(p.chartType == ChartObject::PIE )
#define RADAR			(p.chartType == ChartObject::RADAR )
#define CTS				(p.chartType == ChartObject::XY_CTS)
#define LINE			(p.chartType == ChartObject::XY_DISCRETE ||CTS)
#define SCATTER			(p.chartType == ChartObject::SCATTER )
#define AXIS			(!(PIE||RADAR))		

		INT2( true , chartType , ChartObject::ChartType )

		INT( AXIS , horizontal )
		BOOL( true , antialias )
		BOOL( true , transparent )
		
		FLOAT( AXIS , minValueUser )
		FLOAT( AXIS , maxValueUser )
		INT2( AXIS , autoValues , ChartObject::AutoType)

		FLOAT( CTS , minSeriesUser )
		FLOAT( CTS , maxSeriesUser )
		INT2( CTS , autoSeries , ChartObject::AutoType )

		BOOL( LINE , markerOverAxis )
		#define MULTIPLES(i) INT2( LINE , markerType[i] , ChartObject::MarkerType )\
		INT( LINE , markerColour[i] )\
		INT( LINE , markerSize[i] )\
		INT( LINE , markerThickness[i] )\
		INT2( LINE , lineType[i] , ChartObject::LineType )\
		INT( LINE , lineThickness[i] )\
		INT( LINE , lineColour[i] )

			MULTIPLES(0)
			MULTIPLES(1)
			MULTIPLES(2)
			MULTIPLES(3)
			MULTIPLES(4)
			MULTIPLES(5)
			MULTIPLES(6)
			MULTIPLES(7)
			MULTIPLES(8)

		#undef MULTIPLES

		INT( AXIS , labelValueInterval )
		INT( AXIS , labelSeriesInterval )
		FLOAT( CTS , tickSeriesInterval )
		FLOAT( AXIS , tickValueInterval )

		INT( true , labelValuePrecision )
		INT( AXIS , labelSeriesPrecision )
		INT( true , labelInvValuePrecision )

		INT( AXIS , axisThickness )
		INT( AXIS , tickSeriesSize )
		INT( AXIS , tickValueSize )
		INT( BAR , barWidth )
		FLOAT( BAR , paddingWidth )
		BOOL( true , displayValue )
		FONT( true , displayValueFont )
		INT( true , displayValueColour )
		INT(true,labelValuePadding)

		BOOL(true,titleShow)
		STRING(true,titleText)
		INT(true,titleColour)
		FONT(true,titleFont)
		BOOL(true,titleShadow)
		INT(true,titlePadding)

		BOOL( AXIS, axisTitleSeries )
		BOOL( AXIS, axisTitleValue )
		STRING( AXIS, axisTitleSeriesText )
		STRING( AXIS, axisTitleValueText )
		INT(AXIS,axisTitleColour)
		FONT(AXIS,axisTitleFont)

		INT(true,padLeft)
		INT(true,padRight)
		INT(true,padTop)
		INT(true,padBottom)

		INT(true,backgroundColour)

		INT(AXIS,axisColour)
		INT(RADAR,radarColour)
		INT(RADAR,radarOpacity)
		INT(AXIS,gridSeriesColour)
		INT(AXIS,gridValueColour)
		INT(true,valueOutlineThickness)
		INT(true,valueOutlineColour)
		INT(AXIS,labelSeriesColour)
		INT(true,labelValueColour)
		FONT(AXIS,labelSeriesFont)
		FONT(true,labelValueFont)
		BOOL(AXIS,labelSeries)
		BOOL(true,labelValue)
		INT(PIE,pie3DHeight)
		FLOAT(!AXIS,rotation)

		FLOAT(AXIS,gridXInterval)
		FLOAT(AXIS,gridYInterval)
		INT(AXIS,gridSeriesThickness)
		INT(AXIS,gridValueThickness)

		INT(true,borderThickness)
		INT(true,borderColour)

		INT(AXIS,autoTicks)
		INT(AXIS,vPadding)
		INT(AXIS,hPadding)
		INT(AXIS,gridXShow)
		INT(AXIS,gridYShow)

		INT(AXIS,autoTicksX)
		INT(true,labelValuePrecisionUser)
		INT(CTS,labelSeriesPrecisionUser)

#undef BAR			
#undef PIE			
#undef RADAR		
#undef CTS			
#undef LINE			
#undef SCATTER		
#undef AXIS			