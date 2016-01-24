#include "progressdlg.h"

ProgressDlg::ProgressDlg(QWidget *parent)
	: QDialog( parent )
{
	setupUi(this);

	mStep = 1;
}

ProgressDlg::~ProgressDlg()
{

}

void ProgressDlg::setRange( int start, int end )
{
	mProgressBar->setRange ( start, end );
	mProgressBar->setValue(  start );

	int step_cal = (int) ((end-start) / 100.);
	if ( step_cal >1 )
		mStep = step_cal;
	else
		mStep = 1;	
}

void ProgressDlg::next()
{
	if ( mProgressBar->value() + mStep < mProgressBar->maximum() )
		mProgressBar->setValue( mProgressBar->value() + mStep );
}
