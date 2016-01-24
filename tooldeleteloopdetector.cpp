#include "tooldeleteloopdetector.h"
#include "videolooptrackview.h"

ToolDeleteLoopDetector::ToolDeleteLoopDetector(VideoViewBase *parent)
	: ToolImage(parent)
{

}

ToolDeleteLoopDetector::~ToolDeleteLoopDetector()
{

}

void ToolDeleteLoopDetector::mousePress( QMouseEvent *mouseEvent )
{
	VideoLoopTrackView *view = static_cast<VideoLoopTrackView*>( mView );

	//# get point in scene coordinate system
	//#
	QPoint ptMouse = QPoint( mouseEvent->x(), mouseEvent->y() );
	QPointF pt = view->mapToScene( ptMouse );  //# convert from view to scene coordinate 

	//# iterate all detector to check if point is contained
	//#
	for (int i=0; i<view->mLoopDetectors.count(); ++i)
	{
		LoopDetector *detector = view->mLoopDetectors.at(i);
		if ( detector->mPolygon->contains( pt ) ){
			QMessageBox::StandardButton reply = QMessageBox::question(NULL, "delete detector", "want to delete this detector?", QMessageBox::Yes|QMessageBox::No );
			if (reply == QMessageBox::Yes){
				detector->removeShape();  //# delete shape from scene
				view->mLoopDetectors.removeOne( detector );  //# delete object
			}			
		}
	}

	
	

}
