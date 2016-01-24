#include "matwnd.h"

MatWnd::MatWnd(QWidget *parent)
	: QWidget( parent )
{
	setupUi(this);
	
	// create items showing up matrix window
	//
	mSceneUp = new QGraphicsScene();
	mViewUp = new QGraphicsView();
	mVLayout->addWidget( mViewUp );

	mImageItemUp = new VideoFrameItem();
	mImageItemUp->setPos( QPointF(0,0) );
	mSceneUp->addItem( mImageItemUp );   

	// create items showing low matrix window
	//
	mSceneLow = new QGraphicsScene();
	mViewLow = new QGraphicsView();
	mVLayout->addWidget( mViewLow );

	mImageItemLow = new VideoFrameItem();
	mImageItemLow->setPos( QPointF(0,0) );
	mSceneLow->addItem( mImageItemLow );  

	mViewUp->setScene(  mSceneUp );
	mViewLow->setScene(  mSceneLow );

	setWindowTitle( "plot window" );
}

MatWnd::~MatWnd()
{

}

void MatWnd::showMatUp( const Mat & frame )
{
	if(! frame.data ){
		return;
	}

	// resize the frame
	//Size newSize( width , int( width*(frame.rows)/(frame.cols)) );   // col, row
	//Mat newFrame;
	//cv::resize(frame, newFrame, newSize);

	mImageItemUp->updateImage(frame);
}

void MatWnd::showMatLow( const Mat & frame )
{
	if(! frame.data ){
		return;
	}

	// resize the frame
	//Size newSize( width , int( width*(frame.rows)/(frame.cols)) );   // col, row
	//cv::resize(frame, frame, newSize);

	mImageItemLow->updateImage(frame);
}
