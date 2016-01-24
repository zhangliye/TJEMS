#include "plotwnd.h"

PlotWnd::PlotWnd(QWidget *parent)
	: 	QDockWidget( parent )
{
	setupUi( this );
		
	mWidth = 1000;
	setWindowTitle( "plot window" );

	// create graph and assign data to it:
	mCustomPlot->addGraph();
		
	// set axes ranges, so we see all data:
	mCustomPlot->yAxis->setRange(-1, 1);
}

void PlotWnd::pushData( double x, double y) {
	mXAll.append( x );
	mYAll.append( y );
	mXPlot.append( x );
	mYPlot.append( y );

	// only show first nWidth elements
	//
	if (mXPlot.count()> mWidth)
		mXPlot.pop_front();
	if (mYPlot.count()> mWidth)
		mYPlot.pop_front();
}

PlotWnd::~PlotWnd()
{

}

void PlotWnd::plot()
{
	mCustomPlot->graph(0)->setData( mXPlot, mYPlot );
	
	mCustomPlot->xAxis->setRange( mXPlot.first(), mXPlot.last() );

	mCustomPlot->replot();
}

void PlotWnd::setYRange(int start, int end)
{
	mCustomPlot->yAxis->setRange(start, end);	
	plot();
}

void PlotWnd::setWidth( int width )
{
	if (width<mWidth)
	{
		if ( mXPlot.count()> width  )
		{
			mXPlot.resize( width );
		}
		if ( mYPlot.count()> width  )
		{
			mYPlot.resize( width );
		}
	}

	mWidth = width;
	plot();
}
