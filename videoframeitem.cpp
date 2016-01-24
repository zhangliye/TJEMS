#include "videoframeitem.h"

VideoFrameItem::VideoFrameItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem( parent )
{
	setFlag( QGraphicsItem::ItemIsSelectable );
	setAcceptsHoverEvents( true );
}

VideoFrameItem::~VideoFrameItem()
{

}

void VideoFrameItem::updateImage( const Mat &numImage )
{
	if(! numImage.data ){
		return;
	}

	mCurPixmap.detach();
	
	mCurPixmap = QPixmap::fromImage( QImage((const unsigned char*)numImage.data, numImage.cols, numImage.rows, numImage.step, QImage::Format_RGB888) );
	setPixmap( mCurPixmap );
}
