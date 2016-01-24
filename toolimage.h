#ifndef TOOLIMAGE_H
#define TOOLIMAGE_H

#include "pub.h"
#include <QObject>

class VideoViewBase;
class ToolImage : public QObject
{
	Q_OBJECT

public:
	//# parent, send signal
	//#
	VideoViewBase *mView;
	QObject *mSender;               // signal receiver

public:
	ToolImage(VideoViewBase *parent);
	~ToolImage();
	void install( QObject *sender);
	void uninstall( QObject *sender );

public slots:
	void mousePress( QMouseEvent *mouseEvent);
	void mouseMove( QMouseEvent *mouseEvent);
	void mouseRelease( QMouseEvent *mouseEvent);
	void mouseDoubleClick( QMouseEvent *mouseEvent);
	void keyPress( QKeyEvent *keyEvent);
};

#endif // TOOLIMAGE_H
