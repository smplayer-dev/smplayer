#include <QApplication>
#include "videopreviewconfigdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    VideoPreviewConfigDialog dialog;
    dialog.show();
    
    return app.exec();
}
