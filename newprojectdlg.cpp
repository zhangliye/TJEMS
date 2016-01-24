#include "newprojectdlg.h"
#include <QtGui/QFileDialog>

NewProjectDlg::NewProjectDlg(QWidget *parent)
	: QDialog( parent )
{
	setupUi(this);

	mStep = 1;

	connect( mBtOk, SIGNAL(pressed()), this, SLOT( accept() ) );
	connect( mBtCancel, SIGNAL(pressed()), this,  SLOT( reject() ) );
	connect( mBtBrowse, SIGNAL(pressed()), this,  SLOT( actSelectDir() ));
}

NewProjectDlg::~NewProjectDlg(void)
{
}

void NewProjectDlg::actSelectDir()
{
	QString dir_text = QFileDialog::getExistingDirectory(this, "select project Directory", "c:/", QFileDialog::ShowDirsOnly);
	mEdtPrjectDir->setText( dir_text );
}


