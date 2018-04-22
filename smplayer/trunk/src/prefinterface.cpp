/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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


#include "prefinterface.h"
#include "images.h"
#include "preferences.h"
#include "paths.h"
#include "languages.h"
#include "recents.h"
#include "urlhistory.h"
#include "autohidewidget.h"

#include <QDir>
#include <QStyleFactory>
#include <QFontDialog>
#include <QDebug>

#ifdef HDPI_SUPPORT
#include "hdpisupport.h"

#if QT_VERSION >= 0x050600
#define HDPI_USE_SCALE_FACTOR
#endif
#endif

#define SINGLE_INSTANCE_TAB 2
#define HDPI_TAB 5

PrefInterface::PrefInterface(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);
	/* volume_icon->hide(); */

	// Style combo
#if !STYLE_SWITCHING
	style_label->hide();
	style_combo->hide();
#else
	style_combo->addItem( "Default" );
	style_combo->addItems( QStyleFactory::keys() );
#endif

	// Icon set combo
	iconset_combo->addItem( "H2O", "H2O" );
	iconset_combo->addItem( "Default", "" );

#ifdef SKINS
	n_skins = 0;
#endif

	// User
	QDir icon_dir = Paths::configPath() + "/themes";
	qDebug() << "PrefInterface::PrefInterface: icon_dir:" << icon_dir.absolutePath();
	QStringList iconsets = icon_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int n=0; n < iconsets.count(); n++) {
		#ifdef SKINS
		QString css_file = Paths::configPath() + "/themes/" + iconsets[n] + "/main.css";
		bool is_skin = QFile::exists(css_file);
		//qDebug() << "PrefInterface::PrefInterface: css_file:" << css_file << "is_skin:" << is_skin;
		if (is_skin) {
			skin_combo->addItem( iconsets[n], iconsets[n] );
			n_skins++;
		}
		else
		#endif
		iconset_combo->addItem( iconsets[n], iconsets[n] );
	}

	// Global
	icon_dir = Paths::themesPath();
	qDebug() << "PrefInterface::PrefInterface: icon_dir:" << icon_dir.absolutePath();
	iconsets = icon_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int n=0; n < iconsets.count(); n++) {
		#ifdef SKINS
		QString css_file = Paths::themesPath() + "/" + iconsets[n] + "/main.css";
		bool is_skin = QFile::exists(css_file);
		//qDebug() << "PrefInterface::PrefInterface: css_file:" << css_file << "is_skin:" << is_skin;
		if (is_skin) {
			if (skin_combo->findData( iconsets[n] ) == -1) {
				skin_combo->addItem( iconsets[n], iconsets[n] );
				n_skins++;
			}
		}
		else
		#endif
		if (iconset_combo->findData( iconsets[n] ) == -1) {
			iconset_combo->addItem( iconsets[n], iconsets[n] );
		}
	}

	#ifdef SKINS
	if (skin_combo->itemText(0) == "Black") {
		skin_combo->removeItem(0);
		skin_combo->addItem("Black", "Black");
	}
	#endif

#ifdef SINGLE_INSTANCE
	connect(single_instance_check, SIGNAL(toggled(bool)), 
            this, SLOT(changeInstanceImages()));
#else
	tabWidget->setTabEnabled(SINGLE_INSTANCE_TAB, false);
#endif

#ifdef SKINS
	connect(gui_combo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(GUIChanged(int)));
#endif

	connect(mainwindow_resize_combo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resizeMethodChanged(int)));

#ifndef SEEKBAR_RESOLUTION
	seeking_method_group->hide();
#endif

#ifndef SKINS
	skin_combo->hide();
	skin_label->hide();
	skin_sp->hide();
#endif

#ifdef HDPI_SUPPORT
	scale_group->setEnabled(false);
	connect(hdpi_scale_slider, SIGNAL(valueChanged(int)), this, SLOT(updateHDPIScaleNumber(int)));
	#ifndef HDPI_STORE_DATA
	tabWidget->setTabEnabled(HDPI_TAB, false);
	#endif
#else
	tabWidget->setTabEnabled(HDPI_TAB, false);
