/************************************************************************/
/*
/* FileName    : D:\liye_360syn\syn_develop\tjems2\tjems2\videoview.cpp
/* Description : 
/* Author      : Liye Zhang, during study and work in China, USA and Singapore
/* Email       : chinazhangly@126.com 
/* Date        : 2015/01/17
/* Copyright   : all right reserved
/*
/*------------------------------------------------------------------------------
/* Copyright Details:
/*
/* This program is developed as part of the results of Liye Zhang's PhD dissertation
/* from Zhong-Ren Peng's research group, Tongji University, China.
/* All the source code and related documents can only be copied or modified with the
/* permission of Liye Zhang and Prof. Zhong-Ren Peng's group.
/*
/* It is mainly developed during 2011-2015 in Univeristy of Florida, USA and Tongji
/* University, and University of Singapore.
/*                                                              
/************************************************************************/
#include "videoviewbase.h"
#include "mainwindow.h"
#include "loopdetector.h"
#include <iostream>
using namespace std;

VideoViewBase::VideoViewBase(QWidget *parent)
	: QGraphicsView(parent)
{
	mMainWind = (MainWindow*) parent;	

	//make the view only track mouse moving when button is pressed, draging
	setMouseTracking( true );
		
	// create the scene for containing Graph Item 
	//
	mScene = new QGraphicsScene();
	setScene(  mScene );
	
	// create image item showing video frame
	//
	mVideoImageItem = new VideoFrameItem();
	mVideoImageItem->setPos( QPointF(0,0) );
	//mVideoImageItem->mMovable = false;
	mIsShowingImage = false;       // showing one image at one time
	
	// variables for video reading
	//
	mProStep = 1;          //#frame fetch step
	mPlayStep = 10;
	mManualStepSize = 5;       //# step size of manual adjust
	mFrameCount = -1;          //#frame numbers in the current video
	mCurrentFrameIndex = -1;   //#current frame index 1-based
	mViewType = -1;
	mCapture = new VideoCapture();	  
	
	//# initialize GUI
	initGUI();
}

VideoViewBase::~VideoViewBase()
{

}

// get one frame frame video, increase current index
// frameNum - start from 1 
// return: (ifOk, image)  
bool VideoViewBase::readFrame( int frameNum, bool ifShowProgress /*= false*/ )
{
	Mat readImg;
	// make sure frame number not greater than frame account
	//       
	if (frameNum > mFrameCount)
	{
		QMessageBox::information( this, "wrong frame number", "greater than frame account");
		return false; 
	}

	if(frameNum <= 0)
		frameNum = 1;

	// small frame number, reopen and fetch
	//
	bool isOk = false;

	if (frameNum == mCurrentFrameIndex)  // do not need to read new frame
	{
		return( true );
	}
	else if (frameNum < mCurrentFrameIndex)  // reopen and forward read
	{
		// show progress dialog
		if (ifShowProgress)	{			
			mMainWind->mProgressDlg->setRange(0, frameNum);
			mMainWind->mProgressDlg->show();
			mMainWind->mProgressDlg->raise();
			mMainWind->mProgressDlg->activateWindow();
		}

		// reopen video file and iterate frames to the desired frame
		//
		reOpenVideo(); 			
		int frame_read = 0;
		for( int i=0; i<frameNum; ++i){
			mCapture->grab();

			// progress bar forward
			if (ifShowProgress)	{
				++frame_read;
				if ( frame_read % mMainWind->mProgressDlg->step() == 0 ){ // only move on iterated "mProStep" length				
					mMainWind->mProgressDlg->next(); 
					QApplication::instance()->processEvents();
				}
			}
		}
		//isOk = mCapture->retrieve( mCurrentFrame );   // save to current image Matrix
		isOk = mCapture->retrieve( readImg );    // save to current image Matrix

		// close progress dialog
		if (ifShowProgress)	{			
			mMainWind->mProgressDlg->hide();
			mMainWind->mProgressDlg->close();
		}

	} else {//   fetch forward, just move on frames 
		// show progress dialog
		if (ifShowProgress)	{			
			mMainWind->mProgressDlg->setRange(mCurrentFrameIndex, frameNum);
			mMainWind->mProgressDlg->show();
			mMainWind->mProgressDlg->raise();
			mMainWind->mProgressDlg->activateWindow();
		}

		// iterate frames to the desired frame
		//
		int frame_read = 0;
		for( int i=mCurrentFrameIndex; i<frameNum; ++i ){
			mCapture->grab();

			// progress bar foreward
			if (ifShowProgress)	{ 
				++frame_read;
				if ( frame_read % mMainWind->mProgressDlg->step() == 0 ){ // only move on iterated "mProStep" length				
						mMainWind->mProgressDlg->next();  
						QApplication::instance()->processEvents();
				}
			}
		}		

		//mCurrentFrame.release(); // release memory
		//isOk = mCapture->retrieve( mCurrentFrame );
		isOk = mCapture->retrieve( readImg );				

		// close progress dialog
		if (ifShowProgress)	{			
			mMainWind->mProgressDlg->hide();
			mMainWind->mProgressDlg->close();
		}
	}

	// set image to sub area
	//
	mCurrentFrame.release();  // release memory
	if ( mMainWind->mInterestingArea.count()>0 && mViewType!=1 )   // not ViewExploreView
	{
		int xMin;
		int xMax;
		int yMin;
		int yMax;
		mMainWind->mInterestingArea.boundingRect().getCoords(&xMin, &yMin, &xMax, &yMax);
		//Mat m = readImg.clone();
		//mCurrentFrame.release();
		Mat readSub = readImg.colRange(xMin,xMax).rowRange(yMin,yMax);
		Mat studyImg = Mat( readSub.size(), readImg.type(), Scalar(0, 0, 0) );
		readSub.copyTo( studyImg, this->mMainWind->mMask );
		mCurrentFrame = studyImg;
	} else {
		mCurrentFrame = readImg;
	}

	// update current frame index
	mCurrentFrameIndex = frameNum;

	//////////////////////////////////////////////////////////////////////////
	// update main window GUI
	//	
	// frame index label
	mMainWind->mLabFrameNum->setText(QString("%1/%2").arg( mCurrentFrameIndex).arg(mFrameCount) ); 

	// top slider
	if ( mCurrentFrameIndex < mMainWind->mFrameSlider->maximum() )
		mMainWind->mFrameSlider->setValue( mCurrentFrameIndex );	

	return(isOk);
}

