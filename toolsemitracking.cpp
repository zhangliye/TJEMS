#include "toolsemitracking.h"
#include "videoviewbase.h"
#include "mainwindow.h"
#include "trajectorypoint.h"
#include <iostream>
using namespace std;

ToolSemiTracking::ToolSemiTracking(VideoViewBase *parent)
	: ToolImage(parent)
{
	mEnlargeRate = 2;
	mSubWndWidth = 30;
	mCurrentVehicleID = 0;
}

ToolSemiTracking::~ToolSemiTracking()
{
	;
}

void ToolSemiTracking::mouseMove( QMouseEvent *mouseEvent)
{
	// Extract sum image
	//
	QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
	
	// show in current image
	//
	int x = int( pt.x() );
	int y = int( pt.y() );

	// show on status bar
	mView->mMainWind->mStatusPositionWnd->setText( QString("(%1,%2)").arg(x).arg(y) );

	// make sure point located on image
	//
	if (x<0 || y<0 || x>mView->mCurrentFrame.cols || y>mView->mCurrentFrame.rows)
		return;

	int xMin;
	int xCenter = 0;
	int yCenter = 0;
	if (x - mSubWndWidth>=0){
		xMin = x-mSubWndWidth;
		xCenter = mSubWndWidth;
	} else {
		xMin = 0;
		xCenter = x;
	}
	
	int yMin;
	if (y - mSubWndWidth>=0 ){
		yMin = y-mSubWndWidth;
		yCenter = mSubWndWidth;
	} else {
		yMin = 0;
		yCenter = y;
	}

	// right-bottom bound
	//
	int xMax = (x + mSubWndWidth)>=mView->mCurrentFrame.cols ? mView->mCurrentFrame.cols : (x + mSubWndWidth);
	int yMax = (y + mSubWndWidth)>=mView->mCurrentFrame.rows ? mView->mCurrentFrame.rows : (y + mSubWndWidth);

	// copy sub image to template image, center to center
	//
	mSubImage = mView->mCurrentFrame.colRange(xMin,xMax).rowRange(yMin,yMax);
	mMixedImage = Mat( Size(mSubWndWidth*2, mSubWndWidth*2), mView->mCurrentFrame.type(), Scalar(255, 255, 255) );

	int destXStart = mSubWndWidth - xCenter;
	int destXEnd = destXStart + xMax - xMin;
	int destYStart = mSubWndWidth - yCenter;
	int destYEnd = destYStart + yMax - yMin;
	mSubImage.copyTo( mMixedImage(Rect(destXStart, destYStart, xMax - xMin,yMax - yMin ) ) );
	
	// draw crossing mark and show in window
	// 
	resize(mMixedImage, mMixedImage, mMixedImage.size()*mEnlargeRate);
	line(mMixedImage, Point(0, mMixedImage.rows/2), Point(mMixedImage.cols, mMixedImage.rows/2), Scalar(0, 0, 255) );
	line(mMixedImage, Point(mMixedImage.cols/2, 0), Point(mMixedImage.cols/2, mMixedImage.rows), Scalar(0, 0, 255) );
	mView->mMainWind->mMatWnd->showMatLow( mMixedImage );
}

void ToolSemiTracking::mousePress( QMouseEvent *mouseEvent)
{
	if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   
		// check if the current vechleID is ok, same vehicle cannot appear two time at same time instance
		int frmIndex = mView->mCurrentFrameIndex;
		QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text_old = mView->mMainWind->findTrajPoint( mView, frmIndex, mCurrentVehicleID);
		if( pt_text_old.first != NULL )  // two vehicle appear at same time
		{
			QMessageBox::information(mView, "same vehicle existed", "please change vehicle ID!");
			return;
		}			
		
		// add one point
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		float x = pt.x();
		float y = pt.y();

		// if point not on image, return
		if (x<0 || x>mView->mCurrentFrame.cols || y<0 || y>mView->mCurrentFrame.rows)
			return;
		 
		// add sum image to Image Window
		//		
		int xMin = (x - mSubWndWidth)>=0 ? x-mSubWndWidth : 0;
		int yMin = (y - mSubWndWidth)>=0 ? y-mSubWndWidth : 0;

		int xMax = (x + mSubWndWidth)>=mView->mCurrentFrame.cols ? mView->mCurrentFrame.cols : (x+mSubWndWidth);
		int yMax = (y + mSubWndWidth)>=mView->mCurrentFrame.rows ? mView->mCurrentFrame.rows : (y+mSubWndWidth);
		if ( xMax<=0 )
		{
			xMax=0;
		}

		if ( yMax<=0 )
		{
			yMax=0;
		}

		if ( xMax-xMin == 0)
			return;
		if ( yMax-yMin == 0)
			return;

		Mat mSubImage = mView->mCurrentFrame.colRange(xMin,xMax).rowRange(yMin,yMax);
		Mat subImg = Mat( mSubImage.size(), mSubImage.type(), Scalar(0, 0, 0) );
		mSubImage.copyTo( subImg );

		// draw center and show in small window
		// 
		resize( subImg, subImg, subImg.size()*mEnlargeRate );
		circle( subImg, Point( subImg.cols/2, subImg.rows/2), 1, Scalar(230, 0, 0) );
		circle( subImg, Point( subImg.cols/2, subImg.rows/2), 5, Scalar(230, 0, 0) );
		mView->mMainWind->mMatWnd->showMatUp( subImg );

		// create point item and show
		//
		TrajectoryPoint *pointItem = new TrajectoryPoint( mCurrentVehicleID, mView->mMainWind->getBikeType() );
		pointItem->setPosition( pt.x(), pt.y() );  // 1, 1  
		//pointItem->setPos( pt );    this will cause point lost in GUI       ??????????????????????????????????????????????????????????????
		pointItem->setPen( QPen(Qt::red) );
		mPointItems.append( pointItem );
		mView->scene()->addItem( pointItem );      // show line	

		// create label for ID of vehicle and show
		//
		QGraphicsTextItem *textItem = new QGraphicsTextItem( QString("%1").arg(mCurrentVehicleID) );
		//textItem->setPen( QPen(Qt::red) );
		textItem->setPos( pt );
		QFont font;
		font.setPointSize( 6 );
		textItem->setFont( font );
		textItem->setOpacity( 0.6 );
		mView->scene()->addItem( textItem );

		// save trajectory point into main window
		// 
		
		QPair<TrajectoryPoint*, QGraphicsTextItem*> pair;
		pair.first = pointItem;
		pair.second = textItem;
		if ( !mView->mMainWind->mPrjFileObj.mTrajectory.contains(frmIndex) ) // not found points at time instance
		{
			QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> timePoints;

			timePoints[ mCurrentVehicleID ] = pair;
			mView->mMainWind->mPrjFileObj.mTrajectory[frmIndex] = timePoints;
		} else {   //found point at given time instance
			mView->mMainWind->mPrjFileObj.mTrajectory[frmIndex][ mCurrentVehicleID ] = pair ;
		}
	} 
}

void ToolSemiTracking::keyPress( QKeyEvent *keyEvent)
{
	VideoSemiTrackView *view = static_cast<VideoSemiTrackView*>( mView );
	view->keyPressMoveon( keyEvent );
}

void ToolSemiTracking::actNextID( )
{
	mCurrentVehicleID += 1;

	mView->mMainWind->mSbxCurrentID->setValue( mCurrentVehicleID );
}