#include "toolpan.h"

ToolPan::ToolPan(VideoViewBase *parent)
	: ToolImage(parent)
{

}

ToolPan::~ToolPan()
{

}

void ToolPan::mouseMove( QMouseEvent *mouseEvent )
{

}

void ToolPan::mousePress( QMouseEvent *mouseEvent )
{

}

void ToolPan::keyPress( QKeyEvent *keyEvent )
{
	VideoSemiTrackView *view = static_cast<VideoSemiTrackView*>( mView );
	view->keyPressMoveon( keyEvent );
}