#endif

	retranslateStrings();
}

PrefInterface::~PrefInterface()
{
}

QString PrefInterface::sectionName() {
	return tr("Interface");
}

QPixmap PrefInterface::sectionIcon() {
    return Images::icon("pref_gui");
}

void PrefInterface::createLanguageCombo() {
	QMap <QString,QString> m = Languages::translations();

	// Language combo
	QDir translation_dir = Paths::translationPath();
	QStringList languages = translation_dir.entryList( QStringList() << "*.qm");
	QRegExp rx_lang("smplayer_(.*)\\.qm");
	language_combo->clear();
	language_combo->addItem("<" + tr("System language") + ">");
	for (int n=0; n < languages.count(); n++) {
		if (rx_lang.indexIn(languages[n]) > -1) {
			QString l = rx_lang.cap(1);
			QString text = l;
			if (m.contains(l)) text = m[l] + " ("+l+")";
			language_combo->addItem( text, l );
		}
	}
}

void PrefInterface::retranslateStrings() {
	int mainwindow_resize = mainwindow_resize_combo->currentIndex();
	int timeslider_pos = timeslider_behaviour_combo->currentIndex();

	retranslateUi(this);

	mainwindow_resize_combo->setCurrentIndex(mainwindow_resize);
	timeslider_behaviour_combo->setCurrentIndex(timeslider_pos);

	// Icons
	/* resize_window_icon->setPixmap( Images::icon("resize_window") ); */
	/* volume_icon->setPixmap( Images::icon("speaker") ); */

#ifdef SINGLE_INSTANCE
	changeInstanceImages();
#endif

	// Seek widgets
	seek1->setLabel( tr("&Short jump") );
	seek2->setLabel( tr("&Medium jump") );
	seek3->setLabel( tr("&Long jump") );
	seek4->setLabel( tr("Mouse &wheel jump") );

	if (qApp->isLeftToRight()) {
		seek1->setIcon( Images::icon("forward10s", 32) );
		seek2->setIcon( Images::icon("forward1m", 32) );
		seek3->setIcon( Images::icon("forward10m", 32) );
	} else {
		seek1->setIcon( Images::flippedIcon("forward10s", 32) );
		seek2->setIcon( Images::flippedIcon("forward1m", 32) );
		seek3->setIcon( Images::flippedIcon("forward10m", 32) );
	}
	seek4->setIcon( Images::icon("mouse",32) );

	// Language combo
	int language_item = language_combo->currentIndex();
	createLanguageCombo();
	language_combo->setCurrentIndex( language_item );

	// Iconset combo
	{
		int pos = iconset_combo->findData("");
		if (pos != -1) iconset_combo->setItemText(pos, tr("Classic"));
		pos = iconset_combo->findData("H2O");
		if (pos != -1) iconset_combo->setItemText(pos, tr("Default"));
	}

#if STYLE_SWITCHING
	style_combo->setItemText( 0, tr("Default") );
#endif

	int gui_index = gui_combo->currentIndex();
	gui_combo->clear();
#ifdef DEFAULTGUI
	gui_combo->addItem( tr("Basic GUI"), "DefaultGUI");
#endif
#ifdef MINIGUI
	gui_combo->addItem( tr("Mini GUI"), "MiniGUI");
#endif
#ifdef MPCGUI
	gui_combo->addItem( tr("Mpc GUI"), "MpcGUI");
#endif
#ifdef SKINS
	gui_combo->addItem( tr("Skinnable GUI"), "SkinGUI");
	if (n_skins == 0) {
		QModelIndex index = gui_combo->model()->index(gui_combo->count()-1,0);
		gui_combo->model()->setData(index, QVariant(0), Qt::UserRole -1);
	}
#endif
	gui_combo->setCurrentIndex(gui_index);

	floating_width_label->setNum(floating_width_slider->value());
	floating_margin_label->setNum(floating_margin_slider->value());

#ifdef HDPI_SUPPORT
	#ifdef HDPI_USE_SCALE_FACTOR
	hdpi_scale_label->setText(tr("Scale fact&or:"));
	hdpi_scale_num_label->setNum((double) hdpi_scale_slider->value() / 10);
	#else
	hdpi_scale_label->setText(tr("Pixel rati&o:"));
	hdpi_scale_num_label->setNum(hdpi_scale_slider->value());
	#endif
#endif

	createHelp();
}

