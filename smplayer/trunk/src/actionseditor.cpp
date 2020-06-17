/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

/* This is based on qq14-actioneditor-code.zip from Qt */


#include "actionseditor.h"

#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include <QHeaderView>
#include <QLineEdit>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QRegExp>
#include <QApplication>
#include <QAction>
#include <QDebug>

#include "images.h"
#include "filedialog.h"
#include "paths.h"

#include "shortcutgetter.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

/*
#include <QLineEdit>
#include <QItemDelegate>

class MyDelegate : public QItemDelegate 
{
public:
	MyDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
		virtual void setModelData(QWidget * editor, QAbstractItemModel * model, 
                              const QModelIndex & index ) const;
};

MyDelegate::MyDelegate(QObject *parent) : QItemDelegate(parent)
{
}

static QString old_accel_text;

QWidget * MyDelegate::createEditor(QWidget *parent, 
								   const QStyleOptionViewItem & option,
	                               const QModelIndex & index) const
{
	qDebug("MyDelegate::createEditor");

	old_accel_text = index.model()->data(index, Qt::DisplayRole).toString();
	//qDebug( "text: %s", old_accel_text.toUtf8().data());
	
	return QItemDelegate::createEditor(parent, option, index);
}

void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{
	QLineEdit *line_edit = static_cast<QLineEdit*>(editor);

	QString accelText = QKeySequence(line_edit->text()).toString();
	if (accelText.isEmpty() && !line_edit->text().isEmpty()) {
		model->setData(index, old_accel_text);
	}
	else {
		model->setData(index, accelText);
	}
}
*/


#if USE_MULTIPLE_SHORTCUTS
QString ActionsEditor::shortcutsToString(QList <QKeySequence> shortcuts_list) {
	QString accelText = "";

	for (int n=0; n < shortcuts_list.count(); n++) {
		accelText += shortcuts_list[n].toString(QKeySequence::PortableText);
		if (n < (shortcuts_list.count()-1)) accelText += ", ";
	}

	//qDebug("ActionsEditor::shortcutsToString: accelText: '%s'", accelText.toUtf8().constData());

	return accelText;
}

QList <QKeySequence> ActionsEditor::stringToShortcuts(QString shortcuts) {
	QList <QKeySequence> shortcuts_list;

	QStringList l = shortcuts.split(", ");

	for (int n=0; n < l.count(); n++) {
		//qDebug("%s", l[n].toUtf8().data());
		#if QT_VERSION >= 0x040300
		// Qt 4.3 and 4.4 (at least on linux) seems to have a problem when using Traditional Chinese
		// QKeysequence deletes the arrow key names from the shortcut
		// so this is a work-around.
		QString s = l[n].simplified();
		#else
		QString s = QKeySequence( l[n].simplified() );
		#endif

		//Work-around for Simplified-Chinese
		s.replace( QString::fromUtf8("左"), "Left");
		s.replace( QString::fromUtf8("下"), "Down");
		s.replace( QString::fromUtf8("右"), "Right");
		s.replace( QString::fromUtf8("上"), "Up");

		shortcuts_list.append( s );
		//qDebug("ActionsEditor::stringToShortcuts: shortcut %d: '%s'", n, s.toUtf8().data());
	}

	return shortcuts_list;
}
#endif


#define COL_CONFLICTS 0
#define COL_SHORTCUT 1
#define COL_DESC 2
#define COL_NAME 3

