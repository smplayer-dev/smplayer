
#include "test.h"
#include "mplayerwindow.h"
#include "smplayercorelib.h"
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include "timeslider.h"
#include <QFileDialog>

#include <QApplication>

Gui::Gui( QWidget * parent, Qt::WindowFlags flags ) 
	: QMainWindow(parent, flags)
{
	mpw = new MplayerWindow(this);
	setCentralWidget(mpw);

	smplayerlib = new SmplayerCoreLib(mpw, this);

	QAction * openAct = new QAction( tr("&Open..."), this);
	connect( openAct, SIGNAL(triggered()), this, SLOT(open()) );

	QAction * closeAct = new QAction( tr("&Close"), this);
	connect( closeAct, SIGNAL(triggered()), this, SLOT(close()) );

	QMenu * open_menu = menuBar()->addMenu( tr("&Open") );
	open_menu->addAction(openAct);
	open_menu->addAction(closeAct);

	QAction * playAct = new QAction( tr("&Play/Pause"), this);
	playAct->setShortcut( Qt::Key_Space );
	connect( playAct, SIGNAL(triggered()), 
             smplayerlib->core(), SLOT(play_or_pause()) );

	QAction * stopAct = new QAction( tr("&Stop"), this);
	connect( stopAct, SIGNAL(triggered()), 
             smplayerlib->core(), SLOT(stop()) );

	QMenu * play_menu = menuBar()->addMenu( tr("&Play") );
	play_menu->addAction(playAct);
	play_menu->addAction(stopAct);


	TimeSlider * time_slider = new TimeSlider(this);
	//time_slider->setOrientation(Qt::Horizontal);
	//time_slider->setTracking(false);
	//time_slider->setMinimum(0);
	//time_slider->setMaximum(100);
	connect( time_slider, SIGNAL(posChanged(int)), 
             smplayerlib->core(), SLOT(goToPos(int)) );
	connect( smplayerlib->core(), SIGNAL(posChanged(int)),
             time_slider, SLOT(setPos(int)) );

	QToolBar * control = new QToolBar( tr("Control"), this);
	control->addAction(playAct);
	control->addAction(stopAct);
	control->addSeparator();
	control->addWidget(time_slider);

	addToolBar(Qt::BottomToolBarArea, control);
}

Gui::~Gui() {
}

void Gui::open() {
	QString f = QFileDialog::getOpenFileName( this, tr("Open file") );

	if (!f.isEmpty()) {
		smplayerlib->core()->open(f);
	}
}

int main( int argc, char ** argv ) {
	QApplication a( argc, argv );
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	Gui * w = new Gui();
	w->show();

	return a.exec();
}

#include "moc_test.cpp"