void PrefInterface::setData(Preferences * pref) {
	setLanguage( pref->language );
	setIconSet( pref->iconset );

	setResizeMethod( pref->resize_method );
	setSaveSize( pref->save_window_size_on_exit );

	center_window_check->setChecked(pref->center_window);
	center_if_outside_check->setChecked(pref->center_window_if_outside);

	use_native_openfile_check->setChecked(pref->use_native_open_dialog);

#ifdef SINGLE_INSTANCE
	setUseSingleInstance(pref->use_single_instance);
#endif
	setSeeking1(pref->seeking1);
	setSeeking2(pref->seeking2);
	setSeeking3(pref->seeking3);
	setSeeking4(pref->seeking4);

	setUpdateWhileDragging(pref->update_while_seeking);
#ifdef SEEKBAR_RESOLUTION
	setRelativeSeeking(pref->relative_seeking);
#endif
	setPreciseSeeking(pref->precise_seeking);

	reset_stop_check->setChecked(pref->reset_stop);

	setDefaultFont(pref->default_font);

	setHideVideoOnAudioFiles(pref->hide_video_window_on_audio_files);

#if STYLE_SWITCHING
	setStyle( pref->style );
#endif

	setGUI(pref->gui);

	setFloatingAnimated(pref->floating_control_animated);
	setFloatingWidth(pref->floating_control_width);
	setFloatingMargin(pref->floating_control_margin);
	setDisplayFloatingInCompactMode(pref->floating_display_in_compact_mode);
	floating_move_bottom_check->setChecked(pref->floating_activation_area == AutohideWidget::Bottom);
	floating_hide_delay_spin->setValue(pref->floating_hide_delay);

	setRecentsMaxItems(pref->history_recents->maxItems());
	setURLMaxItems(pref->history_urls->maxItems());
	setRememberDirs(pref->save_dirs);

#ifdef HDPI_SUPPORT
	loadHDPIData();
#endif
}

void PrefInterface::getData(Preferences * pref) {
	requires_restart = false;
	language_changed = false;
	iconset_changed = false;
	gui_changed = false;
	style_changed = false;
	font_changed = false;
	recents_changed = false;

	if (pref->language != language()) {
		pref->language = language();
		language_changed = true;
		qDebug("PrefInterface::getData: chosen language: '%s'", pref->language.toUtf8().data());
	}

	if (pref->iconset != iconSet()) {
		pref->iconset = iconSet();
		iconset_changed = true;
	}

	if (pref->gui != GUI()) {
		pref->gui = GUI();
		gui_changed = true;
	}

	pref->resize_method = resizeMethod();
	pref->save_window_size_on_exit = saveSize();

	pref->center_window = center_window_check->isChecked();
	pref->center_window_if_outside = center_if_outside_check->isChecked();

#ifdef SINGLE_INSTANCE
	pref->use_single_instance = useSingleInstance();
#endif

	pref->use_native_open_dialog = use_native_openfile_check->isChecked();

	pref->seeking1 = seeking1();
	pref->seeking2 = seeking2();
	pref->seeking3 = seeking3();
	pref->seeking4 = seeking4();

	pref->update_while_seeking = updateWhileDragging();
#ifdef SEEKBAR_RESOLUTION
	pref->relative_seeking= relativeSeeking();
#endif
	pref->precise_seeking = preciseSeeking();

	pref->reset_stop = reset_stop_check->isChecked();

	if (pref->default_font != defaultFont()) {
		pref->default_font = defaultFont();
		font_changed = true;
	}

	pref->hide_video_window_on_audio_files = hideVideoOnAudioFiles();

#if STYLE_SWITCHING
	if ( pref->style != style() ) {
		pref->style = style();
		style_changed = true;
	}
#endif

	pref->floating_control_animated = floatingAnimated();
	pref->floating_control_width = floatingWidth();
	pref->floating_control_margin = floatingMargin();
	pref->floating_display_in_compact_mode = displayFloatingInCompactMode();
	pref->floating_activation_area = floating_move_bottom_check->isChecked() ? AutohideWidget::Bottom : AutohideWidget::Anywhere;
	pref->floating_hide_delay = floating_hide_delay_spin->value();

	if (pref->history_recents->maxItems() != recentsMaxItems()) {
		pref->history_recents->setMaxItems( recentsMaxItems() );
		recents_changed = true;
	}

	if (pref->history_urls->maxItems() != urlMaxItems()) {
		pref->history_urls->setMaxItems( urlMaxItems() );
		url_max_changed = true;
	}

	pref->save_dirs = rememberDirs();

#ifdef HDPI_SUPPORT
	saveHDPIData();
#endif
}

