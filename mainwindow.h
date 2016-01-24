#ifndef TJEMS_H
#define TJEMS_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "videoviewbase.h"
#include "videoexploreview.h"
#include "videolooptrackview.h"
#include "videosemitrackview.h"

#include "bottomwnd.h"
#include "matwnd.h"
#include "toolimage.h"
#include "toolpan.h"
#include "toolstudyarea.h"

#include "toollooparea.h"
#include "tooldeleteloopdetector.h"
#include "toolbicycleheadway.h"
#include "toolcoordinateaxis.h"
#include "toolcontrolpoint.h"
#include "tooleditcontrolpoint.h"

#include "toolsemitracking.h"
#include "toolmovetrackmark.h"
#include "tooledittrackmark.h"
#include "tooldeletetrackmark.h"
#include "toolcopytrackmark.h"
#include "plotwnd.h"
#include "toolstudyarea.h"

// dialogs
#include "progressdlg.h"
#include "newprojectdlg.h"
#include "projectfile.h"
#include <QtGlobal>

// global data
class VideoViewBase;
class VideoLoopTrackView;
class VideoSemiTrackView;
class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	void createActions();
	void initGUI();		

	void createMainViews();

	/************************************************************************/
	/* function    : create right dock windows and top toolbars, and add menus
	/*               to top main bar
	/************************************************************************/
	void createDockWindows();

	void createToolbars();
	
	//////////////////////////////////////////////////////////////////////////
	// create dock windows
	void createToolBoxRight(); 
	void createWindSwitchTab();
	void createBottomWnd();
	void createMatWnd();                     // window showing matrix 
	void createToolbarVideoSteps();

	/************************************************************************/
	/* function    : create top toolbars for video play control/ loopdetector control
	/*               and video track control
	/************************************************************************/
	void createToolbarView3D();
	void createToolbarViewLoop();
	void createToolbarViewTrack();
	
	void createToolbarScale();

	void connectMenuToProcWnd();
	void createToolObject();
	void log( const QString &msg );
	void setTool(const QString &tool_name, VideoViewBase *video_view);

	// set tool to pan, safe one
	void releaseTool();

	/************************************************************************/
	/* function    : set state of detection, showing on top bar
	/* parameter   : isOpen - true if device opening
	/************************************************************************/
	void setDetectorState(bool isOpen);

	/************************************************************************/
	/* function    : update mMask matrix
	/* parameter   : mat - whole image
	/************************************************************************/
	void updateMask(const Mat &mat);

	// new project 
	void newProject(QString& prjName, QString& prjDir, QString& prjDescrition, QString& prjAuthor );
	
	// show trajectory points in sem-track view
	void showTrajectoryPoints(){}

	// add position widget
	void createStatusBar();

	QPair<TrajectoryPoint*, QGraphicsTextItem*> findTrajPoint( VideoViewBase*view, int frmIndex, int vehicleID);

protected:
	virtual void closeEvent(QCloseEvent *event);   // user verification before closing program

public slots:
	void actRecognizeVehicle();
	void tabChanged(int index);		
	void actToolChanged(QAbstractButton *button);
	void actPlayStepSizeChanged( int step );
	void actNextFrame();

	//////////////////////////////////////////////////////////////////////////
	// menu actions
	//
	// file
	void actNewProject();
	void openProject();
	void actOpenNewFile();
	void actExit();	

	// setting
	void actSetting();

	// user changed video processing model
	void actVideoModelChanged(QAction *action);

	// help menu act
	//
	void actWebsit();
	void actManual();
	void actAbout();

	void actNextID();
	//////////////////////////////////////////////////////////////////////////
	// GUI reactions
	//
	void actViewScaleChanged( int index );
	void actSetStartFrame();
	void actStepsizeChanged(int step);
	void actProcessingDelayChanged(int step);   // step - ms 1/1000 second		
	
	// actions for 3d reconstruction tools
	//
	void actDrawSpaceGrid(){}
	void actCalibrateCamera(){}

	void showPointsAtTime(int timeInstance=0 );

	void actVehicleIndexChanged(int newIndex);

	// access bike type from right combox
	//
	// 1 - bike; 2 - e-bike
	void setBikeType(int bikeType );
	int getBikeType();

