/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef BASICSMODE_H
#define BASICSMODE_H

#include <QWidget>

#include <QBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;
class DLFLWindow;

class QComboBox;
class QStackedWidget;
class QGroupBox;
class QVBoxLayout;
class QCheckBox;
class QLabel;

// class MainWindow;

class BasicsMode : public QWidget {
	Q_OBJECT

public:
	BasicsMode(QWidget *parent = 0);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);

	QAction *mInsertEdgeAction;
	QAction *mDeleteEdgeAction;	
	QAction *mCollapseEdgeAction;	
	QAction *mSubdivideEdgeAction;	
	QAction *mConnectEdgesAction;	
	QAction *mSpliceCornersAction;
	QAction *mTransformsAction;
	
	QWidget *mInsertEdgeWidget;
	QWidget *mDeleteEdgeWidget;
	QWidget *mCollapseEdgeWidget;
	QWidget *mSubdivideEdgeWidget; 
	QWidget *mConnectEdgesWidget;
	QWidget *mSpliceCornersWidget; 
	QWidget *mTransformsWidget;	
	
	QBoxLayout *mInsertEdgeLayout;
	QBoxLayout *mDeleteEdgeLayout;
	QBoxLayout *mCollapseEdgeLayout;
	QBoxLayout *mSubdivideEdgeLayout; 
	QBoxLayout *mConnectEdgesLayout;
	QBoxLayout *mSpliceCornersLayout; 
	QBoxLayout *mTransformsLayout;

protected:
	void setupInsertEdge();
	void setupDeleteEdge();
	void setupCollapseEdge();
	void setupSubdivideEdge();
	void setupConnectEdges();
	void setupSpliceCorners();
	void setupTransforms();

private:
	
	//transform spinboxes
	QDoubleSpinBox *xScaleSpinBox;
	QDoubleSpinBox *yScaleSpinBox;
	QDoubleSpinBox *zScaleSpinBox;
	QDoubleSpinBox *xPosSpinBox;
	QDoubleSpinBox *yPosSpinBox;
	QDoubleSpinBox *zPosSpinBox;
	
public slots:
	void freeze_transforms();
		
	void triggerInsertEdge();
	void triggerDeleteEdge();
	void triggerCollapseEdge();
	void triggerSubdivideEdge();
	void triggerConnectEdges();
	void triggerSpliceCorners();
	void triggerTransforms();
};

#endif