#ifndef VIDEOFRAMEITEM_H
#define VIDEOFRAMEITEM_H

#include "pub.h"

class VideoFrameItem : public QGraphicsPixmapItem	
{
public:
	QPixmap mCurPixmap;
public:
	VideoFrameItem(QGraphicsItem *parent=NULL);
	~VideoFrameItem();

	void updateImage( const Mat &numImage);

private:
	
};

#endif // VIDEOFRAMEITEM_H
