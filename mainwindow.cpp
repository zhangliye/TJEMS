#include "mainwindow.h"
#include "globaldata.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);

	mVideoExploreView = NULL;
	mVideoLoopView = NULL;
	mVideoTrackView = NULL;

	mPreToolName = "";
	mLabEqualBkThreshold = NULL;
	mIconDetectorOpen = NULL;
	mIconDetectorClose = NULL;
	mToolBtDectorState = NULL;
	mPlotWnd = NULL;
	mSboxPlayStepSize = NULL;	
	mDockWndMat = NULL;
	mDockWndBottom = NULL;
	mToolBarLoop = NULL;
	mToolBarVideoPlay = NULL;
	mSbxCurrentID = NULL;

	mStatusLeftWnd = NULL;
	mStatusPositionWnd = NULL;      // sub text showing position of mouse
	mStatusToolNameWnd = NULL;      // sub text showing position of mouse
	//non-GUI

	// set global data
	//
	globaldata::getInstance()->g_base_dir = QString("D:\\liye_360syn\\syn_develop\\tjems\\tjems2");
	globaldata::getInstance()->g_mainwind = this;
	
	mProgressDlg = new ProgressDlg( this );
	mProgressDlg->hide();

	createActions();
	initGUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
	// file menu
	//	
	connect( actionExit, SIGNAL(triggered()), this, SLOT(actExit()));

	// view menu
	//
	// added after dock windows finished

	// help menu
	//
	connect( actionWebsit, SIGNAL(triggered()), this, SLOT(actWebsit()));
	connect( actionManual, SIGNAL(triggered()), this, SLOT( actManual()) );
	connect( actionAbout, SIGNAL(triggered()), this, SLOT( actAbout()) );

	// set up setting menu
	//
	mVideoModelActGroup = new QActionGroup( this );
	mVideoModelActGroup->addAction( mActionExploringModel );
	mVideoModelActGroup->addAction( mActionLoopDetector );
	mVideoModelActGroup->addAction( mActionDetectionTrack );
	connect( mVideoModelActGroup, SIGNAL(triggered(QAction*)), this, SLOT( actVideoModelChanged(QAction*)) );

	// option menu
	//
	mActionSetting = new QAction("setting", this);
	connect( mActionSetting, SIGNAL(triggered()), this, SLOT(actSetting()) );
}

void MainWindow::createToolbars()
{
	// create top frame slider toolbar
	createToolbarVideoSteps();	

	// create top frame scale view toolbar
	createToolbarScale();  
	
	// controls for 3 views
	createToolbarView3D();
	createToolbarViewLoop();
	createToolbarViewTrack();
}

void MainWindow::createDockWindows()
{
	// right toolbar
	createToolBoxRight();	

	//# view switch window
	createWindSwitchTab();

	//# bottom log window
	createBottomWnd();

	// create window showing matrix
	createMatWnd();
		
	// initialize main menu
	//
	// top
	menuView->addAction( mToolBarVideoControl->toggleViewAction() ); 
	menuView->addAction( mToolBarView->toggleViewAction() ); 
	menuView->addAction( mPlotWnd->toggleViewAction() ); 
	menuView->addSeparator(); 

	// right
	menuView->addAction( mToolBoxLoop->toggleViewAction() ); 
	menuView->addAction( mToolBox3DRec->toggleViewAction() ); 
	menuView->addAction( mToolBoxTrack->toggleViewAction() ); 
	menuView->addAction( mDockWndMat->toggleViewAction() ); 
	menuView->addSeparator();

	// bottom
	menuView->addAction( mDockWndBottom->toggleViewAction() ); 	
}

void MainWindow::initGUI()
{
	// set window title
	this->setWindowTitle( globaldata::getInstance()->G_WINDOW_TITLE );

	// create tool button group for all tool buttons
	//
	mBtGroupImgTool = new QButtonGroup();
	mBtGroupImgTool->setExclusive( true );
	connect(mBtGroupImgTool, SIGNAL(buttonPressed(QAbstractButton *)), this,  SLOT(actToolChanged(QAbstractButton *)));
			
	// add top toolbars
	createToolbars();
	
	createDockWindows();	
	
	// create 3 main views
	createMainViews();

	connectMenuToProcWnd();

	// create image tools
	createToolObject();

	createStatusBar();

	// set the initial GUI for video exploring model
	actVideoModelChanged( mActionExploringModel );
}

void MainWindow::actToolChanged( QAbstractButton *button )
{
	/*# scale tools
	#
	# pan tool
	#     */  
	if (mToolBtPanMap == button){
		QString toolName = "PanMap";
		setTool( toolName, mCurrentView );
	}

	// tools for 3d reconstruction
	//
	if (mBtDrawStudyArea == button){
		QString toolName = "ToolStudyArea";
		setTool( toolName, mCurrentView );
	}

	if (mBtSetControlLine == button){
		QString toolName = "ToolCoordinateAxis";
		setTool( toolName, mCurrentView );
	}
				
	if (mBtSetControlPoint == button){
		QString toolName = "ToolControlPoint";
		setTool( toolName, mCurrentView );
	}

	if (mBtSetEditControlPoint == button){
		QString toolName = "ToolEditControlPoint";
		setTool( toolName, mCurrentView );
	}

	//////////////////////////////////////////////////////
	// tools for loop detector
	//
	if (mBtSetLoopArea == button){
		QString toolName = "ToolLoopArea";
		setTool( toolName, mCurrentView );
	}
		
	if (mBtSetLoopArea == button){
		QString toolName = "ToolLoopArea";
		setTool( toolName, mCurrentView );
	}

	if (mBtSetDeleteLoopDetector == button){
		QString toolName = "ToolDeleteLoopDetector";
		setTool( toolName, mCurrentView );
	}

	if (mBtSetBicycleHeadway == button){
		QString toolName = "ToolBicycleHeadway";
		setTool( toolName, mCurrentView );
	}

	/////////////////////////////////////////////////////////////////////
	// tools for semi-tracking tools
	//
	if (mBtSemiTracking == button){
		QString toolName = "ToolSemiTracking";
		setTool( toolName, mCurrentView );
	}	

	if (mBtMovePoint == button){
		QString toolName = "ToolMoveTrackMark";
		setTool( toolName, mCurrentView );
	}

	if (mBtDeletePoint == button){
		QString toolName = "ToolDeleteTrackMark";
		setTool( toolName, mCurrentView );
	}

	if (mBtEditTrackNode == button){
		QString toolName = "ToolEditTrackMark";
		setTool( toolName, mCurrentView );
	}

	if (mBtCopyTrackMark == button){
		QString toolName = "ToolCopyTrackMark";
		setTool( toolName, mCurrentView );
	}
	
	if (mBtTrackNewVehicle == button){
		actNextID();
	}	
}

