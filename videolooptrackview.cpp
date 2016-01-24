/************************************************************************/
/*
/* FileName    : videoprocessloopview.cpp
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
/* permission of Liye Zhang.
/*
/* It is mainly developed during 2011-2015 in Univeristy of Florida, USA and Tongji
/* University, and University of Singapore.
/*                                                              
/************************************************************************/
#include "videolooptrackview.h"
#include "mainwindow.h"

VideoLoopTrackView::VideoLoopTrackView(QWidget *parent)
	: VideoViewBase(parent)
{
	connect( mTimer, SIGNAL(timeout()), this, SLOT(actNextFrame()) );

	// bicycle loop detector area
	mCurLoopDetector = NULL;

	mViewType = 2;
}

VideoLoopTrackView::~VideoLoopTrackView()
{

}


void VideoLoopTrackView::actNextFrame( int customStep /*=-1*/, bool ifShowCorelateInf/*false*/ )
{
	//'''read new frame then show and make previous frame op==1'''
	int frmInde = 1;

	if (customStep == -1){  // 
		if ( mDetectionModel != true) // in playing model 
		{
			frmInde = mCurrentFrameIndex + mPlayStep;
		} else {  // in processing model
			frmInde = mCurrentFrameIndex + mProStep;
		}
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

	// detect bicycle
	// 
	if ( (mCurLoopDetector != NULL) && ( mDetectionModel == true) ) // only in detection model and detector instaled
	{
		int detect_result = mCurLoopDetector->detectBicycle( mCurrentFrame );
		double y = mCurLoopDetector->mCurDif_rate;
		mMainWind->mPlotWnd->pushData( mCurrentFrameIndex, y );
		mMainWind->mPlotWnd->plot();

		// show message in GUI
		//
		QString msg = QString("%1 / %2").arg(mCurrentFrameIndex).arg(mFrameCount);
		mMainWind->mLabFrameNum->setText( msg );

		if ( detect_result != -1 ) // found bike
		{
			if ( mMainWind->mCbxBikeStop->checkState() == Qt::Checked )
				actPauseProcessing();        
			mMainWind->log( QString("co-relate rate: %1").arg( mCurLoopDetector->mCurDif_rate ) );
		}

		if ( ifShowCorelateInf ){
			mMainWind->log( QString("co-relate rate: %1").arg( mCurLoopDetector->mCurDif_rate ) );
		}
	} else {
		mMainWind->mPlotWnd->pushData( mCurrentFrameIndex, 1 );
		mMainWind->mPlotWnd->plot();
	}
}


void VideoLoopTrackView::actStartProcessing()
{
	if ( mMainWind->mPreToolName=="")
	{
		QMessageBox::information(this, "please set video processing model", "please click traffic video processing button to install processing tools!");
		return;
	}

	startProcessing();	
}

void VideoLoopTrackView::actPauseProcessing( )
{
	pauseProcessing();
}

void VideoLoopTrackView::actEndProcessing( )
{
	pauseProcessing();

	//# save data to csv
	//#
	QString path("");
	QString file_name;
	QString initial_path;
	while(true){
		file_name = mMainWind->mCurrentVideoFile + "_" + mMainWind->mEdtStartFrame->text() + "_" + QString("%1").arg(mCurrentFrameIndex) + ".csv";
		initial_path = QString("d:/") + "headway_" + file_name;

		//# get user selected path and file
		//#
		QFileDialog fileDialog( this, "save to excel csv", initial_path, "csv|(*.csv)" );
		fileDialog.setFileMode( QFileDialog::AnyFile );

		if ( fileDialog.exec()){
			QDir dir = fileDialog.directory();
			QStringList fileNames = fileDialog.selectedFiles();
			path = fileNames[0];
		} else { //# do not select file
			//# if continue / break
			//#
			QString quit_msg = "wrong path. Are you sure you want to exist without saving?";
			QMessageBox::StandardButton reply = QMessageBox::question(this, "do not save", quit_msg, QMessageBox::Yes|QMessageBox::No);
			if (reply == QMessageBox::Yes){
				return;
			} else {
				continue;
			}
		}

		//# save to csv
		//#
		try{
			QFile outputFile( path );
			outputFile.open(QIODevice::WriteOnly);

			/* Check it opened OK */
			if(!outputFile.isOpen()){
				QMessageBox::information(this, "system error", "cannot open file writor");
				return;
			}

			/* Point a QTextStream object at the file */
			QTextStream outStream(&outputFile);

			// write the first line
			outStream << "time, bicycle_type\n";

			QVector<QVector<int>> headways = ((ToolBicycleHeadway*)mMainWind->mToolMap["ToolBicycleHeadway"])->mBicycleHeadways;
			for (int i = 0; i < headways.size(); ++i) {
				QVector<int> row = headways.at(i);
				if (row.count()>=2){
					outStream<< row[0] << "," <<row[1] <<"\n";
				}
			}

			/* Close the file */
			outputFile.close();
		} catch(...){      
			//# if continue / break
			//#
			QString quit_msg = "saved failly, Are you sure you want to exist without saving?";
			QMessageBox::StandardButton reply = QMessageBox::question(this, "do not save", quit_msg, QMessageBox::Yes|QMessageBox::No);
			if (reply == QMessageBox::Yes){
				return;
			} else {
				continue;
			}
		}

		QMessageBox::information(this, "successfully saved", QString("headways saved to ") + path  );
		break;		 
	} /*# end while True:  */  
}
