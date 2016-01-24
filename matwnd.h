#ifndef MATWND_H
#define MATWND_H

#include "pub.h"
#include "videoframeitem.h"
#include "ui_matwnd.h"

class MatWnd : public QWidget, public Ui::MatWnd
{
	Q_OBJECT

public:
	MatWnd(QWidget *parent);
	~MatWnd();
	void showMatUp( const Mat & frame );
	void showMatLow( const Mat & frame );

private:	
	QGraphicsScene *mSceneLow;	
	QGraphicsView *mViewLow;
	VideoFrameItem *mImageItemUp;
	QPixmap mPixmapUp;	

	QGraphicsScene *mSceneUp;	
	QGraphicsView *mViewUp;	
	VideoFrameItem *mImageItemLow;
	QPixmap mPixmapLow;
};

#endif // MATWND_H
