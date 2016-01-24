#include "toolmovetrackmark.h"

#include "videoviewbase.h"
#include "mainwindow.h"
#include <iostream>
using namespace std;

ToolMoveTrackMark::ToolMoveTrackMark(VideoViewBase *parent)
	: ToolImage(parent)
{
	;
}

ToolMoveTrackMark::~ToolMoveTrackMark()
{
	;
}

void ToolMoveTrackMark::mouseMove( QMouseEvent *mouseEvent)
{
	mouseEvent->ignore();

	// Extract sum image
	//
	QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );


	// show in current image
	//
}

void ToolMoveTrackMark::mousePress( QMouseEvent *mouseEvent)
{
	if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   // add one point   
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		mBoundaryPts.append( pt );
		int sub_wnd_width = 10;
		float x = pt.x();
		float y = pt.y();
		int x1 = (x - sub_wnd_width)>=0 ? x-sub_wnd_width : -(x-sub_wnd_width);
		int y1 = (y - sub_wnd_width)>=0 ? y-sub_wnd_width : -(y-sub_wnd_width);

		// draw temporal point
		// 
		/*
		TrajectoryPoint *pointItem = new TrajectoryPoint( pt.x(), pt.y(), 1, 1 );
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );

		mView->scene()->addItem( pointItem );  // show line		*/
	}	
}

void ToolMoveTrackMark::keyPress( QKeyEvent *keyEvent)
{
	cout<<"I am here";
}

