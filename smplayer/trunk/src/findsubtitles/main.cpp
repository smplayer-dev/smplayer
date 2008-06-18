
#include <QApplication>
#include "findsubtitlesdialog.h"

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	FindSubtitlesDialog *w = new FindSubtitlesDialog(0);
	w->show();

	return a.exec();
}