ActionsEditor::ActionsEditor(QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	latest_dir = Paths::shortcutsPath();

	table = new QStandardItemModel(this);
	table->setColumnCount(COL_NAME + 1);

	proxy = new QSortFilterProxyModel(this);
	proxy->setSortRole(Qt::UserRole + 1);
	proxy->setFilterRole(Qt::UserRole + 1);
	proxy->setFilterKeyColumn(-1); // All columns
	proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxy->setSourceModel(table);

	actionsTable = new QTableView(this);
	actionsTable->setModel(proxy);
	actionsTable->setSortingEnabled(true);

	actionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
	actionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	actionsTable->verticalHeader()->hide();

	#if QT_VERSION >= 0x050000
	MyScroller::setScroller(actionsTable->viewport());

	actionsTable->horizontalHeader()->setSectionResizeMode(COL_DESC, QHeaderView::Stretch);
	actionsTable->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);
	actionsTable->horizontalHeader()->setSectionResizeMode(COL_CONFLICTS, QHeaderView::ResizeToContents);
	#else
	actionsTable->horizontalHeader()->setResizeMode(COL_DESC, QHeaderView::Stretch);
	actionsTable->horizontalHeader()->setResizeMode(COL_NAME, QHeaderView::Stretch);
	actionsTable->horizontalHeader()->setResizeMode(COL_CONFLICTS, QHeaderView::ResizeToContents);
	#endif

	actionsTable->setAlternatingRowColors(true);
	//actionsTable->setItemDelegateForColumn( COL_SHORTCUT, new MyDelegate(actionsTable) );

	#if !USE_SHORTCUTGETTER
	connect(actionsTable->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(recordAction(const QModelIndex &)) );
	connect(table, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(validateAction(QStandardItem *)) );
	#else
	connect(actionsTable, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(editShortcut()) );
	#endif

	saveButton = new QPushButton(this);
	loadButton = new QPushButton(this);

	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveActionsTable()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadActionsTable()));

	#if USE_SHORTCUTGETTER
	editButton = new QPushButton(this);
	connect( editButton, SIGNAL(clicked()), this, SLOT(editShortcut()) );
	#endif

	filterEdit = new QLineEdit(this);
	filterEdit->setPlaceholderText(tr("Type to search"));
	connect(filterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(filterEditChanged(const QString &)));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing(8);
	#if USE_SHORTCUTGETTER
	buttonLayout->addWidget(editButton);
	#endif
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(filterEdit);
	buttonLayout->addWidget(loadButton);
	buttonLayout->addWidget(saveButton);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setMargin(8);
	mainLayout->setSpacing(8);
	mainLayout->addWidget(actionsTable);
	mainLayout->addLayout(buttonLayout);

	retranslateStrings();
}

ActionsEditor::~ActionsEditor() {
}

void ActionsEditor::retranslateStrings() {
	table->setHorizontalHeaderLabels( QStringList() << "" <<
		tr("Shortcut") << tr("Description") << tr("Name") );

	saveButton->setText(tr("&Save"));
	saveButton->setIcon(Images::icon("save"));

	loadButton->setText(tr("&Load"));
	loadButton->setIcon(Images::icon("open"));

	#if USE_SHORTCUTGETTER
	editButton->setText(tr("&Change shortcut..."));
	#endif

	//updateView(); // The actions are translated later, so it's useless
}

bool ActionsEditor::isEmpty() {
	return actionsList.isEmpty();
}

void ActionsEditor::clear() {
	actionsList.clear();
}

void ActionsEditor::addActions(QWidget *widget) {
	QAction *action;

	QList<QAction *> actions = widget->findChildren<QAction *>();
	for (int n=0; n < actions.count(); n++) {
		action = static_cast<QAction*> (actions[n]);
		/*
		if (!action->objectName().isEmpty()) {
			qDebug("ActionsEditor::addActions: action # %d: '%s' menu: %d", n, action->objectName().toUtf8().constData(), action->menu()!=0);
		}
		*/
		if (!action->objectName().isEmpty() && !action->inherits("QWidgetAction") && (action->menu()==0) ) {
			actionsList.append(action);
		}
	}

	updateView();
	actionsTable->sortByColumn(COL_CONFLICTS, Qt::AscendingOrder);
	//actionsTable->sortByColumn(COL_DESC, Qt::AscendingOrder);
	//actionsTable->horizontalHeader()->setSortIndicator(COL_CONFLICTS, Qt::AscendingOrder);
}

