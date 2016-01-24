#include "toolimage.h"
#include <iostream>
using namespace std;

ToolImage::ToolImage(VideoViewBase *parent)
	: QObject( (QObject*)parent )
{
	mView = parent;
}

ToolImage::~ToolImage()
{
	uninstall( mSender );
}

void ToolImage::install( QObject *sender )
{	
	//# connect signals
	//#
	connect( sender, SIGNAL(MousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)) );
	connect( sender, SIGNAL(MouseMove(QMouseEvent *)), this, SLOT(mouseMove(QMouseEvent *)) );
	connect( sender, SIGNAL(MouseRelease(QMouseEvent *)), this, SLOT(mouseRelease(QMouseEvent *)) );
	connect( sender, SIGNAL(MouseDoubleClick(QMouseEvent *)), this, SLOT(mouseDoubleClick(QMouseEvent *)) );
	connect( sender, SIGNAL(KeyPress(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)) );

	mSender = sender;
}

void ToolImage::uninstall( QObject *sender )
{
	disconnect( sender, SIGNAL(MousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)) );
	disconnect( sender, SIGNAL(MouseMove(QMouseEvent *)), this, SLOT(mouseMove(QMouseEvent *)) );
	disconnect( sender, SIGNAL(MouseRelease(QMouseEvent *)), this, SLOT(mouseRelease(QMouseEvent *)) );
	disconnect( sender, SIGNAL(MouseDoubleClick(QMouseEvent *)), this, SLOT(mouseDoubleClick(QMouseEvent *)) );
	disconnect( sender, SIGNAL(KeyPress(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)) );
}

void ToolImage::mousePress( QMouseEvent *mouseEvent )
{
	//mouseEvent->accept();
}

void ToolImage::mouseMove( QMouseEvent *mouseEvent )
{
	//cout<<"ToolImage::mouseMove"<<endl;
	//mouseEvent->accept();
}

void ToolImage::mouseRelease( QMouseEvent *mouseEvent )
{
	//mouseEvent->accept();
}

void ToolImage::mouseDoubleClick( QMouseEvent *mouseEvent )
{
	//mouseEvent->accept();
}

void ToolImage::keyPress( QKeyEvent *keyEvent )
{
	//keyEvent->accept();
}

