/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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


	prefassociations.cpp
	Handles file associations in Windows
	Author: Florin Braghis (florin@libertv.ro)
*/


#include "prefassociations.h"
#include "images.h"
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include "winfileassoc.h"
#include "extensions.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

static Qt::CheckState CurItemCheckState = Qt::Unchecked; 


PrefAssociations::PrefAssociations(QWidget * parent, Qt::WindowFlags f)
: PrefWidget(parent, f )
{
	setupUi(this);

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(listWidget->viewport());
#endif

	connect(selectAll, SIGNAL(clicked(bool)), this, SLOT(selectAllClicked(bool)));
	connect(selectNone, SIGNAL(clicked(bool)), this, SLOT(selectNoneClicked(bool)));
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*))); 
	connect(listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(listItemPressed(QListWidgetItem*))); 

	if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
	{
		//Hide Select None - One cannot restore an association in Vista. Go figure.
		selectNone->hide(); 
		//QPushButton* lpbButton = new QPushButton("Launch Program Defaults", this); 
		//hboxLayout->addWidget(lpbButton); 
		//connect(lpbButton, SIGNAL(clicked(bool)), this, SLOT(launchAppDefaults()));
	}

	Extensions e;
	for (int n=0; n < e.multimedia().count(); n++) {
		addItem( e.multimedia()[n] );
	}
	// Add the playlist extensions
	for (int n=0; n < e.playlist().count(); n++) {
		addItem( e.playlist()[n] );
	}
	retranslateStrings();

	something_changed = false;
}

PrefAssociations::~PrefAssociations()
{

}

void PrefAssociations::selectAllClicked(bool)
{
	for (int k = 0; k < listWidget->count(); k++)
		listWidget->item(k)->setCheckState(Qt::Checked);
	listWidget->setFocus(); 

	something_changed = true;
}

void PrefAssociations::selectNoneClicked(bool)
{

	for (int k = 0; k < listWidget->count(); k++)
		listWidget->item(k)->setCheckState(Qt::Unchecked);
	listWidget->setFocus(); 

	something_changed = true;
}

void PrefAssociations::listItemClicked(QListWidgetItem* item)
{
	qDebug("PrefAssociations::listItemClicked");
	
	if (!(item->flags() & Qt::ItemIsEnabled))
		return; 

	if (item->checkState() == CurItemCheckState)
	{
		//Clicked on the list item (not checkbox)
		if (item->checkState() == Qt::Checked)
		{
			item->setCheckState(Qt::Unchecked);
		}
		else
			item->setCheckState(Qt::Checked); 
	}

	//else - clicked on the checkbox itself, do nothing
	
	something_changed = true;
}

void PrefAssociations::listItemPressed(QListWidgetItem* item)
{
	CurItemCheckState = item->checkState(); 
}

void PrefAssociations::addItem(QString label)
{
	QListWidgetItem* item = new QListWidgetItem(listWidget); 
	item->setText(label);
}

void PrefAssociations::refreshList()
{
	m_regExtensions.clear(); 
	WinFileAssoc ().GetRegisteredExtensions(Extensions().multimedia(), m_regExtensions); 

	for (int k = 0; k < listWidget->count(); k++)
	{
		QListWidgetItem* pItem = listWidget->item(k); 
		if (pItem)
		{
			pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

			if (m_regExtensions.contains(pItem->text()))
			{
				pItem->setCheckState(Qt::Checked);
				//Don't allow de-selection in windows VISTA if extension is registered.
				//VISTA doesn't seem to support extension 'restoration' in the API.
				if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA) {
					pItem->setFlags(0);
				}
			}
			else
			{
				pItem->setCheckState(Qt::Unchecked);
			}

		}
	}
}

void PrefAssociations::setData(Preferences * )
{
	refreshList(); 
}

int PrefAssociations::ProcessAssociations(QStringList& current, QStringList& old)
{
	WinFileAssoc RegAssoc; 

	QStringList toRestore; 

	//Restore unselected associations
	foreach(const QString& ext, old)
	{
		if (!current.contains(ext))
			toRestore.append(ext); 
	}

	RegAssoc.RestoreFileAssociations(toRestore); 
	return RegAssoc.CreateFileAssociations(current); 
}

void PrefAssociations::getData(Preferences *)
{
	qDebug("PrefAssociations::getData: something_changed: %d", something_changed);
	if (!something_changed) return;
	
	QStringList extensions; 

	for (int k = 0; k < listWidget->count(); k++)
	{
		QListWidgetItem* pItem = listWidget->item(k); 
		if (pItem && pItem->checkState() == Qt::Checked)
			extensions.append(pItem->text()); 
	}

	int processed = ProcessAssociations(extensions, m_regExtensions); 

	if (processed != extensions.count())
	{
		QMessageBox::warning(this, tr("Warning"), 
            tr("Not all files could be associated. Please check your "
               "security permissions and retry."), QMessageBox::Ok);
	}
	
	refreshList(); //Useless when OK is pressed... How to detect if apply or ok is pressed ?

	something_changed = false;
}

QString PrefAssociations::sectionName() {
	return tr("File Types");
}

QPixmap PrefAssociations::sectionIcon() {
	return Images::icon("pref_associations");
}

void PrefAssociations::retranslateStrings() {

	retranslateUi(this);
	createHelp();
}

void PrefAssociations::createHelp() {

	clearHelp();

	setWhatsThis(selectAll, tr("Select all"), 
		tr("Check all file types in the list"));

	setWhatsThis(selectNone, tr("Select none"), 
		tr("Uncheck all file types in the list"));

	setWhatsThis(listWidget, tr("List of file types"), 
		tr("Check the media file extensions you would like SMPlayer to handle. "
		   "When you click Apply, the checked files will be associated with "
		   "SMPlayer. If you uncheck a media type, the file association will "
		   "be restored.") + "<br><b>" + tr("Note:") + "</b> " +
        tr("Restoration doesn't work on Windows Vista."));
}

#include "moc_prefassociations.cpp"