void MainWindow::createToolBoxRight()
{
	// 1 create right toolbox for video explore 
	// 
	mToolBoxExplore =  new QToolBar( "right video explore tool" ); 
	mToolBoxExplore->setOrientation( Qt::Vertical ); 
	mToolBoxExplore->addWidget ( mBtDrawStudyArea ); 

	addToolBar(Qt::RightToolBarArea, mToolBoxExplore); 

	// 2 create right loop detector toolbox and add buttons
	//
	mToolBoxLoop =  new QToolBar( "right loop detector tool" );
	mToolBoxLoop->setOrientation( Qt::Vertical );	
	mToolBoxLoop->addWidget ( mBtSetLoopArea );
	mToolBoxLoop->addWidget ( mBtSetDeleteLoopDetector );
	mToolBoxLoop->addWidget ( mBtSetBicycleHeadway );	

	addToolBar(Qt::RightToolBarArea, mToolBoxLoop); 

	// 3 create right loop detector toolbox and add buttons
	//
	mToolBoxTrack =  new QToolBar( "Bicycle track" );
	mToolBoxTrack->setOrientation( Qt::Vertical );
	mToolBoxTrack->addWidget ( mBtSemiTracking );
	mToolBoxTrack->addWidget ( mBtDeletePoint );	
	mToolBoxTrack->addWidget ( mBtMovePoint );	
	mToolBoxTrack->addWidget ( mBtEditTrackNode );

	////////////////////////////////////////////////////////////////////////
	// track new vehicle button
	//
	QLabel* seperatorWnd = new QLabel(); 
	mToolBoxTrack->addWidget ( seperatorWnd );

	//	
	mToolBoxTrack->addWidget ( mBtCopyTrackMark );
	mToolBoxTrack->addWidget ( mBtTrackNewVehicle );
	
	// add line edit for current ID
	//
	mSbxCurrentID = new QSpinBox();
	mToolBoxTrack->addWidget( mSbxCurrentID );
	connect( mSbxCurrentID,SIGNAL(valueChanged(int)), this, SLOT(actVehicleIndexChanged(int)) );

	// add combbox for bicycle type
	//
	mCmbBikeType = new QComboBox( );
	mCmbBikeType->addItem( "Bicycle" );
	mCmbBikeType->addItem( "E-bicycle" );
	mToolBoxTrack->addWidget( mCmbBikeType );

	addToolBar(Qt::RightToolBarArea, mToolBoxTrack); 

	// 4 create right 3D reconstruction toolbox and add buttons
	//
	mToolBox3DRec =  new QToolBar( "3D reconstruction" );	
	mToolBox3DRec->addWidget ( mBtSetControlLine );
	mToolBox3DRec->addWidget ( mBtSetControlPoint );
	mToolBox3DRec->addWidget ( mBtSetEditControlPoint );
	mToolBox3DRec->addWidget ( mBtCameraCalibration );
	mToolBox3DRec->addWidget ( mBtCameraSpaceGrid );

	addToolBar(Qt::RightToolBarArea, mToolBox3DRec); 
}

void MainWindow::createWindSwitchTab()
{
	//#add switch tab bar
	//# 

	mSwithTool = new QToolBar( "window switch" );

	mMainExchangeTab = new QTabBar();
	mMainExchangeTab->setUsesScrollButtons( false );
	mMainExchangeTab->setShape( QTabBar::TriangularWest );
	mMainExchangeTab->addTab( "Video Processing" );  
	mMainExchangeTab->addTab( "3D reconstruciton" );
	mMainExchangeTab->addTab( "Trajectory analysis" );   

	mSwithTool->addWidget( mMainExchangeTab );
	addToolBar(Qt::LeftToolBarArea, mSwithTool);         

	connect( mMainExchangeTab, SIGNAL(currentChanged(int)),  this, SLOT(tabChanged(int)) );
}

void MainWindow::createBottomWnd()
{
	// create dock area
	//
	mDockWndBottom = new QDockWidget("trajectory table");
	mDockWndBottom->setAllowedAreas( Qt::LeftDockWidgetArea );

	// create bottom window and add to doc area
	//
	mBottomWnd = new BottomWnd( this );
	mDockWndBottom->setWidget( (QWidget*)mBottomWnd );
	addDockWidget( Qt::BottomDockWidgetArea, mDockWndBottom);
}

void MainWindow::createMatWnd()
{   
	//////////////////////////////////////////////////////////////////////////
	// create matrix plot window
	//
	// create dock area
	//
	mDockWndMat = new QDockWidget("Matrix window");
	mDockWndMat->setAllowedAreas( Qt::RightDockWidgetArea );

	// create matrix window for showing matrix
	//
	mMatWnd = new MatWnd( this );

	mDockWndMat->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	mDockWndMat->setWidget( (QWidget*)mMatWnd );
	addDockWidget( Qt::RightDockWidgetArea, mDockWndMat);

	//////////////////////////////////////////////////////////////////////////
	// create plot window
	//
	// create doc area
	//
	// create matrix window for showing matrix
	//
	mPlotWnd = new PlotWnd( this );	
	
	mPlotWnd->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::TopDockWidgetArea, mPlotWnd);
}

