#ifndef TOOLDELETELOOPDETECTOR_H
#define TOOLDELETELOOPDETECTOR_H

#include "toolimage.h"
#include "videoviewbase.h"

class ToolDeleteLoopDetector : public ToolImage
{
	Q_OBJECT

public:
	ToolDeleteLoopDetector(VideoViewBase *parent);
	~ToolDeleteLoopDetector();

public slots:
	void mousePress( QMouseEvent *mouseEvent);
	
};

#endif // TOOLDELETELOOPDETECTOR_H