void PrefInterface::setLanguage(QString lang) {
	if (lang.isEmpty()) {
		language_combo->setCurrentIndex(0);
	}
	else {
		int pos = language_combo->findData(lang);
		if (pos != -1) 
			language_combo->setCurrentIndex( pos );
		else
			language_combo->setCurrentText(lang);
	}
}

QString PrefInterface::language() {
	if (language_combo->currentIndex()==0) 
		return "";
	else 
		return language_combo->itemData( language_combo->currentIndex() ).toString();
}

void PrefInterface::setIconSet(QString set) {
	iconset_combo->setCurrentIndex(0);
	int pos = iconset_combo->findData(set);
	if (pos != -1) iconset_combo->setCurrentIndex(pos);

#ifdef SKINS
	skin_combo->setCurrentIndex(0);
	pos = skin_combo->findData(set);
	if (pos != -1) skin_combo->setCurrentIndex(pos);
#endif
}

QString PrefInterface::iconSet() {
#ifdef SKINS
	QString GUI = gui_combo->itemData(gui_combo->currentIndex()).toString();
	if (GUI == "SkinGUI") {
		return skin_combo->itemData(skin_combo->currentIndex()).toString();
	}
	else
#endif
	return iconset_combo->itemData(iconset_combo->currentIndex()).toString();
}

void PrefInterface::setResizeMethod(int v) {
	mainwindow_resize_combo->setCurrentIndex(v);
}

int PrefInterface::resizeMethod() {
	return mainwindow_resize_combo->currentIndex();
}

void PrefInterface::setSaveSize(bool b) {
	save_size_check->setChecked(b);
}

bool PrefInterface::saveSize() {
	return save_size_check->isChecked();
}


void PrefInterface::setStyle(QString style) {
	if (style.isEmpty()) {
		style_combo->setCurrentIndex(0);
	} else {
		int i = style_combo->findText(style, Qt::MatchFixedString);
		if (i < 0) i = 0;
		style_combo->setCurrentIndex(i);
	}
}

QString PrefInterface::style() {
	if (style_combo->currentIndex() == 0)
		return "";
	else
		return style_combo->currentText();
}

void PrefInterface::setGUI(QString gui_name) {
#ifdef SKINS
	if ((n_skins == 0) && (gui_name == "SkinGUI")) gui_name = "DefaultGUI";
#endif
	int i = gui_combo->findData(gui_name);
	if (i < 0) i=0;
	gui_combo->setCurrentIndex(i);
}

QString PrefInterface::GUI() {
	return gui_combo->itemData(gui_combo->currentIndex()).toString();
}

#ifdef SKINS
void PrefInterface::GUIChanged(int index) {
	if (gui_combo->itemData(index).toString() == "SkinGUI") {
		iconset_combo->hide();
		iconset_label->hide();
		iconset_sp->hide();
		skin_combo->show();
		skin_label->show();
		skin_sp->show();
	} else {
		iconset_combo->show();
		iconset_label->show();
		iconset_sp->show();
		skin_combo->hide();
		skin_label->hide();
		skin_sp->hide();
	}
}
#endif

void PrefInterface::resizeMethodChanged(int index) {
	center_if_outside_check->setEnabled(index != 0);
}

#ifdef SINGLE_INSTANCE
void PrefInterface::setUseSingleInstance(bool b) {
	single_instance_check->setChecked(b);
	//singleInstanceButtonToggled(b);
}

