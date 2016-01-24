#include "trajedtdlg.h"

TrajEdtDlg::TrajEdtDlg( QWidget *parent, ToolEditTrackMark *editTool )
	: QDialog( parent )
{
	setupUi(this);
	mTrajEditTool = editTool;

	connect( mBtSave, SIGNAL( pressed() ), this, SLOT(actSave()) );
	connect( mBtCancel, SIGNAL( pressed() ), this, SLOT(actCancel()) );

	mCmbBikeType->addItem( "Bicycle" );
	mCmbBikeType->addItem( "E-bicycle" );
}

TrajEdtDlg::~TrajEdtDlg(void)
{

}

void TrajEdtDlg::setBikeType( int bikeType )
{
	if (bikeType==1)
	{
		mCmbBikeType->setCurrentIndex( mCmbBikeType->findText("Bicycle") );
	} else if ( bikeType==2 )
	{
		mCmbBikeType->setCurrentIndex( mCmbBikeType->findText("E-bicycle") );
	}
}

int TrajEdtDlg::getBikeType()
{
	if (mCmbBikeType->currentText()=="Bicycle")
	{
		return(1);
	} else if (mCmbBikeType->currentText()=="E-bicycle")
	{
		return(2);
	}

	return(-1);
}

void TrajEdtDlg::actSave()
{
	if ( mTrajEditTool->mNearPt.first != NULL )
	{
		// change point data
		mTrajEditTool->mNearPt.first->bikeType( getBikeType() );
		mTrajEditTool->mNearPt.first->bikeID( mSpinID->value() );
		
		// unselect the current point
		mTrajEditTool->mNearPt.first->clearSelected();
	}

	// change label text
	if ( mTrajEditTool->mNearPt.second!=NULL )
	{
		mTrajEditTool->mNearPt.second->setPlainText( QString("%1").arg( mSpinID->value() ) );
	}
	
	this->hide();
}

void TrajEdtDlg::actCancel()
{
	// change label text
	if ( mTrajEditTool->mNearPt.second!=NULL )
	{
		mTrajEditTool->mNearPt.second->setPlainText( QString("%1").arg( mSpinID->value() ) );
	}

	this->hide();
}
