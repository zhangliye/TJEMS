#include "pub.h"

#include "toollooparea.h"
#include "videoviewbase.h"
#include "mainwindow.h"
#include "loopdetector.h"

ToolStudyArea::ToolStudyArea(VideoViewBase *parent)
	: ToolImage(parent)
{
	mPrePoint.setX(-1);
	mPrePoint.setY(-1);
}

ToolStudyArea::~ToolStudyArea()
{

}

void ToolStudyArea::mousePress( QMouseEvent *mouseEvent )
{	
	if (Qt::RightButton == mouseEvent->button()){  //# right button click, finished a loop area
		// draw last line segment
		QPointF pt = mBoundaryPts.first();

		// draw temporal point
		//
		QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem( pt.x(), pt.y(), 1, 1);
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );

		mView->scene()->addItem( pointItem );  //# show line

		// draw temporal line
		//
		if ( mPrePoint.x()>=0 && mPrePoint.y()>=0  ){
			QGraphicsLineItem *line = new QGraphicsLineItem( mPrePoint.x(), mPrePoint.y(), pt.x(), pt.y()  );
			line->setPen( QPen(Qt::green) );
			mLineItems.append( line );			

			mView->scene()->addItem( line );  //# show line
		}

		// record new previouse point
		//
		mPrePoint.setX( -1 );
		mPrePoint.setY( -1 );

		// set the study area of main window and project file object
		//
		QVector<QPoint> points;
		for(int i=0; i<mBoundaryPts.size(); ++i){
			points.append( QPoint(mBoundaryPts[i].x(), mBoundaryPts[i].y()) );
		}
		mView->mMainWind->mInterestingArea = QPolygon( points );
		mView->mMainWind->mPrjFileObj.setStudyArea( points );   // save study area to project file object
		mView->mMainWind->updateMask( mView->mCurrentFrame );   //update mask matrix according to polygon of study area
		
		// release, set to pan tool
		mView->mMainWind->releaseTool();
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

		mView->scene()->addItem( pointItem );  //# show line

		// draw temporal line
		//
		if ( mPrePoint.x()>=0 && mPrePoint.y()>=0  ){
			QGraphicsLineItem *line = new QGraphicsLineItem( mPrePoint.x(), mPrePoint.y(), pt.x(), pt.y()  );
			line->setPen( QPen(Qt::green) );
			mLineItems.append( line );			

			mView->scene()->addItem( line );  //# show line
		}

		// record new previouse point
		mPrePoint.setX(pt.x());
		mPrePoint.setY(pt.y());
	}	
}

void ToolStudyArea::keyPress( QKeyEvent *keyEvent )
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

		//# clear temporal lines
		//#
		if (mLineItems.length() > 0){
			QGraphicsLineItem *line = mLineItems.takeLast(); //# remove the last point
			mView->scene()->removeItem( line ); 
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

void ToolStudyArea::clearTempPointLine()
{
	//# clear temporal points
	//#
	for (int i = 0; i < mPointItems.size(); ++i) {
		mView->scene()->removeItem( mPointItems.at(i) );

		QGraphicsEllipseItem *pPt = mPointItems.at(i);
		if (pPt != NULL)
			delete pPt;
	}
	mPointItems.clear();

	//# clear temporal lines
	//#
	for (int i = 0; i < mLineItems.size(); ++i)
	{
		mView->scene()->removeItem( mLineItems.at(i) );
		QGraphicsLineItem *pLine = mLineItems.at(i);
		if (pLine != NULL)
			delete pLine;
	}
	mLineItems.clear();

	//# clear points and prepoint for starting new a detector
	//#
	mPrePoint.setX(-1);
	mPrePoint.setY(-1);

	mBoundaryPts.clear();
}

bool ToolStudyArea::valid()
{
	return mBoundaryPts.count() > 0;
}
