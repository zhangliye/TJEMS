/**********************************************************************************
//  File: videoprocesstrackview.cpp
//
//  Description: view of half-automatic processing
//  Author: Liye Zhang
//  Date: 2015, Nov. 3, National University of Singapore
//  Modification: 
//  
***********************************************************************************/

#include "videosemitrackview.h"
#include "mainwindow.h"
#include "pub.h"

VideoSemiTrackView::VideoSemiTrackView(QWidget *parent)
	: VideoViewBase(parent)
{
	connect( mTimer, SIGNAL(timeout()), this, SLOT(actNextFrame()) );

	mVideoImageItemBackGround = new VideoFrameItem();
	mVideoImageItemBackGround->setPos( QPointF(0,0) );	

	mBkSubtracktor = new BackgroundSubtractorMOG2();

	mViewType = 3;
}

VideoSemiTrackView::~VideoSemiTrackView()
{
	
}

void VideoSemiTrackView::actNextFrame( int customStep /*=-1*/, bool ifShowCorelateInf/*false*/ )
{
	
	// read new frame then show and make previous frame op==1 
	// 
	int frmInde = 1; 
	if (customStep == -1){  // 
		frmInde = mCurrentFrameIndex + mProStep; 
	} else { 
		frmInde = mCurrentFrameIndex + customStep; 
	} 

	// show current image in center window
	//
	bool ifOk = readFrame( frmInde );     
	if (ifOk){ // get new frame		
		showFrame( mCurrentFrame );
	} else {
		QMessageBox::information( this, "the end of the video", "please finish the processing, it is the end");
		actPauseProcessing();
		return;
	}

	// hide other points, show point at this time
	mMainWind->showPointsAtTime( frmInde );
}

void VideoSemiTrackView::actStartProcessing()
{
	startProcessing();	
}

void VideoSemiTrackView::actPauseProcessing( )
{
	pauseProcessing();
}

void VideoSemiTrackView::actEndProcessing( )
{
	pauseProcessing();

	// save data to csv
	//
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

void VideoSemiTrackView::keyPressMoveon( QKeyEvent *keyEvent )
{
	//''' keyEvent - QKeyEvent
	//	'''
	int key = keyEvent->key();

	if ( key == Qt::Key_Space ){
		actNextFrame( mManualStepSize, true );
		return;
	}

	if ( key == Qt::Key_Right ){
		actNextFrame( mManualStepSize, true );
		return;
	}

	if ( key == Qt::Key_Left ){
		actNextFrame( -( mManualStepSize ), true );
		return;
	}	
}