void MainWindow::createToolbarVideoSteps()
{
	// frame slider toolbar
	//
	mToolBarVideoControl = new QToolBar("top video processing toolbar");

	//#############################################################################
	//# create layout and added controls
	//#
	//# slider show progress
	//#
	QHBoxLayout *barLayout = new QHBoxLayout();
	mFrameSlider = new QSlider( Qt::Horizontal );
	mToolBarVideoControl->addWidget( mFrameSlider );

	//# label show current frame number
	//#
	mLabFrameNum = new QLabel( "0/0" );
	mLabFrameNum->setMinimumWidth( 80 );
	mToolBarVideoControl->addWidget( mLabFrameNum );

	//##############################################################################
	//# start and pause buttons
	//#
	mToolGroupVideoPlay = new QButtonGroup();   //# buttons for playing video
	mToolGroupVideoPlay->setExclusive( true );
	
	//###################################################################
	// set start frame editor
	//	
	mToolBarVideoControl->addWidget( new QLabel("start frame:") );
	mEdtStartFrame = new QLineEdit( "1" );
	mToolBarVideoControl->addWidget( mEdtStartFrame ); 

	// set start frame button
	//
	mBtSetStartFrame = new QPushButton( "set frame" );
	connect(mBtSetStartFrame, SIGNAL(clicked()), this, SLOT(actSetStartFrame()));
	mToolBarVideoControl->addWidget( mBtSetStartFrame );

	// video sampling step length (key press)
	//
	mToolBarVideoControl->addWidget( new QLabel("sampling step(frame): ") );
	mSboxStepSize = new QSpinBox();
	mSboxStepSize->setRange( 1, 300);    
	connect(mSboxStepSize, SIGNAL(valueChanged(int)), this, SLOT(actStepsizeChanged(int)));   //# step size changed action     
	mToolBarVideoControl->addWidget( mSboxStepSize );  

	// processing delay time
	//
	mToolBarVideoControl->addWidget( new QLabel("processing delay time(ms): ") );
	mSboxProcessingDelay = new QSpinBox();
	mSboxProcessingDelay->setRange( 1, 60*1000);   // 1 minute
	mSboxProcessingDelay->setSingleStep( 10 );    // 0.01 second	
	connect(mSboxProcessingDelay, SIGNAL(valueChanged(int)), this, SLOT(actProcessingDelayChanged(int)));   //# step size changed action     
	mToolBarVideoControl->addWidget( mSboxProcessingDelay );  		
	
	//////////////////////////////////////////////////////////////////////////
	// video play step length (key press)
	//
	mToolBarVideoControl->addWidget( new QLabel(" play step(frame): ") );
	mSboxPlayStepSize = new QSpinBox();
	mSboxPlayStepSize->setRange( 1, 100);   	
	connect(mSboxPlayStepSize, SIGNAL(valueChanged(int)), this, SLOT(actPlayStepSizeChanged(int)));   //# step size changed action     
	mToolBarVideoControl->addWidget( mSboxPlayStepSize ); 

	//########################################################
	//# manual adjust step
	//#
	mLabManualStep = new QLabel( "single step size(frame): 5 " );
	mToolBarVideoControl->addWidget( mLabManualStep );

	addToolBar(Qt::TopToolBarArea, mToolBarVideoControl);
}

void MainWindow::createToolbarScale()
{
	//# view scale tool buttons
	//#
	mToolBarView = new QToolBar("View scale toolbar");
	mToolBarView->addWidget( new QLabel("View scale: ") );

	//# add combox
	//# 
	mCmbViewScale = new QComboBox();
	QList<QString> strItmes;
	strItmes.append( "None" );
	strItmes.append( "25%" );
	strItmes.append( "50%" );
	strItmes.append( "75%" );
	strItmes.append( "100%" );
	strItmes.append( "150%" );
	strItmes.append( "200%" );
	strItmes.append( "250%" );
	strItmes.append( "300%" );
	strItmes.append( "400%" );
	QStringList items( strItmes );
	mCmbViewScale->addItems( items );
	mCmbViewScale->setCurrentIndex( mCmbViewScale->findText("100%") );

	mToolBarView->addWidget( mCmbViewScale );

	//# combox signal link
	//#
	connect( mCmbViewScale, SIGNAL(currentIndexChanged(int)), this, SLOT(actViewScaleChanged(int)));
	
	//# add pane map button
	//#
	QString g_base_dir = globaldata::getInstance()->g_base_dir;
	QIcon *button_icon = new QIcon( g_base_dir + "/image/pan_map.PNG");
	mToolBtPanMap = new QToolButton();
	mToolBtPanMap->setIcon( *button_icon );
	mToolBtPanMap->setCheckable( true );
	mBtGroupImgTool->addButton(mToolBtPanMap);
	mToolBarView->addWidget(mToolBtPanMap);

	//#connect( action_scale_window, SIGNAL()
	//# add scale window
	//#
	button_icon = new QIcon( g_base_dir + "/image/scale_window.PNG" );
	mToolBtScaleMap = new QToolButton();
	mToolBtScaleMap->setIcon( *button_icon );
	mToolBtScaleMap->setCheckable( true );
	mBtGroupImgTool->addButton( mToolBtScaleMap );
	mToolBarView->addWidget(mToolBtScaleMap);

	//#connect( action_scale_window, SIGNAL()
	//# add scale full button
	//#
	button_icon = new QIcon( g_base_dir + "/image/zoom_full.PNG" );
	mToolBtZoomFull = new QToolButton();
	mToolBtZoomFull->setIcon( *button_icon );
	mToolBtZoomFull->setCheckable( true );
	mBtGroupImgTool->addButton( mToolBtZoomFull );
	mToolBarView->addWidget(mToolBtZoomFull);

	addToolBar( Qt::TopToolBarArea, mToolBarView );
}

void MainWindow::createToolbarView3D()
{
	//////////////////////////////////////////////////////////////////////////
	// buttons for video play
	//
	QButtonGroup *mToolGroupPlay = new QButtonGroup();
	mToolGroupPlay->setExclusive( true );	

	QString g_base_dir = globaldata::getInstance()->g_base_dir;
	QIcon *button_icon = new QIcon( g_base_dir + "/image/start.PNG");

	mBtVideoStart = new QToolButton();
	mBtVideoStart->setIcon( *button_icon );	
	mBtVideoStart->setCheckable( true );		
	mToolGroupPlay->addButton( mBtVideoStart );        

	// pause button
	//
	button_icon = new QIcon(g_base_dir + "/image/pause.PNG");
	mBtVideoPause = new QToolButton();
	mBtVideoPause->setIcon( *button_icon );
	mBtVideoPause->setCheckable(true);	
	mToolGroupPlay->addButton( mBtVideoPause );

	// end button
	//
	button_icon = new QIcon( g_base_dir + "/image/stop.PNG");
	mBtVideoEnd = new QToolButton();
	mBtVideoEnd->setIcon( *button_icon );
	mBtVideoEnd->setCheckable(true);	
	mToolGroupPlay->addButton( mBtVideoEnd );	

	//////////////////////////////////////////////////////////////////////////
	// video play
	//
	mToolBarVideoPlay = new QToolBar("video play toolbar");
	mToolBarVideoPlay->addWidget( mBtVideoStart );   
	mToolBarVideoPlay->addWidget( mBtVideoPause );  
	mToolBarVideoPlay->addWidget( mBtVideoEnd ); 

	addToolBar(Qt::TopToolBarArea, mToolBarVideoPlay);
}

