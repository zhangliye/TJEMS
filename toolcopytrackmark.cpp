#include "toolcopytrackmark.h"
#include "videolooptrackview.h"

ToolCopyTrackMark::ToolCopyTrackMark(VideoViewBase *parent)
	: ToolImage(parent)
{
	mBaseView = parent;
	mSourcePt.first = NULL;
	mSourcePt.second = NULL;

	mDestPt.first = NULL;
	mDestPt.second = NULL;
}

ToolCopyTrackMark::~ToolCopyTrackMark()
{

}

void ToolCopyTrackMark::mousePress( QMouseEvent *mouseEvent )
{
	if (Qt::LeftButton == mouseEvent->button())    // left mouse button, add point and line 
	{   // add one point   
		// save point to list
		QPointF pt = mView->mapToScene( QPoint(mouseEvent->x(), mouseEvent->y()) );
		int cur_time = mView->mCurrentFrameIndex;

		if ( mSourcePt.first==NULL ){  // this finding source point
			mSourcePt = mBaseView->mMainWind->mPrjFileObj.nearestPoint(pt.x(), pt.y(), cur_time);
			mSourcePt.first->setSelected();

			// set ID and type to left tool button
			// 
			mView->mMainWind->setBikeType( mSourcePt.first->bikeType() );
			mView->mMainWind->mSbxCurrentID->setValue( mSourcePt.first->bikeID() );
		} else {  // finding the destination point 
			mDestPt = mBaseView->mMainWind->mPrjFileObj.nearestPoint(pt.x(), pt.y(), cur_time);

			// copy bike id and type, from source point to destination point
			//
			mDestPt.first->bikeID( mSourcePt.first->bikeID() );
			mDestPt.first->bikeType( mSourcePt.first->bikeType() );
			mDestPt.first->setSelected();

			// change label

			// set source point's pointers to NULL, clear old ones
			mSourcePt.first->clearSelected();
			mSourcePt.first = NULL;
			mSourcePt.second = NULL;			
		}	
	} else if ( Qt::RightButton == mouseEvent->button() )  // right button, clear copyed data to start new round
	{
		// set source point's pointers to NULL, clear old ones
		if(mSourcePt.first!= NULL)
			mSourcePt.first->clearSelected();

		mSourcePt.first = NULL;
		mSourcePt.second = NULL;		
	}
}
