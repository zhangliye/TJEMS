#ifndef BOTTOMWND_H
#define BOTTOMWND_H

#include "pub.h"
#include "ui_bottomwnd.h"

class BottomWnd : public QWidget, public Ui::BottomWnd
{
	Q_OBJECT

public:
	BottomWnd( QWidget *parent );
	~BottomWnd();

private:	
};

#endif // BOTTOMWND_H
