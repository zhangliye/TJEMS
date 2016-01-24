#include "tooleditcontrolpoint.h"
#include "pub.h"

#include "toollooparea.h"
#include "videoviewbase.h"
#include "mainwindow.h"
#include "loopdetector.h"
#include "coordinateedtwnd.h"

ToolEditControlPoint::ToolEditControlPoint(VideoViewBase *parent)
	: ToolImage(parent)
{
	mPrePoint.setX(-1);
	mPrePoint.setY(-1);
}

ToolEditControlPoint::~ToolEditControlPoint()
{

}

void ToolEditControlPoint::mousePress( QMouseEvent *mouseEvent )
{		
	if (Qt::RightButton == mouseEvent->button()){  // right button click, finished a loop area
		// draw last line segment
		QPointF pt = mBoundaryPts.first();

		// draw temporal point
		//
		QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem( pt.x(), pt.y(), 1, 1);
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );

		mView->scene()->addItem( pointItem );  // show line				
	} 
	else if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   // add one point   
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		mBoundaryPts.append( pt );

		// draw temporal point
		//
		QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem( pt.x(), pt.y(), 1, 1);
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );

		mView->scene()->addItem( pointItem );  // show line

		// record new previous point
		mPrePoint.setX(pt.x());
		mPrePoint.setY(pt.y());
	}	

	CoordinateEdtWnd coordWnd( mView->mMainWind );
	coordWnd.exec();
}

void ToolEditControlPoint::keyPress( QKeyEvent *keyEvent )
{
	//''' remove last added detector area
	//	keyEvent - QKeyEvent
	//	'''
	//# ESC, delete the just added detector rectangle boundary point
	//#
	if (keyEvent->key() == Qt::Key_Escape) {
		if (mPointItems.length() > 0) {
			QGraphicsEllipseItem *pt = mPointItems.takeLast(); //# remove the last point
			mView->scene()->removeItem( pt ); 
		}

		//# remove the previous boundary point
		//# 
		if ( mBoundaryPts.count() > 0){
			mBoundaryPts.pop_back();

			//# if boundary point list is not empty, change pre-point
			//#
			if (mBoundaryPts.count() > 0 ){
				mPrePoint = QPoint( mBoundaryPts.last().x(), mBoundaryPts.last().y() );  //# new pre-point for draw new line
			} else {
				mPrePoint.setX(-1);
				mPrePoint.setY(-1);
			}
		}
	} // end if (keyEvent->key() == Qt::Key_Escape) {
}