void MainWindow::createToolbarViewLoop()
{
	//////////////////////////////////////////////////////////////////////////
	// buttons for loop detector start/pause/end/ bars / detector state
	//
	QButtonGroup *mToolGroupLoop = new QButtonGroup();
	mToolGroupLoop->setExclusive( true );

	QString g_base_dir = globaldata::getInstance()->g_base_dir;
	QIcon *button_icon = new QIcon( g_base_dir + "/image/start.PNG");

	mBtLoopStart = new QToolButton();
	mBtLoopStart->setIcon( *button_icon );
	mBtLoopStart->setCheckable( true );	
	mToolGroupLoop->addButton(mBtLoopStart);        

	// pause button
	//
	button_icon = new QIcon(g_base_dir + "/image/pause.PNG");
	mBtLoopPause = new QToolButton();
	mBtLoopPause->setIcon( *button_icon );
	mBtLoopPause->setCheckable(true);		
	mToolGroupLoop->addButton( mBtLoopPause );

	// end button
	//
	button_icon = new QIcon( g_base_dir + "/image/stop.PNG");
	mBtLoopEnd = new QToolButton();
	mBtLoopEnd->setIcon( *button_icon );
	mBtLoopEnd->setCheckable(true);	
	mToolGroupLoop->addButton( mBtLoopEnd );

	// detector state button on top toolbar
	//
	mIconDetectorOpen = new QIcon( g_base_dir + "/image/detector_open.PNG");
	mIconDetectorClose = new QIcon( g_base_dir + "/image/detector_close.PNG");
	mToolBtDectorState = new QToolButton();
	mToolBtDectorState->setIcon( *mIconDetectorClose );
	mToolBtDectorState->setCheckable(false);

	//########################################################
	//# loop detector video control toolbar
	//#
	mToolBarLoop = new QToolBar("loop parameter toolbar");	

	mToolBarLoop->addWidget( mBtLoopStart );	
	mToolBarLoop->addWidget( mBtLoopPause );	
	mToolBarLoop->addWidget( mBtLoopEnd );	
	mToolBarLoop->addWidget( mToolBtDectorState );

	mCbxBikeStop = new QCheckBox("pause found bike", this);
	mCbxBikeStop->setCheckState( Qt::Checked );
	mToolBarLoop->addWidget( mCbxBikeStop );

	// co-relation rate threshold, default 0.8
	//
	mLabEqualBkThreshold = new QLabel( "Co-relation threshold: 0.8" );
	mToolBarLoop->addWidget( mLabEqualBkThreshold );

	addToolBar(Qt::TopToolBarArea, mToolBarLoop);
}

void MainWindow::createToolbarViewTrack()
{
	//////////////////////////////////////////////////////////////////////////
	// buttons for video track 
	// 
	QButtonGroup *mGroupTrack = new QButtonGroup(); 
	mGroupTrack->setExclusive( true ); 
	QString g_base_dir = globaldata::getInstance()->g_base_dir; 
	QIcon *button_icon = new QIcon( g_base_dir + "/image/start.PNG"); 

	mBtTrackStart = new QToolButton(); 
	mBtTrackStart->setIcon( *button_icon ); 
	mBtTrackStart->setCheckable( true ); 
	mGroupTrack->addButton( mBtTrackStart ); 

	// single step button
	g_base_dir = globaldata::getInstance()->g_base_dir; 
	button_icon = new QIcon( g_base_dir + "/image/next_step.PNG"); 

	mBtTrackSingleStep = new QToolButton(); 
	mBtTrackSingleStep->setIcon( *button_icon ); 
	mBtTrackSingleStep->setCheckable( true ); 
	mGroupTrack->addButton( mBtTrackSingleStep ); 

	// pause button
	//
	button_icon = new QIcon(g_base_dir + "/image/pause.PNG");
	mBtTrackPause = new QToolButton();
	mBtTrackPause->setIcon( *button_icon );
	mBtTrackPause->setCheckable(true);		
	mGroupTrack->addButton( mBtTrackPause );

	// end button
	//
	button_icon = new QIcon( g_base_dir + "/image/stop.PNG");
	mBtTrackEnd = new QToolButton();
	mBtTrackEnd->setIcon( *button_icon );
	mBtTrackEnd->setCheckable(true);	
	mGroupTrack->addButton( mBtTrackEnd );	

	//////////////////////////////////////////////////////////////////////////
	// create buttons needed for right toolbox
	// buttons for loop detector
	//
	mBtDrawStudyArea = new QPushButton("&Set Study Area");
	mBtDrawStudyArea->setCheckable( true );
	mBtGroupImgTool->addButton( mBtDrawStudyArea );

	mBtSetLoopArea = new QPushButton("&Drag loop detection area");
	mBtSetLoopArea->setCheckable( true );
	mBtGroupImgTool->addButton( mBtSetLoopArea );

	mBtSetDeleteLoopDetector = new QPushButton("&Delete loop detector");
	mBtSetDeleteLoopDetector->setCheckable( true );
	mBtGroupImgTool->addButton( mBtSetDeleteLoopDetector );    

	mBtSetBicycleHeadway = new QPushButton("&Bicycle Headway");
	mBtSetBicycleHeadway->setCheckable( true );
	mBtGroupImgTool->addButton( mBtSetBicycleHeadway );   

	// buttons for bicycle tracking
	//
	mBtSemiTracking = new QPushButton("Semi-automatic track");
	mBtGroupImgTool->addButton( mBtSemiTracking );

	mBtDeletePoint = new QPushButton("delete point");
	mBtGroupImgTool->addButton( mBtDeletePoint );	

	mBtMovePoint = new QPushButton("move point");
	mBtGroupImgTool->addButton( mBtMovePoint );

	mBtEditTrackNode = new QPushButton("edit track point");
	mBtGroupImgTool->addButton( mBtEditTrackNode );

	mBtTrackNewVehicle = new QPushButton("&track new vehicle");
	mBtGroupImgTool->addButton( mBtTrackNewVehicle );

	mBtCopyTrackMark = new QPushButton("&copy track vehicle");
	mBtGroupImgTool->addButton( mBtCopyTrackMark );
	
	// buttons for 3D reconstruction
	//	
	mBtSetControlLine = new QPushButton("&set control line");
	mBtGroupImgTool->addButton( mBtSetControlLine );      
	//connect( mBtSetControlLine, SIGNAL( "clicked()" ), this, SLOT(actSetControlLine()) );      

	mBtSetControlPoint = new QPushButton("&set control point");
	mBtGroupImgTool->addButton( mBtSetControlPoint );
	//connect( mBtSetControlPoint, SIGNAL( "clicked()" ), this, SLOT(actSetControlPoint())  );

	mBtSetEditControlPoint = new QPushButton("&Edit control point");
	mBtGroupImgTool->addButton( mBtSetEditControlPoint );	

	mBtCameraCalibration = new QPushButton("&camera calibration"); 
	mBtGroupImgTool->addButton( mBtCameraCalibration ); 
	connect( mBtCameraCalibration, SIGNAL( "clicked()" ), this, SLOT(actCalibrateCamera()) );   	
	
	mBtCameraSpaceGrid = new QPushButton("&space grid"); 
	mBtGroupImgTool->addButton( mBtCameraCalibration ); 
	connect( mBtCameraSpaceGrid, SIGNAL( "clicked()" ), this, SLOT(actDrawSpaceGrid()) );   	

	//////////////////////////////////////////////////////////////////////////
	// video track toolbar
	//
	mToolBarTrack = new QToolBar("video track control toolbar");	
	mToolBarTrack->addWidget( mBtTrackStart );	
	mToolBarTrack->addWidget( mBtTrackSingleStep );		
	mToolBarTrack->addWidget( mBtTrackPause );	
	mToolBarTrack->addWidget( mBtTrackEnd );

	addToolBar(Qt::TopToolBarArea, mToolBarTrack);
}

