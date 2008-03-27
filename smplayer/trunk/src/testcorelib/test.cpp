
#include "test.h"
#include "mplayerwindow.h"
#include "smplayercorelib.h"
#include <QAction>
#include <QMenuBar>
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
	connect( playAct, SIGNAL(triggered()), 
             smplayerlib->core(), SLOT(play_or_pause()) );

	QAction * stopAct = new QAction( tr("&Stop"), this);
	connect( stopAct, SIGNAL(triggered()), 
             smplayerlib->core(), SLOT(stop()) );

	QMenu * play_menu = menuBar()->addMenu( tr("&Play") );
	play_menu->addAction(playAct);
	play_menu->addAction(stopAct);
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
