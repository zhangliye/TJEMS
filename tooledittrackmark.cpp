#include "tooledittrackmark.h"
#include "videoviewbase.h"
#include "mainwindow.h"
#include <iostream>
using namespace std;

ToolEditTrackMark::ToolEditTrackMark(VideoViewBase *parent)
	: ToolImage(parent)
{
	mBaseView = parent;
	mNearPt.first = NULL;
	mNearPt.second = NULL;

	mTrajEdtDlg = new TrajEdtDlg( parent, this );
	mTrajEdtDlg->hide();
}

ToolEditTrackMark::~ToolEditTrackMark()
{
	;
}

void ToolEditTrackMark::mouseMove( QMouseEvent *mouseEvent)
{
	// Extract sum image
	//
	QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
	
	// show in current image
	//
}

void ToolEditTrackMark::mousePress( QMouseEvent *mouseEvent)
{
	if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   // add one point   
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		int cur_time = mView->mCurrentFrameIndex;

		// clear previous selected point 
		//
		if ( mNearPt.first!=NULL)
			mNearPt.first->setSelected();

		// set new point selected
		//
		mNearPt = mBaseView->mMainWind->mPrjFileObj.nearestPoint(pt.x(), pt.y(), cur_time);
		if ( mNearPt.first!=NULL)
		{
			mNearPt.first->setSelected();

			// show editor dialog
			mTrajEdtDlg->move(mouseEvent->x(), mouseEvent->y());

			// set trajectory point current ID and type
			mTrajEdtDlg->mSpinID->setValue( mNearPt.first->bikeID() );
			mTrajEdtDlg->setBikeType( mNearPt.first->bikeType() );
			mTrajEdtDlg->show();
		}
	}	
}

void ToolEditTrackMark::keyPress( QKeyEvent *keyEvent)
{
	VideoSemiTrackView *view = dynamic_cast<VideoSemiTrackView*>( mView );
	view->keyPressMoveon( keyEvent );
}