void MainWindow::actNextID()
{
	ToolSemiTracking* semiTrackTool = (ToolSemiTracking*)( mToolMap["ToolSemiTracking"] );
	semiTrackTool->actNextID(); 
}

void MainWindow::createToolObject()
{
	///create tool object and save in maps
	mToolMap.clear();   

	// scale tools
	//
	mToolMap["PanMap"] = new ToolPan( mVideoExploreView );

	// 3D reconstruction tools
	//
	mToolMap["ToolCoordinateAxis"] = new ToolCoordinateAxis( mVideoExploreView );
	mToolMap["ToolControlPoint"] = new ToolControlPoint( mVideoExploreView );

	// video tools
	//
	mToolMap["ToolStudyArea"] = new ToolStudyArea( mVideoExploreView );
	mToolMap["ToolLoopArea"] = new ToolLoopArea( mVideoLoopView );
	mToolMap["ToolDeleteLoopDetector"] = new ToolDeleteLoopDetector( mVideoLoopView );
	mToolMap["ToolBicycleHeadway"] = new ToolBicycleHeadway( mVideoLoopView );

	// semi-track tools
	//
	mToolMap["ToolSemiTracking"] = new ToolSemiTracking( mVideoTrackView );
	mToolMap["ToolMoveTrackMark"] = new ToolMoveTrackMark( mVideoTrackView );
	mToolMap["ToolDeleteTrackMark"] = new ToolDeleteTrackMark( mVideoTrackView );
	mToolMap["ToolEditTrackMark"] = new ToolEditTrackMark( mVideoTrackView );
	mToolMap["ToolCopyTrackMark"] = new ToolCopyTrackMark( mVideoTrackView );	
}

void MainWindow::setTool( const QString &tool_name, VideoViewBase *video_view )
{
	//''' uninstall old tool, install new tool
	//'''
	//# uninstall old tool
	//# 
	if ( mToolMap.contains( mPreToolName ) ){
		mToolMap[ mPreToolName ]->uninstall( video_view ); 
		QString msg = mPreToolName + " uninstalled";
		log( msg );
	}

	//
	//# install new tool
	//# 
	if (mToolMap.contains( tool_name )){
		mToolMap[ tool_name ]->install( video_view );   

		// change state and marker
		//
		if ( tool_name=="ToolBicycleHeadway" ) {
			setDetectorState( true );  // show detector state on top bar	
		} else {
			setDetectorState( false );  // show detector state on top bar
		}

		QString msg = tool_name + " installed";  
		log( msg );
		mPreToolName = tool_name;   

		// show tool name in status bar
		mStatusToolNameWnd->setText( QString("Tool:   ")+tool_name );
	}
}

void MainWindow::actRecognizeVehicle()
{
	setCursor( Qt::CrossCursor );
}

void MainWindow::tabChanged( int index )
{
	QMessageBox::information(NULL, "tabChanged", "" );
}

void MainWindow::connectMenuToProcWnd()
{ 
	connect( mActionNewProject, SIGNAL(triggered()), this, SLOT(actNewProject())  );
	connect( mActionOpenProject, SIGNAL(triggered()), this, SLOT(openProject())  );
	connect( actionOpen_Video_Files, SIGNAL(triggered()), this, SLOT(actOpenNewFile())  );
	connect( actionOpen_Current_Video, SIGNAL(triggered()), mVideoExploreView, SLOT(openVideo())  );

	connect( actionNext_Frame, SIGNAL(triggered()), this, SLOT(actNextFrame()) );
	connect( actionFinish_processing, SIGNAL(triggered()), this, SLOT(actEndProcessing()) );
	connect( actionSetting, SIGNAL(triggered()), mVideoExploreView, SLOT(actSetting()) );
}

void MainWindow::actNextFrame()
{
	if ( mCurrentView != NULL )
		mCurrentView->actNextFrame();
}

void MainWindow::actWebsit()
{
	QDesktopServices::openUrl( QUrl("http://opentrans.cn", QUrl::TolerantMode) );
}

void MainWindow::actManual()
{
	QString docPath = "file:///" + globaldata::getInstance()->g_base_dir + "\\doc\\manual.pdf";
	QDesktopServices::openUrl(QUrl( docPath ));
}

void MainWindow::actAbout()
{
	QString message("");
	message += "<p>This is an Trajectory extraction and management platform for traffic flow study</p>";
	message += "<p>The first version is developed by Liye in University of Singapore (2013-2014)</p>";
	message += "<p><a href= \"mailto:chinazhangly@126.com\">Author email: chinazhangly@126.com</a></p>";
	QMessageBox::information( this, "About TJEMS", message);
}

//''' insert string message in first row of log window at bottom
//	msg - string
//	'''
void MainWindow::log( const QString &msg )
{
	QString msg_show = QString("%1:  %2").arg( mBottomWnd->mLstWidLog->count()+1 ).arg(msg);
	mBottomWnd->mLstWidLog->insertItem( 0, msg_show );
	QApplication::instance()->processEvents();   // make GUI active
}

