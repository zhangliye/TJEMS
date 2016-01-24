#pragma once
#include "toolimage.h"
#include "videoviewbase.h"
#include "trajectorypoint.h"

#include "mainwindow.h"

class ToolCopyTrackMark : public ToolImage
{
	Q_OBJECT

public:
	ToolCopyTrackMark(VideoViewBase *parent);
	~ToolCopyTrackMark();

public slots:
	void mousePress( QMouseEvent *mouseEvent);

public:
	VideoViewBase* mBaseView;
	QPair<TrajectoryPoint*, QGraphicsTextItem*> mSourcePt;  // selected point by mouse, copy from 
	QPair<TrajectoryPoint*, QGraphicsTextItem*> mDestPt;    // selected point by mouse, copy to 
};