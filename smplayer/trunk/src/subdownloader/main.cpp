
#include <QApplication>
#include "subdownloaderdialog.h"

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	SubDownloaderDialog *w = new SubDownloaderDialog(0);
	w->show();

	return a.exec();
}