void MainWindow::actSetting()
{

}

void MainWindow::actViewScaleChanged( int index )
{
	QString scale_text = mCmbViewScale->itemText( index );

	float rate = 1;
	if (scale_text == "25%")
		rate = 0.25;         
	else if (scale_text == "50%")
		rate = 0.5;
	else if (scale_text == "75%")
		rate = 0.75;
	else if (scale_text == "100%")
		rate = 1.;
	else if (scale_text == "150%")
		rate = 1.5;
	else if (scale_text == "200%")
		rate = 2.;
	else if (scale_text == "250%")
		rate = 2.5;
	else if (scale_text == "300%")
		rate = 3.;
	else if (scale_text == "400%")
		rate = 4.;

	QMatrix transM = QMatrix(rate, 0, 0, rate, 0, 0);   //#qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy
	mCurrentView->setMatrix( transM, false );
}

void MainWindow::actSetStartFrame()
{
	//action, set starting frame of video

	mCurrentView->pauseProcessing();   //# stop the auto play

	int startFrame;
	try{
		startFrame = mEdtStartFrame->text().toInt();
	} catch(...) {
		startFrame = 1;
	}
	
	mVideoExploreView->readFrame( startFrame, true );	
	mVideoExploreView->showFrame( mVideoExploreView->mCurrentFrame );
}

void MainWindow::actStepsizeChanged( int step)
{
	try{
		if (mVideoExploreView!=NULL)
			mCurrentView->mProStep = step;
	} catch(...) {
		;
	}
}

void MainWindow::actProcessingDelayChanged( int step )
{
	mVideoExploreView->mTimer->setInterval( step );
}

//
//
void MainWindow::actOpenNewFile()
{
	//'''open new video file '''
	QString videoFilePath = QFileDialog::getOpenFileName( this, "select UAV video file ", "", "UAV vidoe file  ( *.avi )" );
	try{
		if(videoFilePath.isEmpty()) {
			QMessageBox::information( this, "path error", "please select a valid UAV flight project file" );
			return;
		}
	} catch(...) {
		QMessageBox::information( this, "path error", "path contains not common characteristics" );
	}

	if ( QFile::exists(videoFilePath) )
	{
		mVideoExploreView->openNewFile( videoFilePath );
		mCurrentVideoFile = videoFilePath;
		mPrjFileObj.mVideoPath = mCurrentVideoFile;
	}	
}

void MainWindow::setDetectorState( bool isOpen )
{
	if (isOpen) {
		mToolBtDectorState->setIcon( *mIconDetectorOpen );
		static_cast<VideoLoopTrackView*>(mCurrentView)->mDetectionModel = true;
	} else {
		mToolBtDectorState->setIcon( *mIconDetectorClose );
		static_cast<VideoLoopTrackView*>(mCurrentView)->mDetectionModel = false;
	}
}