public:
	// QGraphicsView show items                  
	VideoViewBase        *mCurrentView;       // pointer to the current view
	VideoExploreView     *mVideoExploreView;  // no video processing model
	VideoLoopTrackView   *mVideoLoopView;     // loop detection model
	VideoSemiTrackView   *mVideoTrackView;    // bicycle detection and tracking		

	//////////////////////////////////////////////////////////////////////////
	// main menu actions
	//
	QActionGroup *mVideoModelActGroup; 
	QAction      *mVideoModelPreAction;    // previous action of the videl model menu
	 
	//////////////////////////////////////////////////////////////////////////
	//                      video sample control toolbar
	//////////////////////////////////////////////////////////////////////////
	//
	QToolBar *mToolBarVideoControl;        // top video play bar

	// controls for top video auto processing toolbars
	QSlider *mFrameSlider;             // slider showing processing porgress
	QLabel  *mLabFrameNum;              // current processing label
	QButtonGroup *mToolGroupVideoPlay;	
	QLineEdit    *mEdtStartFrame;
	QPushButton  *mBtSetStartFrame;
	QSpinBox     *mSboxStepSize;
	QSpinBox     *mSboxProcessingDelay;    // processing delay ms (video showing delay)
	QLabel       *mLabManualStep;
	QLabel       *mLabEqualBkThreshold;
	QSpinBox     *mSboxPlayStepSize;

	QIcon *mIconDetectorOpen;          // Icon indicate detector state
	QIcon *mIconDetectorClose;			

	//////////////////////////////////////////////////////////////////////////
	//                       view scale
	//////////////////////////////////////////////////////////////////////////
	// ScaleToolbar buttons
	//
	QToolBar *mToolBarView;

	QComboBox    *mCmbViewScale;  
	QToolButton  *mToolBtPanMap;  
	QToolButton  *mToolBtScaleMap; 
	QToolButton  *mToolBtZoomFull; 

	//////////////////////////////////////////////////////////////////////////
	//                             video 3d reconstruction
	//////////////////////////////////////////////////////////////////////////
	// video play toolbar
	//
	QToolBar *mToolBarVideoPlay;
		
	// video play buttons
	QToolButton *mBtVideoStart;
	QToolButton *mBtVideoPause;
	QToolButton *mBtVideoEnd;

	QToolBar    *mToolBoxExplore;
	QPushButton *mBtDrawStudyArea;	

	//////////////////////////////////////////////////////////////////////////
	//                            loop detector view
	//////////////////////////////////////////////////////////////////////////
	// 3-1 top: loop tool bar
	QToolBar *mToolBarLoop;		

	// loop detector buttons
	QToolButton *mBtLoopStart; 
	QToolButton *mBtLoopPause; 
	QToolButton *mBtLoopEnd; 
	QToolButton *mToolBtDectorState;   // button showing detector state
	QCheckBox   *mCbxBikeStop;    // when found bicycle, stop video

	QToolBar *mToolBoxLoop; 

	// buttons for loop detector	
	QPushButton *mBtSetLoopArea;
	QPushButton *mBtSetDeleteLoopDetector;
	QPushButton *mBtSetBicycleHeadway;

	//////////////////////////////////////////////////////////////////////////
	//                           detection and track view
	//////////////////////////////////////////////////////////////////////////
	// top: loop tool bar
	QToolBar *mToolBarTrack;

	// right box
	QToolBar *mToolBoxTrack; 

	// bicycle detection and track buttons
	QToolButton *mBtTrackStart;
	QToolButton *mBtTrackSingleStep;
	QToolButton *mBtTrackPause;
	QToolButton *mBtTrackEnd;

	// buttons for semi tracking
	//
	QPushButton *mBtSemiTracking;    // set auto tracking area
	QPushButton *mBtDeletePoint;     // delete tracking area	
	QPushButton *mBtMovePoint;   
	QPushButton *mBtEditTrackNode;
	QPushButton *mBtCopyTrackMark;
	QPushButton *mBtTrackNewVehicle;	
	QComboBox* mCmbBikeType;

	//////////////////////////////////////////////////////////////////////////
	//                           sub windows
	//////////////////////////////////////////////////////////////////////////
	// plot curve window
	PlotWnd *mPlotWnd;		

	// right: window showing matrix
	QDockWidget *mDockWndMat;   // dock window to contain MatWnd
	MatWnd      *mMatWnd;

	// bottom: window for logging, ...
	QDockWidget *mDockWndBottom;      // dock window for BottomWnd 
	BottomWnd   *mBottomWnd;

	// dialog shows working progress
	ProgressDlg *mProgressDlg;        

	//////////////////////////////////////////////////////////////////////////
	// right: 3D reconstruction toolbox
	//
	QToolBar *mToolBox3DRec; 

	// buttons for 3D reconstruction
	QPushButton *mBtSetControlLine;
	QPushButton *mBtSetControlPoint;
	QPushButton *mBtSetEditControlPoint;
	QPushButton *mBtCameraCalibration;	
	QPushButton *mBtCameraSpaceGrid;

	// for tools for all toolbars 		
	QButtonGroup *mBtGroupImgTool;   // button group for all toolbars

	QToolBar *mSwithTool;
	QTabBar  *mMainExchangeTab;			
	QSpinBox  *mSbxCurrentID;        //editing current ID

	// sub window for status bar
	//
	QLabel *mStatusLeftWnd;
	QLabel *mStatusPositionWnd;      // sub text showing position of mouse
	QLabel *mStatusToolNameWnd;      // show tool name

	// menu actions
	QAction *mActionSetting;

	//////////////////////////////////////////////////////////////////////////
	//                         non-GUI data
	//////////////////////////////////////////////////////////////////////////
	// video file data
	QString mCurrentVideoFile;   // video file full path	
	QMap<QString, ToolImage*> mToolMap;  // {'show name': object, }
	QString  mPreToolName;               // name of the previous tool, for deleting
	QPolygon mInterestingArea;           // study area polygon	
	Mat mMask;                           // mask area matrix, interesting hot area

	// project file data
	ProjectFile mPrjFileObj;
};

#endif // TJEMS_H
