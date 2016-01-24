#include "toolbicycleheadway.h"
#include "videoviewbase.h"
#include "mainwindow.h"

ToolBicycleHeadway::ToolBicycleHeadway(VideoViewBase *parent)
	: ToolImage(parent)
{
	mCount = 0;	
}

ToolBicycleHeadway::~ToolBicycleHeadway()
{

}

void ToolBicycleHeadway::keyPress( QKeyEvent *keyEvent)
{
	VideoLoopTrackView *view = static_cast<VideoLoopTrackView*>( mView );

	//''' keyEvent - QKeyEvent
	//	'''
	int key = keyEvent->key();
	
	//# space -- stop in to step 
	//#
	if ( key == Qt::Key_Space ){
		mCount += 1;

		// make sure in detection model
		if (!view->mDetectionModel){
			view->mMainWind->setDetectorState( true );
		}
			
		view->actPauseProcessing();   //# stop the auto play
		view->actNextFrame( view->mManualStepSize, true );
	}

	else if ( key == Qt::Key_U ){   // update background
		view->mCurLoopDetector->updateBackGround( view->mCurrentFrame );
		view->actStartProcessing();   //# start the auto play   
	}

	else if ( key == Qt::Key_V ){   // set auto play model, not detect

		view->actStartProcessing();        //# start the auto play
		view->mMainWind->setDetectorState( false );   // show detector state on top bar
		//view->setFocus();
	}

	//# b -- continue auto play
	//#
	else if (key == Qt::Key_B ) {       //# continual auto detection
		view->mMainWind->setDetectorState( true );  // show detector state on top bar		
		view->actStartProcessing();      // start the auto play
		//view->setFocus();
	}
	
	//# q -- add traditional bicycle
	//#
	else if (key == Qt::Key_Q) {
		QVector<int> row;
		row.append( view->mCurrentFrameIndex );
		row.append( 1 );
		mBicycleHeadways.append( row );
		view->mMainWind->log( QString("add t-bike, %1").arg( mBicycleHeadways.count()) ); 
	}
	
	//# a -- a, e-bicycle, type value == 2
	//#
	else if ( key == Qt::Key_A ){
		QVector<int> row;
		row.append( view->mCurrentFrameIndex );
		row.append( 2 );
		mBicycleHeadways.append( row );
		view->mMainWind->log( QString("add e-bike, %1").arg( mBicycleHeadways.count()) );
	}     

	else if (key == Qt::Key_PageUp ) {    // increase all decrease detector threshold, default = 0.8
		if (view->mCurLoopDetector!=NULL)
			view->mCurLoopDetector->increaseThreshold();
	}
	else if (key == Qt::Key_PageDown ) {
		if (view->mCurLoopDetector!=NULL)
			view->mCurLoopDetector->decreaseThreshold();
	}

	//# up -- increase step size
	//#
	else if ( key == Qt::Key_Up ) {
		if (view->mTimer->isActive()) {   // auto playing
			view->mMainWind->mSboxProcessingDelay->stepUp();
		} else {  // single step model
			if ( (view->mManualStepSize + 1) >10 ) {
				view->mManualStepSize = 10;
			} else {
				view->mManualStepSize += 1;
			}

			view->mMainWind->mLabManualStep->setText( QString("manual step: %1").arg(view->mManualStepSize) );
		}

		// in playing model, increase step
		//
		if ( view->mDetectionModel != true)
		{
			view->increasePlayStep();
		}
	}

	//# down -- decrease step size
	//#
	else if (key == Qt::Key_Down ) {

		if (view->mTimer->isActive()) {   // auto playing
			view->mMainWind->mSboxProcessingDelay->stepDown();
		} else {  // single step model
			
			view->mManualStepSize -= 1;
			if (view->mManualStepSize <= 1)
				view->mManualStepSize = 1;

			view->mMainWind->mLabManualStep->setText( QString("manual step: %1").arg(view->mManualStepSize) );
		}

		// in playing model, decrease step
		//
		if ( view->mDetectionModel != true)
		{
			view->decreasePlayStep();
		}

	} // end if ( key == Qt::Key_Space ){
	
	//# stop the event continue
	//#
	keyEvent->accept();
}