void MainWindow::actPlayStepSizeChanged( int step )
{
	mVideoExploreView->mPlayStep = step;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	// Save project file
	mPrjFileObj.saveProjectFile();

	switch( QMessageBox::information( NULL, QString("close program"),
		QString("Do you really want to close this program?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) ) 
	{
	case QMessageBox::Yes:	
		event->accept();
		QMainWindow::closeEvent(event);
		break;
	case QMessageBox::No:
	default: 
		event->ignore();
		break; 
	}  
}

void MainWindow::actExit()
{
	this->close();
}

void MainWindow::actVideoModelChanged( QAction *action )
{	
	// verify if study exists
	//
	bool study_arae_ok = false;
	if ( mToolMap.contains("ToolStudyArea") )
	{
		int pt_number = mInterestingArea.size();
		study_arae_ok = static_cast<ToolStudyArea*>(mToolMap["ToolStudyArea"])->valid();
		if ( pt_number>0 || study_arae_ok )
		{
			study_arae_ok = true;
		}
	}

	if ( action == mActionExploringModel ) {            // exploring video
		mToolBarVideoControl->show();

		mToolBarVideoPlay->show();
		mToolBarLoop->hide();				
		mToolBarTrack->hide();

		mToolBoxExplore->show();
		mToolBoxLoop->hide();
		mToolBoxTrack->hide();
		mToolBox3DRec->show();
		mDockWndMat->hide();
		mPlotWnd->hide();
		mDockWndBottom->show();

		mVideoExploreView->show();
		mVideoLoopView->hide();
		mVideoTrackView->hide();
		mCurrentView = (VideoViewBase *)mVideoExploreView;
	} else if ( action == mActionLoopDetector ) {           // loop detector
		// video processing model, need study area
		if (!study_arae_ok)
		{
			QMessageBox::information(this, "", "please set the study area!");
			mVideoModelPreAction->setChecked( true );
			return;
		}						

		mToolBarVideoPlay->hide();
		mToolBarLoop->show();				
		mToolBarTrack->hide();

		mToolBoxExplore->hide();
		mToolBoxLoop->show();
		mToolBoxTrack->hide();
		mToolBox3DRec->hide();
		mDockWndMat->show();
		mPlotWnd->show();
		mDockWndBottom->show();

		mVideoExploreView->hide();
		mVideoLoopView->show();
		mVideoTrackView->hide();
		mCurrentView = (VideoViewBase *)mVideoLoopView;

	} else if ( action == mActionDetectionTrack )	{           // video detection and tracking
		// video processing model, need study area
		if (!study_arae_ok)
		{
			QMessageBox::information(this, "", "please set the study area!");
			mVideoModelPreAction->setChecked( true );
			return;
		}

		mToolBarVideoPlay->hide();
		mToolBarLoop->hide();				
		mToolBarTrack->show();

		mToolBoxExplore->hide();
		mToolBoxLoop->hide();
		mToolBoxTrack->show();
		mToolBox3DRec->hide();
		mDockWndMat->show();
		mPlotWnd->hide();
		mDockWndBottom->show();

		mVideoExploreView->hide();
		mVideoLoopView->hide();
		mVideoTrackView->show();
		mCurrentView = (VideoViewBase *)mVideoTrackView;
	}

	// if video file not opened in current view, open
	//
	if ( !mCurrentView->videoOpened() )  // not opened
	{
		if( mCurrentVideoFile != "" )
			mCurrentView->openNewFile( mCurrentVideoFile );
			mCurrentView->fitInView( mCurrentView->mScene->itemsBoundingRect(), Qt::KeepAspectRatio );
			mCmbViewScale->setCurrentIndex( mCmbViewScale->findText("None") );
	}

	// release, set to pan tool
	releaseTool();

	// record previous menu selection
	mVideoModelPreAction = action;	
}

void MainWindow::createMainViews()
{
	//#########################################################################
	// add main view for video processing
	// 
	// set the video processing window
	//
	QVBoxLayout *mMainWndLayout = new QVBoxLayout;
	QWidget *mMainWnd = new QWidget( this );

	// create three video processing model, and set explored model
	//
	mVideoExploreView = new VideoExploreView( this );
	mVideoLoopView = new VideoLoopTrackView( this );
	mVideoTrackView = new VideoSemiTrackView( this );

	// set current central window
	mVideoExploreView->show();
	mVideoLoopView->hide();
	mVideoTrackView->hide();

	mMainWndLayout->addWidget( mVideoExploreView );
	mMainWndLayout->addWidget( mVideoLoopView );
	mMainWndLayout->addWidget( mVideoTrackView );

	mMainWnd->setLayout( mMainWndLayout );
	setCentralWidget( mMainWnd );

	// main window and VideoViewBase all ok, its time to set video delay time value
	//
	mSboxProcessingDelay->setValue( mVideoExploreView->mTimer->interval() );
	mSboxPlayStepSize->setValue( mVideoExploreView->mPlayStep );

	connect( mBtVideoStart, SIGNAL(pressed()), mVideoExploreView, SLOT(actStartProcessing()));
	connect( mBtVideoPause, SIGNAL(pressed()), mVideoExploreView, SLOT(actPauseProcessing()));	
	connect( mBtVideoEnd, SIGNAL(pressed()), mVideoExploreView, SLOT(actEndProcessing()));

	connect( mBtLoopStart, SIGNAL(pressed()), mVideoLoopView, SLOT(actStartProcessing()));
	connect( mBtLoopPause, SIGNAL(pressed()), mVideoLoopView, SLOT(actPauseProcessing()));
	connect( mBtLoopEnd, SIGNAL(pressed()), mVideoLoopView, SLOT(actEndProcessing()));

	connect( mBtTrackStart, SIGNAL(pressed()), mVideoTrackView, SLOT(actStartProcessing()));
	connect( mBtTrackSingleStep, SIGNAL(pressed()), mVideoTrackView, SLOT(actNextFrame()));	
	connect( mBtTrackPause, SIGNAL(pressed()), mVideoTrackView, SLOT(actPauseProcessing()));
	connect( mBtTrackEnd, SIGNAL(pressed()), mVideoTrackView, SLOT(actEndProcessing()));
}

void MainWindow::releaseTool()
{
	QString toolName = "PanMap";
	setTool( toolName, mCurrentView );
}

void MainWindow::updateMask( const Mat &mat)
{
	// set detector background 
	//
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	mInterestingArea.boundingRect().getCoords(&xMin, &yMin, &xMax, &yMax);

	if( xMin<0 )
		xMin = 0;
	if( yMin<0 )
		yMin = 0;
	if ( xMax>= mat.cols )
		xMax = mat.cols;
	if ( yMax>= mat.rows )
		yMax = mat.rows;

	// create small matrix to return
	Mat subM = mat( Range(yMin, yMax), Range(xMin, xMax) ).clone(); //row, col	

	vector<Point> contSingle;
	QPointF pt;
	int x;
	int y;
	for (int i=0; i< mInterestingArea.size(); ++i)
	{
		pt = mInterestingArea.at(i);
		x = pt.x()-xMin >= 0 ? pt.x()-xMin : 0;
		y = pt.y()-yMin >= 0 ? pt.y()-yMin : 0;
		contSingle.push_back( Point(x, y) );	
	}

	vector<vector<Point>> contours;
	contours.push_back( contSingle );

	// create zero matrix and mark mask
	mMask = Mat( Mat::zeros(subM.size(), subM.type()) );
	fillPoly( mMask, contours, cv::Scalar(256,256,256));	
}

void MainWindow::actNewProject()
{
	NewProjectDlg newPrjDlg(this);
	if ( newPrjDlg.exec()== QDialog::Accepted ){
		QString prjName =  newPrjDlg.mEdtName->text();
		QString prjDescrition =  newPrjDlg.mEdtDescription->text();
		QString prjAuthor =  newPrjDlg.mEdtAuthor->text();
		QString prjDir = newPrjDlg.mEdtPrjectDir->text();

		prjDir += "/" + prjName;
		mPrjFileObj.createProject(prjDir, prjName, prjDescrition, prjAuthor);

		//newProject(prjName, prjDir, prjDescrition, prjAuthor );
	}
}

void MainWindow::newProject( QString& prjName, QString& prjDir, QString& prjDescrition, QString& prjAuthor )
{
	/* create new project files and open
	   prjName - string; prjDir- full dir for project; prjDescrition; prjAuthor - string, author name
	*/

	QDir dir = QDir::root(); 
	QString prjDirSub = prjDir + "/" + prjName; 
	try{ 
		dir.mkdir( prjDirSub ); 
		//mSimData.createEmptySimFile( prjDirSub, prjName ) 
		//mSimData.setProjectInf( prjName, prjDescrition, prjAuthor ) 
		//mSimData.saveData()     
	} catch(...) {
		QMessageBox::information(this, "directory error", "wrong directory, please use other directory");
		return;
	}
	//readPrjFile( self.mSimData.mSimFilePath );	
}

void MainWindow::openProject()
{
	//'''open new video file '''
	//
	QString videoFilePath = QFileDialog::getOpenFileName( this, "select project file ", "", "project file  ( *.prj )" );
	try{
		if(videoFilePath.isEmpty()) {
			QMessageBox::information( this, "path error", "please select a valid UAV flight project file" );
			return;
		}
	} catch(...) {
		QMessageBox::information( this, "path error", "path contains not common characteristics" );
	}

	if ( mPrjFileObj.openPrjectFile( videoFilePath ) )  // parse XML file opened
	{
		// open real contents
		//
		if ( !QFile::exists( mPrjFileObj.mVideoPath ) ){
			QMessageBox::information( this, "video error", "video file not found" );
			return;
		}
				
		///////////////////////////////////////////////////////////////////////
		// add study area in Main Window
		//
		QVector<QPoint> points;
		int x;
		int y;
		int length = mPrjFileObj.mStudyAreaPoints.size();
		for (int i=0; i<length; ++i){
			x = mPrjFileObj.mStudyAreaPoints[i].x();
			y = mPrjFileObj.mStudyAreaPoints[i].y();
			points.append( QPoint(x, y) );
		}
		mInterestingArea = QPolygon( points );

		// open the video in three explorer
		//
		mVideoExploreView->openNewFile( mPrjFileObj.mVideoPath );       // this function will clear GraphicsScene 
		mVideoTrackView->openNewFile( mPrjFileObj.mVideoPath );         // this function will clear GraphicsScene 
		mCurrentVideoFile = mPrjFileObj.mVideoPath;

		// add Trajectory points to GraphicsItem to VideoTrackView
		//
		mPrjFileObj.addItemsToView( mVideoTrackView );

		// show items on the current time instance, default 0 time
		showPointsAtTime( int( mVideoTrackView->mCurrentFrameIndex ) );

		////////////////////////////////////////////////////////////////////////////////////////
		// draw study area in mVideoExploreView
		//
		int x1;
		int y1;
		int x2;
		int y2;
		length = mPrjFileObj.mStudyAreaPoints.size();
		for (int i=0; i<length-1; ++i)
		{
			x1 = mPrjFileObj.mStudyAreaPoints[i].x();
			y1 = mPrjFileObj.mStudyAreaPoints[i].y();
			x2 = mPrjFileObj.mStudyAreaPoints[i+1].x();
			y2 = mPrjFileObj.mStudyAreaPoints[i+1].y();

			QGraphicsLineItem *line = new QGraphicsLineItem( x1, y1, x2, y2  );
			line->setPen( QPen(Qt::green) );	
			mVideoExploreView->scene()->addItem( line );  //# show line
		}

		// draw last end and first end
		//
		x1 = mPrjFileObj.mStudyAreaPoints[length-1].x();
		y1 = mPrjFileObj.mStudyAreaPoints[length-1].y();
		x2 = mPrjFileObj.mStudyAreaPoints[0].x();
		y2 = mPrjFileObj.mStudyAreaPoints[0].y();

		QGraphicsLineItem *line = new QGraphicsLineItem( x1, y1, x2, y2  );
		line->setPen( QPen(Qt::green) );	
		mVideoExploreView->scene()->addItem( line );  //# show line
	}
}

// QMap<int, QMap<int, TrajectoryPoint*>> mTrajectory;    //{FrameIndex: {VehilcleID:QPoint, ...}}
void MainWindow::showPointsAtTime( int timeInstance )
{
	QMapIterator<int, QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> > i(mPrjFileObj.mTrajectory);
	while ( i.hasNext() ) {
		i.next();  // move next
		int time = i.key();
		QMap<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> ptsMap = i.value();

		QMapIterator<int, QPair<TrajectoryPoint*, QGraphicsTextItem*>> j( ptsMap );
		while ( j.hasNext() )
		{
			j.next();  // move next
			
			int veicleID = j.key();
			QPair<TrajectoryPoint*, QGraphicsTextItem*> pt_text = j.value();
			TrajectoryPoint* pt = (TrajectoryPoint*)pt_text.first;
			QGraphicsTextItem* text = (QGraphicsTextItem*)pt_text.second;
			try
			{
				if ( pt!=NULL && text!=NULL )
				{
					if ( time!= timeInstance) {
						pt->hide();
						text->hide();
					} else {
						pt->show();
						text->show();
					}
				}
				else{
					QString msg= QString("Trajectory point error: time(%1), vehicle(%2)").arg(time).arg(veicleID);
					log( msg );
				}
			}
			catch (...)
			{
				int a = 0;
			}			
		}		
	}			
}

void MainWindow::actVehicleIndexChanged( int newIndex )
{
	ToolSemiTracking* tool = (ToolSemiTracking*)mToolMap["ToolSemiTracking"];
	tool->mCurrentVehicleID = newIndex;
}

void MainWindow::createStatusBar()
{
	mStatusLeftWnd = new QLabel();
	mStatusLeftWnd->setAlignment( Qt::AlignLeft );
	this->mStatusbar->insertWidget( 0, mStatusLeftWnd, 1 );
		
	mStatusToolNameWnd = new QLabel();
	mStatusToolNameWnd->setFrameStyle( QFrame::Panel | QFrame::Sunken  );
	mStatusToolNameWnd->setAlignment( Qt::AlignLeft );
	mStatusToolNameWnd->setMinimumWidth( 300 );
	mStatusToolNameWnd->setFixedWidth( 300 );
	this->mStatusbar->insertWidget( 1, mStatusToolNameWnd, 0);

	mStatusPositionWnd = new QLabel();
	mStatusPositionWnd->setFrameStyle( QFrame::Panel | QFrame::Sunken  );
	mStatusPositionWnd->setAlignment( Qt::AlignLeft );
	mStatusPositionWnd->setMinimumWidth( 150 );
	mStatusPositionWnd->setFixedWidth( 150 );
	this->mStatusbar->insertWidget( 2, mStatusPositionWnd, 0);
}

int MainWindow::getBikeType()
{
	if (mCmbBikeType->currentText()=="Bicycle")
	{
		return(1);
	} else if (mCmbBikeType->currentText()=="E-bicycle")
	{
		return(1);
	}

	return(-1);	
}

void MainWindow::setBikeType( int bikeType )
{
	if (bikeType==1)
	{
		mCmbBikeType->setCurrentIndex( mCmbBikeType->findText("Bicycle") );
	} else if ( bikeType==2 )
	{
		mCmbBikeType->setCurrentIndex( mCmbBikeType->findText("E-bicycle") );
	}
}

QPair<TrajectoryPoint*, QGraphicsTextItem*> MainWindow::findTrajPoint( VideoViewBase*view, int frmIndex, int vehicleID )
{
	// save trajectory point into main window
	// 
	QPair<TrajectoryPoint*, QGraphicsTextItem*> pair;
	pair.first = NULL;
	pair.second = NULL;

	if ( !mPrjFileObj.mTrajectory.contains(frmIndex) ) // not found points at time instance
	{
		return( pair );
	} else {  //found point at given time instance

		if ( mPrjFileObj.mTrajectory[frmIndex].contains( vehicleID ) ) // same vehicle at the same time, error message
		{
			return( pair );
		} else {
			return( mPrjFileObj.mTrajectory[frmIndex][ vehicleID ] );
		}
	}

	return( pair );
}



