#ifndef TOOLPAN_H
#define TOOLPAN_H

#include "toolimage.h"
#include "videoviewbase.h"
#include "videosemitrackview.h"

class ToolPan : public ToolImage
{
	Q_OBJECT

public:
	ToolPan(VideoViewBase *parent);
	~ToolPan();

public slots:
	void mouseMove( QMouseEvent *mouseEvent);
	void mousePress( QMouseEvent *mouseEvent);
	void keyPress( QKeyEvent *keyEvent);

private:
	
};

#endif // TOOLPAN_H
