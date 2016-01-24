#include "videoexploreview.h"
#include "mainwindow.h"

VideoExploreView::VideoExploreView(QWidget *parent)
	: VideoViewBase(parent)
{
	connect( mTimer, SIGNAL(timeout()), this, SLOT(actNextFrame()) );

	mViewType = 1;
}

VideoExploreView::~VideoExploreView()
{

}

void VideoExploreView::updateMainWndGUI()
{

}

void VideoExploreView::actNextFrame( int customStep /*=-1*/, bool ifShowCorelateInf/*false*/ )
{
	//'''read new frame then show and make previous frame op==1'''
	int frmInde = 1;

	if (customStep == -1){  //
		frmInde = mCurrentFrameIndex + mPlayStep;
	} else {
		frmInde = mCurrentFrameIndex + customStep;
	}

	bool ifOk = readFrame( frmInde );

	if (ifOk){ // get new frame
		showFrame( mCurrentFrame );
	} else {
		QMessageBox::information( this, "the end of the video", "please finish the processing, it is the end");
		actPauseProcessing();
		return;
	}
}


void VideoExploreView::actStartProcessing()
{
	startProcessing();	
}

void VideoExploreView::actPauseProcessing( )
{
	pauseProcessing();
}

void VideoExploreView::actEndProcessing( )
{
	pauseProcessing();
}
