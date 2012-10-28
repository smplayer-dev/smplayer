/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>
    umplayer, Copyright (C) 2010 Ori Rejwan

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "qpropertysetter.h"
#include "myicon.h"
#include "images.h"
#include "actiontools.h"


#define ICON_ADD(icon, png) { icon.addPixmap(Images::icon(png, 16), QIcon::Normal, QIcon::Off); \
                              icon.addPixmap(Images::icon(png, 16), QIcon::Active, QIcon::Off); \
                              icon.addPixmap(Images::icon(png, 16), QIcon::Selected, QIcon::Off); \
                              icon.addPixmap(Images::icon(png, 16), QIcon::Disabled, QIcon::Off);}


IconSetter* IconSetter::m_instance = 0;

IconSetter::IconSetter(QWidget *parent) :
    QWidget(parent)
{
    logoBgPix = Images::icon("ump-splash-bg");
    logoSplashPix = Images::icon("ump-splash-highlight");
    logoPix = Images::icon("ump-splash-logo");
}

IconSetter* IconSetter::instance()
{
    if(m_instance == 0)
    {
        m_instance = new IconSetter();
    }
    return m_instance;
}

void IconSetter::removeInstance()
{
    if(m_instance)
        delete m_instance;
    m_instance = 0;
}

void IconSetter::setActionIcon(QPixmap pixmap )
{
#if 0
//#define SAVE_ICONS 1
#define SAVE(name) { QPixmap p = pixmap.copy(n*24, 0, 24, 24); \
                     QString s = "/tmp/" name ".png"; \
                     p.save(s); }

    for(int n = 0; n < 10; ++n )
    {
        QIcon icon;
        icon.addPixmap(pixmap.copy(n*24, 0, 24, 24), QIcon::Normal, QIcon::Off);
        icon.addPixmap(pixmap.copy(n*24, 24, 24, 24), QIcon::Active, QIcon::Off);
        icon.addPixmap(pixmap.copy(n*24, 48, 24, 24), QIcon::Selected, QIcon::Off);
        icon.addPixmap(pixmap.copy(n*24, 72, 24, 24), QIcon::Disabled, QIcon::Off);
        QAction * action = 0;
		//ActionTools::findAction("aaa", toolbar_actions);
        switch(n)
        {
        case 0: action = ActionTools::findAction("open_file", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("open")
				#endif
                break;
        case 1: action = ActionTools::findAction("open_directory", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("open_folder")
				#endif
                break;
        case 2: action = ActionTools::findAction("open_dvd", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("dvd")
				#endif
                break;
        case 3: action = ActionTools::findAction("open_url", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("url")
				#endif
                break;
        case 4: action = ActionTools::findAction("screenshot", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("screenshot")
				#endif
                break;
        case 5: action = ActionTools::findAction("show_file_properties", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("info")
				#endif
                break;
        case 6: action = ActionTools::findAction("show_find_sub_dialog", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("download_subs")
				#endif
                break;
        case 7: action = ActionTools::findAction("show_preferences", toolbar_actions);
				if (action) ICON_ADD(icon, "file")
				#if SAVE_ICONS
				SAVE("prefs")
				#endif
                break;
        }
        if (action) action->setIcon(icon);
    }
#endif
}

void IconSetter::buttonIcon(int buttonNo, QPixmap pix )
{
    MyIcon icon;
    int w = pix.width();
    int h = pix.height();
    icon.setPixmap(pix.copy(0, 0, w, h/4 ), MyIcon::Normal, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/4, w, h/4 ), MyIcon::MouseOver, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/2, w, h/4 ), MyIcon::MouseDown, MyIcon::Off);
    icon.setPixmap(pix.copy(0, 3*h/4, w, h/4 ), MyIcon::Disabled, MyIcon::Off);
    MyIcon icon2;
    switch(buttonNo)
    {
    case 1:
        playControl->setBackwardIcon(icon);
        break;
    case 2:
        playControl->setPreviousIcon(icon);break;
    case 3:        
        icon2.setPixmap(pix.copy(0, 0, w/2, h/4 ), MyIcon::Normal, MyIcon::Off);
        icon2.setPixmap(pix.copy(0, h/4, w/2, h/4 ), MyIcon::MouseOver, MyIcon::Off);
        icon2.setPixmap(pix.copy(0, h/2, w/2, h/4 ), MyIcon::MouseDown, MyIcon::Off);
        icon2.setPixmap(pix.copy(0, 3*h/4, w/2, h/4 ), MyIcon::Disabled, MyIcon::Off);

        icon2.setPixmap(pix.copy(w/2, 0, w/2, h/4 ), MyIcon::Normal, MyIcon::On);
        icon2.setPixmap(pix.copy(w/2, h/4, w/2, h/4 ), MyIcon::MouseOver, MyIcon::On);
        icon2.setPixmap(pix.copy(w/2, h/2, w/2, h/4 ), MyIcon::MouseDown, MyIcon::On);
        icon2.setPixmap(pix.copy(w/2, 3*h/4, w/2, h/4 ), MyIcon::Disabled, MyIcon::On);

        playControl->setPlayPauseIcon(icon2);
        break;
    case 4:
        playControl->setStopIcon(icon);break;
    case 5:
        playControl->setRecordIcon(icon);break;
    case 6:
        playControl->setNextIcon(icon);break;
    case 7:
        playControl->setForwardIcon(icon);break;

    }
}


void IconSetter::mediaPanelButtonIcon( int n, QPixmap pix)
{
    if(pix.isNull()) return;
    MyIcon icon;
    int w = pix.width();
    int h = pix.height();
    icon.setPixmap(pix.copy(0, 0, w/2, h/4 ), MyIcon::Normal, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/4, w/2, h/4 ), MyIcon::MouseOver, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/2, w/2, h/4 ), MyIcon::MouseDown, MyIcon::Off);
    icon.setPixmap(pix.copy(0, 3*h/4, w/2, h/4 ), MyIcon::Disabled, MyIcon::Off);

    icon.setPixmap(pix.copy(w/2, 0, w/2, h/4 ), MyIcon::Normal, MyIcon::On);
    icon.setPixmap(pix.copy(w/2, h/4, w/2, h/4 ), MyIcon::MouseOver, MyIcon::On);
    icon.setPixmap(pix.copy(w/2, h/2, w/2, h/4 ), MyIcon::MouseDown, MyIcon::On);
    icon.setPixmap(pix.copy(w/2, 3*h/4, w/2, h/4 ), MyIcon::Disabled, MyIcon::On);

    switch(n)
    {
    case 1:
        mediaPanel->setShuffleIcon(icon);break;
    case 2:
        mediaPanel->setRepeatIcon(icon);break;
    }
}

#include "moc_qpropertysetter.cpp"