bool PrefInterface::useSingleInstance() {
	return single_instance_check->isChecked();
}
#endif

void PrefInterface::setSeeking1(int n) {
	seek1->setTime(n);
}

int PrefInterface::seeking1() {
	return seek1->time();
}

void PrefInterface::setSeeking2(int n) {
	seek2->setTime(n);
}

int PrefInterface::seeking2() {
	return seek2->time();
}

void PrefInterface::setSeeking3(int n) {
	seek3->setTime(n);
}

int PrefInterface::seeking3() {
	return seek3->time();
}

void PrefInterface::setSeeking4(int n) {
	seek4->setTime(n);
}

int PrefInterface::seeking4() {
	return seek4->time();
}

void PrefInterface::setUpdateWhileDragging(bool b) {
	if (b) 
		timeslider_behaviour_combo->setCurrentIndex(0);
	else
		timeslider_behaviour_combo->setCurrentIndex(1);
}

bool PrefInterface::updateWhileDragging() {
	return (timeslider_behaviour_combo->currentIndex() == 0);
}

#ifdef SEEKBAR_RESOLUTION
void PrefInterface::setRelativeSeeking(bool b) {
	relative_seeking_button->setChecked(b);
	absolute_seeking_button->setChecked(!b);
}

bool PrefInterface::relativeSeeking() {
	return relative_seeking_button->isChecked();
}
#endif

void PrefInterface::setPreciseSeeking(bool b) {
	precise_seeking_check->setChecked(b);
}

bool PrefInterface::preciseSeeking() {
	return precise_seeking_check->isChecked();
}

void PrefInterface::setDefaultFont(QString font_desc) {
	default_font_edit->setText(font_desc);
}

QString PrefInterface::defaultFont() {
	return default_font_edit->text();
}

void PrefInterface::on_changeFontButton_clicked() {
	QFont f = qApp->font();

	if (!default_font_edit->text().isEmpty()) {
		f.fromString(default_font_edit->text());
	}

	bool ok;
	f = QFontDialog::getFont( &ok, f, this);

	if (ok) {
		default_font_edit->setText( f.toString() );
	}
}

#ifdef SINGLE_INSTANCE
void PrefInterface::changeInstanceImages() {
	if (single_instance_check->isChecked())
		instances_icon->setPixmap( Images::icon("instance1") );
	else
		instances_icon->setPixmap( Images::icon("instance2") );
}
#endif

void PrefInterface::setHideVideoOnAudioFiles(bool b) {
	hide_video_window_on_audio_check->setChecked(b);
}

bool PrefInterface::hideVideoOnAudioFiles() {
	return hide_video_window_on_audio_check->isChecked();
}

// Floating tab
void PrefInterface::setFloatingAnimated(bool b) {
	floating_animated_check->setChecked(b);
}

bool PrefInterface::floatingAnimated() {
	return floating_animated_check->isChecked();
}

void PrefInterface::setFloatingWidth(int percentage) {
	floating_width_slider->setValue(percentage);
}

int PrefInterface::floatingWidth() {
	return floating_width_slider->value();
}

void PrefInterface::setFloatingMargin(int pixels) {
	floating_margin_slider->setValue(pixels);
}

int PrefInterface::floatingMargin() {
	return floating_margin_slider->value();
}

void PrefInterface::setDisplayFloatingInCompactMode(bool b) {
	floating_compact_check->setChecked(b);
}

bool PrefInterface::displayFloatingInCompactMode() {
	return floating_compact_check->isChecked();
}

void PrefInterface::setRecentsMaxItems(int n) {
	recents_max_items_spin->setValue(n);
}

int PrefInterface::recentsMaxItems() {
	return recents_max_items_spin->value();
}

void PrefInterface::setURLMaxItems(int n) {
	url_max_items_spin->setValue(n);
}

int PrefInterface::urlMaxItems() {
	return url_max_items_spin->value();
}

void PrefInterface::setRememberDirs(bool b) {
	save_dirs_check->setChecked(b);
}

bool PrefInterface::rememberDirs() {
	return save_dirs_check->isChecked();
}

