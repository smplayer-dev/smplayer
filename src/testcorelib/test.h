
#ifndef _TEST_H_
#define _TEST_H_

#include <QMainWindow>

class MplayerWindow;
class SmplayerCoreLib;

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    Gui( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~Gui();

public slots:
	void open();

private:
	MplayerWindow * mpw;
	SmplayerCoreLib * smplayerlib;
};

#endif

