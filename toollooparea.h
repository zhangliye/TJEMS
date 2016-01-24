#ifndef TOOLLOOPAREA_H
#define TOOLLOOPAREA_H

#include "toolimage.h"

class ToolLoopArea : public ToolImage
{
	Q_OBJECT

public:
	ToolLoopArea(VideoViewBase *parent);
	~ToolLoopArea();
	void clearTempPointLine();

public:
	QVector<QPointF> mBoundaryPts;       //#[(x1, y1), ...]  points of detector boundary

	//# temporal drawing
	//#
	QPoint mPrePoint;

	//# bicycle loop detector area  	
	QList<QGraphicsEllipseItem*> mPointItems;
	QList<QGraphicsLineItem*> mLineItems;

public slots:
	void mousePress( QMouseEvent *mouseEvent);
	void keyPress( QKeyEvent *keyEvent);	
};

#endif // TOOLLOOPAREA_H
