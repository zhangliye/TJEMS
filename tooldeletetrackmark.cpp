#include "tooldeletetrackmark.h"

ToolDeleteTrackMark::ToolDeleteTrackMark(VideoViewBase *parent)
	: ToolImage(parent)
{
	mBaseView = parent;
	mNearPt.first = NULL;
	mNearPt.second = NULL;
}

ToolDeleteTrackMark::~ToolDeleteTrackMark()
{
	;
}

void ToolDeleteTrackMark::mouseMove( QMouseEvent *mouseEvent)
{
	// Extract sum image
	//
	QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );


	// show in current image
	//
}

void ToolDeleteTrackMark::mousePress( QMouseEvent *mouseEvent)
{
	if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   // add one point   
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		int cur_time = mView->mCurrentFrameIndex;
		mNearPt = mBaseView->mMainWind->mPrjFileObj.nearestPoint(pt.x(), pt.y(), cur_time);

		if ( mNearPt.first!=NULL)
		{
			mNearPt.first->setSelected();
		}
	}
}

void ToolDeleteTrackMark::keyPress( QKeyEvent *keyEvent)
{
	if ( keyEvent==QKeySequence::Delete )   // delete key pressed
	{
		removeCurrentPoint();
	} else {
		/*
		//program crash ??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
		VideoSemiTrackView *mBaseView = dynamic_cast<VideoSemiTrackView*>( mBaseView );
		mBaseView->keyPressMoveon( keyEvent );
		*/
	}
}

void ToolDeleteTrackMark::removeCurrentPoint()
{
	//     frame_index  bike_id       
	QMapIterator<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > i(mView->mMainWind->mPrjFileObj.mTrajectory);
	while ( i.hasNext() ) {
		i.next();  // move next

		// if not given time instance, skip
		if ( i.key() != mView->mCurrentFrameIndex )
			continue;

		// iterate points at given time instance
		//
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> ptsMap = i.value();
		QMapIterator<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> j( ptsMap );
		while ( j.hasNext() )
		{
			j.next();  // move next

			int veicleID = j.key();
			QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text = j.value();
			TrajectoryPoint* pt = (TrajectoryPoint*)pt_text.first;
			QGraphicsTextItem* text = (QGraphicsTextItem*)pt_text.second;

			// remove from container
			if ( pt==mNearPt.first && text==mNearPt.second ){
				mView->mMainWind->mPrjFileObj.mTrajectory[i.key()].remove( veicleID );
			}
		}		
	} // end while ( i.hasNext() ) {

	// remove point and text from scence
	//
	if (mNearPt.first!=NULL)
		mView->scene()->removeItem( mNearPt.first );

	if (mNearPt.second!=NULL)
		mView->scene()->removeItem( mNearPt.second );	
}