#ifdef HDPI_SUPPORT
void PrefInterface::loadHDPIData() {
	HDPISupport * hdpi = HDPISupport::instance();
	enable_hdpi_check->setChecked(hdpi->isHDPIEnabled());
	auto_scale_check->setChecked(hdpi->autoScale());

	#ifdef HDPI_USE_SCALE_FACTOR
	hdpi_scale_slider->setMinimum(5);
	hdpi_scale_slider->setMaximum(4*10);
	hdpi_scale_slider->setValue(hdpi->scaleFactor() * 10);
	#else
	hdpi_scale_slider->setMinimum(0);
	hdpi_scale_slider->setMaximum(4);
	hdpi_scale_slider->setValue(hdpi->pixelRatio());
	#endif
}

void PrefInterface::updateHDPIScaleNumber(int v) {
	#ifdef HDPI_USE_SCALE_FACTOR
	hdpi_scale_num_label->setNum((double) v / 10);
	#else
	hdpi_scale_num_label->setNum(v);
	#endif
}

void PrefInterface::saveHDPIData() {
	HDPISupport * hdpi = HDPISupport::instance();
	hdpi->setHDPIEnabled(enable_hdpi_check->isChecked());
	hdpi->setAutoScale(auto_scale_check->isChecked());
	#ifdef HDPI_USE_SCALE_FACTOR
	hdpi->setScaleFactor((double) hdpi_scale_slider->value() / 10);
	#else
	hdpi->setPixelRatio(hdpi_scale_slider->value());
	#endif
	#ifdef HDPI_STORE_DATA
	hdpi->save();
	#endif
}
#endif