void ActionsEditor::updateView() {
	table->setRowCount( actionsList.count() );

	QAction *action;
	QString accelText;

	#if !USE_SHORTCUTGETTER
	dont_validate = true;
	#endif
	//actionsTable->setSortingEnabled(false);

	for (int n = 0; n < actionsList.count(); n++) {
		action = static_cast<QAction*> (actionsList[n]);

		#if USE_MULTIPLE_SHORTCUTS
		accelText = shortcutsToString( action->shortcuts() );
		#else
		accelText = action->shortcut().toString();
		#endif

		// Conflict column
		QStandardItem * i_conf = new QStandardItem();

		// Name column
		QStandardItem * i_name = new QStandardItem(action->objectName());

		// Desc column
		QStandardItem * i_desc = new QStandardItem(action->text().replace("&",""));
		i_desc->setIcon( action->icon() );

		// Shortcut column
		QStandardItem * i_shortcut = new QStandardItem(accelText);
		int column_height = i_shortcut->sizeHint().height();
		i_shortcut->setSizeHint(QSize(150, column_height));

		// Set flags
		#if !USE_SHORTCUTGETTER
		i_conf->setFlags(Qt::ItemIsEnabled);
		i_name->setFlags(Qt::ItemIsEnabled);
		i_desc->setFlags(Qt::ItemIsEnabled);
		#else
		i_conf->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		i_name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		i_desc->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		i_shortcut->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		#endif

		// Data for sorting
		i_conf->setData(n);
		i_name->setData(i_name->text());
		i_desc->setData(i_desc->text());
		i_shortcut->setData(i_shortcut->text());

		// Add items to table
		table->setItem(n, COL_CONFLICTS, i_conf );
		table->setItem(n, COL_NAME, i_name );
		table->setItem(n, COL_DESC, i_desc );
		table->setItem(n, COL_SHORTCUT, i_shortcut );

	}
	hasConflicts(); // Check for conflicts

	actionsTable->resizeColumnsToContents();
	actionsTable->selectionModel()->setCurrentIndex(actionsTable->model()->index(0, 0), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

	#if !USE_SHORTCUTGETTER
	dont_validate = false;
	#endif
	//actionsTable->setSortingEnabled(true);
}


void ActionsEditor::applyChanges() {
	qDebug("ActionsEditor::applyChanges");

	for (int row = 0; row < (int)actionsList.size(); ++row) {
		QAction *action = actionsList[row];
		QStandardItem *i = table->item(row, COL_SHORTCUT);

		#if USE_MULTIPLE_SHORTCUTS
		action->setShortcuts( stringToShortcuts(i->text()) );
		#else
		action->setShortcut( QKeySequence(i->text()) );
		#endif
	}
}

#if !USE_SHORTCUTGETTER
void ActionsEditor::recordAction(const QModelIndex & index) {
	//qDebug("ActionsEditor::recordAction");

	int current = index.row();
	QStandardItem * i = table->item(current, COL_SHORTCUT);

	if (i->column() == COL_SHORTCUT) {
		//qDebug("ActionsEditor::recordAction: %d %d %s", i->row(), i->column(), i->text().toUtf8().data());
		oldAccelText = i->text();
	}
}

void ActionsEditor::validateAction(QStandardItem * i) {
	//qDebug("ActionsEditor::validateAction");
	if (dont_validate) return;

	if (i->column() == COL_SHORTCUT) {
		QString accelText = QKeySequence(i->text()).toString();

		if (accelText.isEmpty() && !i->text().isEmpty()) {
			/*
			QAction * action = static_cast<QAction*> (actionsList[i->row()]);
			QString oldAccelText= action->accel().toString();
			*/
			i->setText(oldAccelText);
		}
		else {
			i->setText(accelText);
		}

		if (hasConflicts()) qApp->beep();
	}
}

#else

void ActionsEditor::editShortcut() {
	QModelIndex index = actionsTable->currentIndex();
	QModelIndex s_index = proxy->mapToSource(index);
	int current = s_index.row();
	QStandardItem * i = table->item(current, COL_SHORTCUT);
	if (i) {
		ShortcutGetter d(this);
		QString result = d.exec( i->text() );

		if (!result.isNull()) {
			//qDebug("ActionsEditor::editShortcut: result: '%s'", result.toUtf8().constData());
			QString accelText = QKeySequence(result).toString(QKeySequence::PortableText);
			i->setText(accelText);
			if (hasConflicts()) qApp->beep();
		}
	}
}
#endif

void ActionsEditor::filterEditChanged(const QString & filter) {
	proxy->setFilterWildcard(filter.trimmed());
}

int ActionsEditor::findActionName(const QString & name) {
	for (int row = 0; row < table->rowCount(); row++) {
		if (table->item(row, COL_NAME)->text() == name) return row;
	}
	return -1;
}

bool ActionsEditor::containsShortcut(const QString & accel, const QString & shortcut) {
	QStringList shortcut_list = accel.split(", ");
	QString s;
	foreach(s, shortcut_list) {
		s = s.trimmed();
		//qDebug("ActionsEditor::containsShortcut: comparing '%s' with '%s'", s.toUtf8().constData(), shortcut.toUtf8().constData());
		if (s == shortcut) return true;
	}
	return false;
}

int ActionsEditor::findActionAccel(const QString & accel, int ignoreRow) {
	QStringList shortcuts = accel.split(", ");
	QString shortcut;

	for (int row = 0; row < table->rowCount(); row++) {
		QStandardItem * i = table->item(row, COL_SHORTCUT);
		if (i && row != ignoreRow) {
			if (!i->text().isEmpty()) {
				foreach(shortcut, shortcuts) {
					if (containsShortcut(i->text(), shortcut.trimmed())) {
						return row;
					}
				}
			}
		}
	}
	return -1;
}

bool ActionsEditor::hasConflicts() {
	int found;
	bool conflict = false;

	QString accelText;
	QStandardItem *i;

	for (int n = 0; n < table->rowCount(); n++) {
		//actionsTable->setText( n, COL_CONFLICTS, " ");
		i = table->item( n, COL_CONFLICTS );
		if (i) i->setIcon( QPixmap() );

		i = table->item(n, COL_SHORTCUT );
		if (i) {
			accelText = i->text();
			if (!accelText.isEmpty()) {
				found = findActionAccel( accelText, n );
				if ( (found != -1) /*&& (found != n)*/ ) {
					conflict = true;
					//actionsTable->setText( n, COL_CONFLICTS, "!");
					table->item( n, COL_CONFLICTS )->setIcon( Images::icon("conflict") );
				}
			}
		}
	}
	//if (conflict) qApp->beep();
	return conflict;
}


void ActionsEditor::saveActionsTable() {
	QString s = MyFileDialog::getSaveFileName(
                    this, tr("Choose a filename"), 
                    latest_dir,
                    tr("Key files") +" (*.keys)" );

	if (!s.isEmpty()) {
		// If filename has no extension, add it
		if (QFileInfo(s).suffix().isEmpty()) {
			s = s + ".keys";
		}
		if (QFileInfo(s).exists()) {
			int res = QMessageBox::question( this,
					tr("Confirm overwrite?"),
                    tr("The file %1 already exists.\n"
                       "Do you want to overwrite?").arg(s),
                    QMessageBox::Yes,
                    QMessageBox::No,
                    Qt::NoButton);
			if (res == QMessageBox::No ) {
            	return;
			}
		}
		latest_dir = QFileInfo(s).absolutePath();
		bool r = saveActionsTable(s);
		if (!r) {
			QMessageBox::warning(this, tr("Error"), 
               tr("The file couldn't be saved"), 
               QMessageBox::Ok, Qt::NoButton);
		}
	}
}

bool ActionsEditor::saveActionsTable(const QString & filename) {
	qDebug("ActionsEditor::saveActions: '%s'", filename.toUtf8().data());

	QFile f( filename );
	if ( f.open( QIODevice::WriteOnly ) ) {
		QTextStream stream( &f );
		stream.setCodec("UTF-8");

		for (int row = 0; row < table->rowCount(); row++) {
			stream << table->item(row, COL_NAME)->text() << "\t" 
                   << table->item(row, COL_SHORTCUT)->text() << "\n";
		}
		f.close();
		return true;
	}
	return false;
}

void ActionsEditor::loadActionsTable() {
	QString s = MyFileDialog::getOpenFileName(
                    this, tr("Choose a file"),
                    latest_dir, tr("Key files") +" (*.keys)" );

	if (!s.isEmpty()) {
		latest_dir = QFileInfo(s).absolutePath();
		bool r = loadActionsTable(s);
		if (!r) {
			QMessageBox::warning(this, tr("Error"), 
               tr("The file couldn't be loaded"), 
               QMessageBox::Ok, Qt::NoButton);
		}
	}
}

bool ActionsEditor::loadActionsTable(const QString & filename) {
	qDebug() << "ActionsEditor::loadActions:" <<  filename;

	int row;

	QFile f( filename );
	if ( f.open( QIODevice::ReadOnly ) ) {

		#if !USE_SHORTCUTGETTER
		dont_validate = true;
		#endif

		QTextStream stream( &f );
		stream.setCodec("UTF-8");

		QString line;
		while ( !stream.atEnd() ) {
			line = stream.readLine().trimmed();
			qDebug() << "ActionsEditor::loadActions: line:" << line;
			QString name;
			QString accelText;
			int pos = line.indexOf(QRegExp("\\t|\\s"));
			//qDebug() << "ActionsEditor::loadActions: pos:" << pos;
			if (pos == -1) {
				name = line;
			} else {
				name = line.left(pos);
				accelText = line.mid(pos+1).trimmed();
			}
			qDebug() << "ActionsEditor::loadActions: name:" << name << "accel:" << accelText;
			if (!name.isEmpty()) {
				row = findActionName(name);
				if (row > -1) {
					qDebug() << "ActionsEditor::loadActions: action found!";
					table->item(row, COL_SHORTCUT)->setText(accelText);
				}
			} else {
				qDebug() << "ActionsEditor::loadActions: error in line";
			}
		}
		f.close();
		hasConflicts(); // Check for conflicts

		#if !USE_SHORTCUTGETTER
		dont_validate = false;
		#endif

		return true;
	} else {
		return false;
	}
}


// Static functions

void ActionsEditor::saveToConfig(QObject *o, QSettings *set) {
	qDebug("ActionsEditor::saveToConfig");

	set->beginGroup("actions");

	QAction *action;
	QList<QAction *> actions = o->findChildren<QAction *>();
	for (int n=0; n < actions.count(); n++) {
		action = static_cast<QAction*> (actions[n]);
		if (!action->objectName().isEmpty() && !action->inherits("QWidgetAction")) {
			#if USE_MULTIPLE_SHORTCUTS
			QString accelText = shortcutsToString(action->shortcuts());
			#else
			QString accelText = action->shortcut().toString();
			#endif
			set->setValue(action->objectName(), accelText);
		}
	}

	set->endGroup();
}


void ActionsEditor::loadFromConfig(QObject *o, QSettings *set) {
	qDebug("ActionsEditor::loadFromConfig");

	set->beginGroup("actions");

	QAction *action;
	QString accelText;

	QList<QAction *> actions = o->findChildren<QAction *>();
	for (int n=0; n < actions.count(); n++) {
		action = static_cast<QAction*> (actions[n]);
		if (!action->objectName().isEmpty() && !action->inherits("QWidgetAction")) {
			#if USE_MULTIPLE_SHORTCUTS
			QString current = shortcutsToString(action->shortcuts());
			accelText = set->value(action->objectName(), current).toString();
			action->setShortcuts( stringToShortcuts( accelText ) );
			#else
			accelText = set->value(action->objectName(), action->shortcut().toString()).toString();
			action->setShortcut(QKeySequence(accelText));
			#endif
		}
	}

	set->endGroup();
}

QAction * ActionsEditor::findAction(QObject *o, const QString & name) {
	QAction *action;

	QList<QAction *> actions = o->findChildren<QAction *>();
	for (int n=0; n < actions.count(); n++) {
		action = static_cast<QAction*> (actions[n]);
		/* qDebug("ActionsEditor::findAction: %s", action->objectName().toLatin1().constData()); */
		if (name == action->objectName()) return action;
	}

	return 0;
}

QStringList ActionsEditor::actionsNames(QObject *o) {
	QStringList l;

	QAction *action;

	QList<QAction *> actions = o->findChildren<QAction *>();
	for (int n=0; n < actions.count(); n++) {
		action = static_cast<QAction*> (actions[n]);
		//qDebug("action name: '%s'", action->objectName().toUtf8().data());
		//qDebug("action name: '%s'", action->text().toUtf8().data());
		if (!action->objectName().isEmpty())
			l.append( action->objectName() );
	}

	return l;
}


// Language change stuff
void ActionsEditor::changeEvent(QEvent *e) {
	if (e->type() == QEvent::LanguageChange) {
		retranslateStrings();
	} else {
		QWidget::changeEvent(e);
	}
}

#include "moc_actionseditor.cpp"
