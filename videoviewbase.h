#ifndef MANUALPROCESSINGVIEW_H
#define MANUALPROCESSINGVIEW_H

#include <QObject>
// -*- coding: utf-8 -*-
//------------------------------------------------------------------------------
//
// FileName    : videoviewbase.h
// Description : manual processing view, to display image and processing with mouse
// Author      : Liye Zhang, Tongji University, China; University of Florida, USA
// Email       : chinazhangly@126.com 
// Date        : 2013-5-8 
// Copyright   : all right reserved
//
//------------------------------------------------------------------------------
// Copyright Details:
//
// This program is developed as part of the results of Liye Zhang's PhD dissertation
// from Zhong-Ren Peng's research group, Tongji University, China.
// All the source code and related documents can only be copied or modified with the
// permission of Liye Zhang and Prof. Zhong-Ren Peng's group.
// 
// It is mainly developed during 2011-2013 in Univeristy of Florida, USA and Tongji
// University, China.
//
//------------------------------------------------------------------------------
#include "pub.h"
#include "videoframeitem.h"
#include "loopdetector.h"

class MainWindow;
class VideoViewBase : public QGraphicsView
{
	Q_OBJECT
public:
	//window for user to process video
	VideoViewBase( QWidget *parent=NULL);
	~VideoViewBase();
	void initGUI();

	/************************************************************************/
	/* function    : fetch frame and save in "mCurrentFrame"
	/* parameter   : frameNum - frame index to fetch; ifShowProgress - if show
	/*               progress window
	/* return      : true- if successful
	/************************************************************************/
	bool readFrame( int frameNum, bool ifShowProgress = false);	

	void showFrame( const Mat & frame);
	float calcScale( const QRect &rect);
	void clearData();
	void reOpenVideo();

	/************************************************************************/
	/* function    : check if video file is opened
	/* return      : true, if opened
	/************************************************************************/
	bool videoOpened(){
		return (mFrameCount > 0 && mCurrentFrameIndex > 0 );
	}

	// stat or stop processing
	//
	void pauseProcessing(){
		mTimer->stop();
	}
	void startProcessing();	

	void increasePlayStep();
	void decreasePlayStep();

protected:
	void mousePressEvent( QMouseEvent *e);
	void mouseMoveEvent( QMouseEvent *e);
	void mouseReleaseEvent( QMouseEvent *e);
	void mouseDoubleClickEvent( QMouseEvent *e);
	void keyPressEvent(QKeyEvent * event );	

signals:
	void MousePress( QMouseEvent *e);
	void MouseMove( QMouseEvent *e);
	void MouseRelease( QMouseEvent *e);
	void MouseDoubleClick( QMouseEvent *e);
	void KeyPress(QKeyEvent *event );

public slots:
	void openNewFile( const QString &videoFilePath );
	void openVideo();

	// for video control
	//
	virtual void actStartProcessing() = 0;
	virtual void actPauseProcessing() = 0;
	virtual void actEndProcessing() = 0;

	/************************************************************************/
	/* function    : for time auto invoce action or single step video processing
	/* parameter   : customStep- custom frame sampling step; 
	/*               ifShowCorelateInf - if show rate value in main window log
	/************************************************************************/
	virtual void actNextFrame(int customStep=-1, bool ifShowCorelateInf=false ) = 0;

public:
	//mainwindow handle
	MainWindow *mMainWind;

	int mViewType;

	// create the scene for containing Graph Item 
	//
	QGraphicsScene *mScene;   
	Mat mCurrentFrame;            // current video frame (numpy)        
	Size mFrameSize;              // (width, height) / col, row
	float mRScale;                // the small side ration =  frame / window

	// create image item showing video frame
	//
	VideoFrameItem *mVideoImageItem; 

	bool mIsShowingImage;            // showing one image at one time

	// variables for video reading
	//
	QString mVideoFile;   // video file full path

	int mProStep;             // video processing frame fetch step
	int mPlayStep;            // video play sample step, default 10	
	int mManualStepSize;      // step size of manual adjust
	int mFrameCount;          // frame numbers in the current video
	int mCurrentFrameIndex;   // current frame index 1-based, defaut -1
	VideoCapture *mCapture;   // video reader
	QTimer *mTimer;           // for auto video playing				
};

#endif // MANUALPROCESSINGVIEW_H