void PrefInterface::createHelp() {
	clearHelp();

	addSectionTitle(tr("Interface"));

	setWhatsThis(mainwindow_resize_combo, tr("Autoresize"),
        tr("The main window can be resized automatically. Select the option "
           "you prefer.") );

	setWhatsThis(center_if_outside_check, tr("Prevent window to get outside of screen"),
		tr("If after an autoresize the main window gets outside of the screen this option "
           "will center the window to prevent it.") );

	setWhatsThis(center_window_check, tr("Center window"),
        tr("When this option is enabled, the main window will be centered on the desktop.") );

	setWhatsThis(save_size_check, tr("Remember position and size"),
        tr("If you check this option, the position and size of the main "
           "window will be saved and restored when you run SMPlayer again.") );

	setWhatsThis(hide_video_window_on_audio_check, tr("Hide video window when playing audio files"),
        tr("If this option is enabled the video window will be hidden when playing audio files.") );

	setWhatsThis(language_combo, tr("Language"),
		tr("Here you can change the language of the application.") );

	setWhatsThis(gui_combo, tr("GUI"),
        tr("Select the graphic interface you prefer for the application.") +"<br>"+
        tr("The <b>Basic GUI</b> provides the traditional interface, with the "
           "toolbar and control bar.") +" "+ 
        tr("The <b>Mini GUI</b> provides a more simple interface, without toolbar and a control bar with few "
           "buttons.")
#ifdef MPCGUI
        +" "+
        tr("The <b>Mpc GUI</b> looks like the interface in Media Player Classic.")
#endif
#ifdef SKINS
        +" "+
        tr("The <b>Skinnable GUI</b> provides an interface where several skins are available.")
#endif

        );

	setWhatsThis(iconset_combo, tr("Icon set"),
        tr("Select the icon set you prefer for the application.") );

#ifdef SKINS
	setWhatsThis(skin_combo, tr("Skin"),
        tr("Select the skin you prefer for the application. Only available with the skinnable GUI.") );
#endif

	setWhatsThis(style_combo, tr("Style"),
        tr("Select the style you prefer for the application.") );


	setWhatsThis(changeFontButton, tr("Default font"),
        tr("You can change here the application's font.") );

	setWhatsThis(use_native_openfile_check, tr("Use the system native file dialog"),
        tr("When this option is enabled, SMPlayer will try to use the system native file dialog. Otherwise it will use the internal one.") );

	addSectionTitle(tr("Seeking"));

	setWhatsThis(seek1, tr("Short jump"),
        tr("Select the time that should be go forward or backward when you "
           "choose the %1 action.").arg(tr("short jump")) );

	setWhatsThis(seek2, tr("Medium jump"),
        tr("Select the time that should be go forward or backward when you "
           "choose the %1 action.").arg(tr("medium jump")) );

	setWhatsThis(seek3, tr("Long jump"),
        tr("Select the time that should be go forward or backward when you "
           "choose the %1 action.").arg(tr("long jump")) );

	setWhatsThis(seek4, tr("Mouse wheel jump"),
        tr("Select the time that should be go forward or backward when you "
           "move the mouse wheel.") );

	setWhatsThis(timeslider_behaviour_combo, tr("Behaviour of time slider"),
        tr("Select what to do when dragging the time slider.") );

#ifdef SEEKBAR_RESOLUTION
	setWhatsThis(seeking_method_group, tr("Seeking method"),
		tr("Sets the method to be used when seeking with the slider. "
           "Absolute seeking may be a little bit more accurate, while "
           "relative seeking may work better with files with a wrong length.") );
#endif

	setWhatsThis(precise_seeking_check, tr("Precise seeking"),
		tr("If this option is enabled, seeks are more accurate but they "
           "can be a little bit slower. May not work with some video formats.") +"<br>"+
		tr("Note: this option only works when using mpv as multimedia engine.") );

	setWhatsThis(reset_stop_check, tr("Pressing the stop button once resets the time position"),
		tr("By default when the stop button is pressed the time position is remembered "
           "so if you press play button the media will resume at the same point. You need "
           "to press the stop button twice to reset the time position, but if this "
           "option is checked the time position will be set to 0 with only one "
           "press of the stop button.") );

#ifdef SINGLE_INSTANCE
	addSectionTitle(tr("Instances"));

	setWhatsThis(single_instance_check, 
        tr("Use only one running instance of SMPlayer"),
        tr("Check this option if you want to use an already running instance "
           "of SMPlayer when opening other files.") );
#endif

	addSectionTitle(tr("Floating control"));

	setWhatsThis(floating_animated_check, tr("Animated"),
		tr("If this option is enabled, the floating control will appear "
           "with an animation.") );

	setWhatsThis(floating_width_slider, tr("Width"),
		tr("Specifies the width of the control (as a percentage).") );

	setWhatsThis(floating_margin_slider, tr("Margin"),
		tr("This option sets the number of pixels that the floating control "
           "will be away from the bottom of the screen. Useful when the "
           "screen is a TV, as the overscan might prevent the control to be "
           "visible.") );

	setWhatsThis(floating_move_bottom_check, tr("Show only when moving the mouse to the bottom of the screen"),
		tr("If this option is checked, the floating control will only be displayed when the mouse is moved "
           "to the bottom of the screen. Otherwise the control will appear whenever the mouse is moved, no matter "
           "its position.") );

	setWhatsThis(floating_compact_check, tr("Display in compact mode too"),
		tr("If this option is enabled, the floating control will appear "
           "in compact mode too.") +" " +
		tr("This option only works with the basic GUI.") +" "+
		tr("<b>Warning:</b> the floating control has not been "
           "designed for compact mode and it might not work properly.") );

	setWhatsThis(floating_hide_delay_spin, tr("Time to hide the control"),
		tr("Sets the time (in milliseconds) to hide the control after the mouse went away from the control."));

	addSectionTitle(tr("Privacy"));

	setWhatsThis(recents_max_items_spin, tr("Recent files"),
        tr("Select the maximum number of items that will be shown in the "
           "<b>Open->Recent files</b> submenu. If you set it to 0 that "
           "menu won't be shown at all.") );

	setWhatsThis(url_max_items_spin, tr("Max. URLs"),
        tr("Select the maximum number of items that the <b>Open->URL</b> "
           "dialog will remember. Set it to 0 if you don't want any URL "
           "to be stored.") );

	setWhatsThis(save_dirs_check, tr("Remember last directory"),
		tr("If this option is checked, SMPlayer will remember the last folder you use to open a file.") );
}

#include "moc_prefinterface.cpp"
