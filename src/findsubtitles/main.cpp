
#include <QApplication>
#include "findsubtitleswindow.h"

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	FindSubtitlesWindow *w = new FindSubtitlesWindow(0);
	w->show();

	return a.exec();
}

