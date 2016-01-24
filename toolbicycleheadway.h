#ifndef TOOLBICYCLEHEADWAY_H
#define TOOLBICYCLEHEADWAY_H

#include "toolimage.h"

class VideoViewBase;

class ToolBicycleHeadway : public ToolImage
{
	Q_OBJECT

public:
	QVector<QVector<int>> mBicycleHeadways;     //#[frame, 1]   # 1- Q, ebick; 2- A, bicycle
	int mCount;

public:
	ToolBicycleHeadway(VideoViewBase *parent);
	~ToolBicycleHeadway();

public slots:
	void keyPress( QKeyEvent *keyEvent);
};

#endif // TOOLBICYCLEHEADWAY_H
