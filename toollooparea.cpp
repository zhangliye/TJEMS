#include "pub.h"

#include "toollooparea.h"
#include "videoviewbase.h"
#include "mainwindow.h"
#include "loopdetector.h"

ToolLoopArea::ToolLoopArea(VideoViewBase *parent)
	: ToolImage(parent)
{
	mPrePoint.setX(-1);
	mPrePoint.setY(-1);
}

ToolLoopArea::~ToolLoopArea()
{

}

void ToolLoopArea::mousePress( QMouseEvent *mouseEvent )
{	
	VideoLoopTrackView *view = static_cast<VideoLoopTrackView*>( mView );

	if (Qt::RightButton == mouseEvent->button()){  //# right button click, finished a loop area
		LoopDetector *loop_detector = new LoopDetector( view );
		loop_detector->setShape( mBoundaryPts );

		// set detector background 
		//
		qreal xMin;
		qreal xMax;
		qreal yMin;
		qreal yMax;
		loop_detector->mPolygon->boundingRect().getCoords(&xMin, &yMin, &xMax, &yMax);

		if( xMin<0 )
			xMin = 0;
		if( yMin<0 )
			yMin = 0;
		if ( xMax>= view->mCurrentFrame.cols )
			xMax = view->mCurrentFrame.cols;
		if ( yMax>= view->mCurrentFrame.rows )
			yMax = view->mCurrentFrame.rows;

		Mat subM = view->mCurrentFrame( Range(yMin, yMax), Range(xMin, xMax) ).clone(); //row, col		
		loop_detector->mBackGroundMat = subM;
		view->mMainWind->mMatWnd->showMatUp( subM );   //show in up matrix window
		
		// add one loop detector to view 
		//
		loop_detector->updateDetectorMask( view->mCurrentFrame );   //update the polygon area of the deteor area
		view->mLoopDetectors.append( loop_detector );
		view->mCurLoopDetector = loop_detector;

		//# clear temporal points and lines
		clearTempPointLine();    
	} 
	else if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   //# add one point   
		//# save point to list
		//#   
		QPointF pt = view->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		mBoundaryPts.append( pt );

		//# draw temporal point
		//#
		QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem( pt.x(), pt.y(), 1, 1);
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );

		view->scene()->addItem( pointItem );  //# show line

		//# draw temporal line
		//#
		if ( mPrePoint.x()>=0 && mPrePoint.y()>=0  ){
			QGraphicsLineItem *line = new QGraphicsLineItem( mPrePoint.x(), mPrePoint.y(), pt.x(), pt.y()  );
			line->setPen( QPen(Qt::green) );
			mLineItems.append( line );

			view->scene()->addItem( line );  //# show line
		}

		// record new previous point
		mPrePoint.setX(pt.x());
		mPrePoint.setY(pt.y());
	}
}

void ToolLoopArea::keyPress( QKeyEvent *keyEvent )
{
	VideoLoopTrackView *view = static_cast<VideoLoopTrackView*>( mView );

	//''' remove last added detector area
	//	keyEvent - QKeyEvent
	//	'''
	//# ESC, delete the just added detector rectangle boundary point
	//#
	if (keyEvent->key() == Qt::Key_Escape) {
		if (mPointItems.length() > 0) {
			QGraphicsEllipseItem *pt = mPointItems.takeLast(); //# remove the last point
			view->scene()->removeItem( pt ); 
		}

		//# clear temporal lines
		//#
		if (mLineItems.length() > 0){
			QGraphicsLineItem *line = mLineItems.takeLast(); //# remove the last point
			view->scene()->removeItem( line ); 
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

void ToolLoopArea::clearTempPointLine()
{
	VideoLoopTrackView *view = static_cast<VideoLoopTrackView*>( mView );

	//# clear temporal points
	//#
	for (int i = 0; i < mPointItems.size(); ++i) {
		view->scene()->removeItem( mPointItems.at(i) );

		QGraphicsEllipseItem *pPt = mPointItems.at(i);
		if (pPt != NULL)
			delete pPt;
	}
	mPointItems.clear();

	//# clear temporal lines
	//#
	for (int i = 0; i < mLineItems.size(); ++i)
	{
		view->scene()->removeItem( mLineItems.at(i) );
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
