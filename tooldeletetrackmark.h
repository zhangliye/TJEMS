#ifndef TOOLDELETEMARK_H
#define TOOLDELETEMARK_H

#include "toolimage.h"
#include "videoviewbase.h"
#include "videoviewbase.h"
#include "videosemitrackview.h"
#include "mainwindow.h"
#include <iostream>
#include "trajectorypoint.h"

using namespace std;

class ToolDeleteTrackMark : public ToolImage
{
	Q_OBJECT

public:
	ToolDeleteTrackMark(VideoViewBase *parent);
	~ToolDeleteTrackMark();

	// remove selected point
	void removeCurrentPoint();

public:
	VideoViewBase* mBaseView;
	QPair<TrajectoryPoint*, QGraphicsTextItem*> mNearPt;  // selected point by mouse

public slots:
	void mouseMove( QMouseEvent *mouseEvent);
	void mousePress( QMouseEvent *mouseEvent);
	void keyPress( QKeyEvent *keyEvent);			
};


#endif // TOOLSEMITRACK