void VideoViewBase::reOpenVideo()
{
	mCapture->release();
	mCapture->open( mVideoFile.toStdString() );
}

void VideoViewBase::showFrame( const Mat & frame )
{
	//show video frame in Scene, just update image
	//	frame- numpy, video frame
	//
	if ( !mIsShowingImage )
	{
		mIsShowingImage = true;

		// show image
		mVideoImageItem->updateImage( frame );

		if ( scene()->items().count( mVideoImageItem ) < 1) //   new 
			scene()->addItem( mVideoImageItem );   

		mIsShowingImage = false;		
	}
}

void VideoViewBase::openVideo()
{

}

void VideoViewBase::clearData()
{
	try{
		mCapture->release();
	} catch (...) {
		;
	}
	
	mFrameCount = -1;          //frame numbers in the current video
	mCurrentFrameIndex = -1;   //current frame index 0-based
	mScene->clear();
}

void VideoViewBase::mousePressEvent( QMouseEvent *e )
{
	emit MousePress(e); 
}

void VideoViewBase::mouseMoveEvent( QMouseEvent *e )
{
	emit MouseMove(e); 
}

void VideoViewBase::mouseReleaseEvent( QMouseEvent *e )
{
	emit MouseRelease(e); 
}

void VideoViewBase::mouseDoubleClickEvent( QMouseEvent *e )
{
	emit MouseDoubleClick(e);
}

void VideoViewBase::keyPressEvent( QKeyEvent * event )
{
	emit KeyPress( event );  
}

void VideoViewBase::startProcessing()
{
	mTimer->start();

	// change marker and GUI state
	mMainWind->setDetectorState( true );    // show detector state on top bar
}

void VideoViewBase::increasePlayStep()
{
	if(mPlayStep+1<1000) ++mPlayStep;
	mMainWind->mSboxPlayStepSize->setValue( mPlayStep );
}

void VideoViewBase::decreasePlayStep()
{
	if(mPlayStep-1>0) --mPlayStep;
	mMainWind->mSboxPlayStepSize->setValue( mPlayStep );
}


void VideoViewBase::openNewFile(  const QString &videoFilePath  )
{
	//'''open new video file '''
	clearData();
	
	mVideoFile = videoFilePath;
	mCapture->open( videoFilePath.toStdString() );   //
	// set slider range 
	//
	mFrameCount = mCapture->get( CV_CAP_PROP_FRAME_COUNT );
	mMainWind->mFrameSlider->setRange(1, mFrameCount);
	mMainWind->mFrameSlider->setValue( 1 );
	
	// show frame number on label GUI
	//
	QString msg = QString("%1 / %2").arg(1).arg(mFrameCount);
	mMainWind->mLabFrameNum->setText( msg );

	// show first image on QGraphicItem
	//
	mCurrentFrameIndex = 0;
	bool isOk = readFrame( mCurrentFrameIndex );       
	if(isOk){
		mFrameSize = mCurrentFrame.size();
		showFrame( mCurrentFrame );
	} else {
		QMessageBox::information( this, "no image", QString("no image at %1").arg(mCurrentFrameIndex) );
	}

	//force view to receive mouse move message when mouse is pressed
	viewport()->setMouseTracking( true );

	//////////////////////////////////////////////////////////////////////////
	// update main window GUI
	//
	// top slider
	mMainWind->mFrameSlider->setRange(1, mFrameCount);
	mMainWind->mFrameSlider->setValue(1);

	// label
	mMainWind->mLabFrameNum->setText(QString("1/%1").arg(mFrameCount) ); 
}

void VideoViewBase::initGUI()
{
	mMainWind->mSboxStepSize->setValue( mProStep  );   // initialize step of main window   

	/* initlize timer
	   */       
	mTimer = new QTimer();
	mTimer->setInterval( 200 );  // delay time is 1 second		
}
