#ifndef COORDINATEEDTWND_H
#define COORDINATEEDTWND_H

#include "pub.h"
#include "videoframeitem.h"
#include "ui_coordinateedtwnd.h"

class CoordinateEdtWnd : public QDialog, public Ui::CoordinateEdtWnd
{
	Q_OBJECT

public:
	CoordinateEdtWnd(QWidget *parent);
	~CoordinateEdtWnd();

private:
	
};

#endif // COORDINATEEDTWND_H
