/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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


#include "prefsubtitles.h"
#include "images.h"
#include "global.h"
#include "preferences.h"
#include "helper.h"
#include "filedialog.h"
#include "languages.h"

#include <QColorDialog>
#include <QSettings>
#include <QTextStream>

PrefSubtitles::PrefSubtitles(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	ttf_font_edit->setDialogType(FileChooser::GetFileName);
#ifdef Q_OS_WIN
	ttf_font_edit->setOptions(QFileDialog::DontUseNativeDialog);
#endif

	if (!Global::pref->show_font_scale_options_in_preferences) {
		scale_label->hide();
		ass_scale_label->hide();
		font_text_scale->hide();
		ass_font_scale->hide();
	}

#if USE_ASS_STYLES
	connect( style_border_style_combo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(checkBorderStyleCombo(int)) );

	simple_styles_container->hide();
#else
	styles_container->hide();
#endif

	retranslateStrings();
}

PrefSubtitles::~PrefSubtitles()
{
}

QString PrefSubtitles::sectionName() {
	return tr("Subtitles");
}

QPixmap PrefSubtitles::sectionIcon() {
    return Images::icon("pref_subtitles");
}


void PrefSubtitles::retranslateStrings() {
	int font_autoscale_item = font_autoscale_combo->currentIndex();
	int font_autoload_item = font_autoload_combo->currentIndex();

	retranslateUi(this);

	font_autoscale_combo->setCurrentIndex(font_autoscale_item);
	font_autoload_combo->setCurrentIndex(font_autoload_item);

	// Encodings combo
	//int font_encoding_item = font_encoding_combo->currentIndex();
	QString current_encoding = fontEncoding();
	QString current_enca_lang = encaLang();
	font_encoding_combo->clear();
	enca_lang_combo->clear();

	QMap<QString,QString> l = Languages::encodings();
	QMapIterator<QString, QString> i(l);
	while (i.hasNext()) {
		i.next();
		font_encoding_combo->addItem( i.value() + " (" + i.key() + ")", i.key() );
	}
	l = Languages::list(); i = l;
	while (i.hasNext()) {
		i.next();
		enca_lang_combo->addItem( i.value() + " (" + i.key() + ")", i.key() );
	}
	font_encoding_combo->model()->sort(0);
	enca_lang_combo->model()->sort(0);
	//font_encoding_combo->setCurrentIndex(font_encoding_item);
	setFontEncoding(current_encoding);
	setEncaLang(current_enca_lang);

	sub_pos_label->setNum( sub_pos_slider->value() );

	ttf_font_edit->setCaption(tr("Choose a ttf file"));
	ttf_font_edit->setFilter(tr("Truetype Fonts") + " (*.ttf)");

#if USE_ASS_STYLES
	// Ass styles
	int alignment_item = style_alignment_combo->currentIndex();
	style_alignment_combo->clear();
	style_alignment_combo->addItem(tr("Left"), 1);
	style_alignment_combo->addItem(tr("Centered"), 2);
	style_alignment_combo->addItem(tr("Right"), 3);
	style_alignment_combo->setCurrentIndex(alignment_item);

	int borderstyle_item = style_border_style_combo->currentIndex();
	style_border_style_combo->clear();
	style_border_style_combo->addItem(tr("Outline"), 1);
	style_border_style_combo->addItem(tr("Opaque box"), 3);
	style_border_style_combo->setCurrentIndex(borderstyle_item);
#endif

	createHelp();
}

void PrefSubtitles::setData(Preferences * pref) {
	setFontName( pref->font_name );
	setFontFile( pref->font_file );
	setUseFontconfig( pref->use_fontconfig );
	setFontAutoscale( pref->font_autoscale );
	setFontTextscale( pref->initial_sub_scale );
	setAssFontScale( pref->initial_sub_scale_ass );
	setAutoloadSub( pref->autoload_sub );
	setFontFuzziness( pref->subfuzziness );
	setFontEncoding( pref->subcp );
	setUseEnca( pref->use_enca );
	setEncaLang( pref->enca_lang );
	setUseFontASS( pref->use_ass_subtitles );
#if !USE_ASS_STYLES
	setAssColor( pref->ass_color );
	setAssBorderColor( pref->ass_border_color );
	setAssStyles( pref->ass_styles );
#endif
	setSubPos( pref->initial_sub_pos );
	setSubtitlesOnScreenshots( pref->subtitles_on_screenshots );

#if USE_ASS_STYLES
	// Load ass styles
	style_font_combo->setCurrentText(pref->style_fontname);
	style_size_spin->setValue(pref->style_fontsize);
	style_text_color_button->setColor(pref->style_primarycolor);
	style_border_color_button->setColor(pref->style_backcolor);
	style_bold_check->setChecked(pref->style_bold);
	style_italic_check->setChecked(pref->style_italic);
	style_alignment_combo->setCurrentIndex(style_alignment_combo->findData(pref->style_alignment));
	style_border_style_combo->setCurrentIndex(style_border_style_combo->findData(pref->style_borderstyle));
	style_outline_spin->setValue(pref->style_outline);
	style_shadow_spin->setValue(pref->style_shadow);
#endif
}

void PrefSubtitles::getData(Preferences * pref) {
	requires_restart = false;

	TEST_AND_SET(pref->font_name, fontName());
	TEST_AND_SET(pref->font_file, fontFile());
	TEST_AND_SET(pref->use_fontconfig, useFontconfig());
	TEST_AND_SET(pref->font_autoscale, fontAutoscale());
	pref->initial_sub_scale = fontTextscale();
	pref->initial_sub_scale_ass = assFontScale();
	TEST_AND_SET(pref->autoload_sub, autoloadSub());
	TEST_AND_SET(pref->subfuzziness, fontFuzziness());
	TEST_AND_SET(pref->subcp, fontEncoding());
	TEST_AND_SET(pref->use_enca, useEnca());
	TEST_AND_SET(pref->enca_lang, encaLang());
	TEST_AND_SET(pref->use_ass_subtitles, useFontASS());
#if !USE_ASS_STYLES
	TEST_AND_SET(pref->ass_color, assColor());
	TEST_AND_SET(pref->ass_border_color, assBorderColor());
	TEST_AND_SET(pref->ass_styles, assStyles());
#endif
	pref->initial_sub_pos = subPos();
	TEST_AND_SET(pref->subtitles_on_screenshots, subtitlesOnScreenshots());

#if USE_ASS_STYLES
	// Save ass styles
	TEST_AND_SET(pref->style_fontname, style_font_combo->currentText());
	TEST_AND_SET(pref->style_fontsize, style_size_spin->value());
	TEST_AND_SET(pref->style_primarycolor, style_text_color_button->color().rgb());
	TEST_AND_SET(pref->style_backcolor, style_border_color_button->color().rgb());
	TEST_AND_SET(pref->style_bold, style_bold_check->isChecked());
	TEST_AND_SET(pref->style_italic, style_italic_check->isChecked());
	TEST_AND_SET(pref->style_alignment, style_alignment_combo->itemData(style_alignment_combo->currentIndex()).toInt());
	TEST_AND_SET(pref->style_borderstyle, style_border_style_combo->itemData(style_border_style_combo->currentIndex()).toInt());
	TEST_AND_SET(pref->style_outline, style_outline_spin->value());
	TEST_AND_SET(pref->style_shadow, style_shadow_spin->value());

	exportStyles( Helper::subtitleStyleFile() );
#endif
}

#if USE_ASS_STYLES
bool PrefSubtitles::exportStyles(const QString & filename) {
	QFile f(filename);
	if (f.open(QFile::WriteOnly)) {
		QTextStream out(&f);
		out << "[Script Info]" << endl;
		out << "ScriptType: v4.00+" << endl;
		out << "Collisions: Normal" << endl;
		out << endl;
		out << "[V4+ Styles]" << endl;
		out << "Format: Name, Fontname, Fontsize, PrimaryColour, BackColour, Bold, Italic, Alignment, BorderStyle, Outline, Shadow, MarginV, MarginL, MarginR" << endl;
		out << "Style: Default,";
		out << style_font_combo->currentText() << "," ;
		out << style_size_spin->value() << "," ;
		out << "&H" << Helper::colorToAABBGGRR(style_text_color_button->color().rgb()) << "," ;
		out << "&H" << Helper::colorToAABBGGRR(style_border_color_button->color().rgb()) << "," ;
		out << (style_bold_check->isChecked() ? -1 : 0) << "," ;
		out << (style_italic_check->isChecked() ? -1 : 0) << "," ;
		out << style_alignment_combo->itemData(style_alignment_combo->currentIndex()).toInt() << "," ;
		out << style_border_style_combo->itemData(style_border_style_combo->currentIndex()).toInt() << "," ;
		out << style_outline_spin->value() << "," ;
		out << style_shadow_spin->value() << "," ;
		out << "10, 20, 20" ;
		out << endl;

		f.close();
		return true;
	}
	return false;
}

void PrefSubtitles::checkBorderStyleCombo( int index ) {
	bool b = (index == 0);
	style_outline_spin->setEnabled(b);
	style_shadow_spin->setEnabled(b);
	style_outline_label->setEnabled(b);
	style_shadow_label->setEnabled(b);
}
#endif

void PrefSubtitles::setFontName(QString font_name) {
	fontCombo->setCurrentText(font_name);
}

QString PrefSubtitles::fontName() {
	return fontCombo->currentText();
}

void PrefSubtitles::setFontFile(QString font_file) {
	ttf_font_edit->setText( font_file );
}

QString PrefSubtitles::fontFile() {
	return ttf_font_edit->text();
}


void PrefSubtitles::setUseFontconfig(bool b) {
	system_font_button->setChecked(b);
	ttf_font_button->setChecked(!b);
}

bool PrefSubtitles::useFontconfig() {
	return system_font_button->isChecked();
}

void PrefSubtitles::setFontAutoscale(int n) {
	font_autoscale_combo->setCurrentIndex(n);
}

int PrefSubtitles::fontAutoscale() {
	return font_autoscale_combo->currentIndex();
}

void PrefSubtitles::setFontTextscale(double n) {
	font_text_scale->setValue(n);
}

double PrefSubtitles::fontTextscale() {
	return font_text_scale->value();
}

void PrefSubtitles::setAssFontScale(double n) {
	ass_font_scale->setValue(n);
}

double PrefSubtitles::assFontScale() {
	return ass_font_scale->value();
}

void PrefSubtitles::setAutoloadSub(bool v) {
	font_autoload_check->setChecked(v);
}

bool PrefSubtitles::autoloadSub() {
	return font_autoload_check->isChecked();
}

void PrefSubtitles::setFontEncoding(QString s) {
	int i = font_encoding_combo->findData(s);
	font_encoding_combo->setCurrentIndex(i);
}

QString PrefSubtitles::fontEncoding() {
	int index = font_encoding_combo->currentIndex();
	return font_encoding_combo->itemData(index).toString();
}

void PrefSubtitles::setEncaLang(QString s) {
	int i = enca_lang_combo->findData(s);
	enca_lang_combo->setCurrentIndex(i);
}

QString PrefSubtitles::encaLang() {
	int index = enca_lang_combo->currentIndex();
	return enca_lang_combo->itemData(index).toString();
}

void PrefSubtitles::setUseEnca(bool b) {
	use_enca_check->setChecked(b);
}

bool PrefSubtitles::useEnca() {
	return use_enca_check->isChecked();
}

void PrefSubtitles::setSubPos(int pos) {
	sub_pos_slider->setValue(pos);
}

int PrefSubtitles::subPos() {
	return sub_pos_slider->value();
}

void PrefSubtitles::setUseFontASS(bool v) {
	font_ass_check->setChecked(v);
	//assButtonToggled(v);
}

bool PrefSubtitles::useFontASS() {
	return font_ass_check->isChecked();
}

#if !USE_ASS_STYLES
void PrefSubtitles::setAssColor( unsigned int color ) {
	colorButton->setColor(color);
}

unsigned int PrefSubtitles::assColor() {
	return colorButton->color().rgb();
}

void PrefSubtitles::setAssBorderColor( unsigned int color ) {
	borderButton->setColor(color);
}

unsigned int PrefSubtitles::assBorderColor() {
	return borderButton->color().rgb();
}

void PrefSubtitles::setAssStyles(QString styles) {
	ass_styles_edit->setText(styles);
}

QString PrefSubtitles::assStyles() {
	return ass_styles_edit->text();
}
#endif

void PrefSubtitles::setFontFuzziness(int n) {
	font_autoload_combo->setCurrentIndex(n);
}

int PrefSubtitles::fontFuzziness() {
	return font_autoload_combo->currentIndex();
}

void PrefSubtitles::setSubtitlesOnScreenshots(bool b) {
	subtitles_on_screeshots_check->setChecked(b);
}

bool PrefSubtitles::subtitlesOnScreenshots() {
	return subtitles_on_screeshots_check->isChecked();
}


void PrefSubtitles::createHelp() {
	clearHelp();

	addSectionTitle(tr("Subtitles"));

	setWhatsThis(font_autoload_combo, tr("Autoload"), 
        tr("Select the subtitle autoload method.") );

	setWhatsThis(font_autoload_check, tr("Select first available subtitle"), 
        tr("If there are one or more subtitle tracks available, one of them "
           "will be automatically selected, usually the first one, although if "
           "one of them matches the user's preferred language that one will "
           "be used instead.") );

	setWhatsThis(font_encoding_combo, tr("Default subtitle encoding"), 
        tr("Select the encoding which will be used for subtitle files "
           "by default.") );

	setWhatsThis(use_enca_check, tr("Try to autodetect for this language"),
		tr("When this option is on, the encoding of the subtitles will be "
           "tried to be autodetected for the given language. "
           "It will fall back to the default encoding if the autodetection "
           "fails. This option requires a MPlayer compiled with ENCA "
           "support.") );

	setWhatsThis(enca_lang_combo, tr("Subtitle language"),
		tr("Select the language for which you want the encoding to be guessed "
           "automatically.") );

	setWhatsThis(sub_pos_slider, tr("Subtitle position"),
		tr("This option specifies the position of the subtitles over the "
           "video window. <i>100</i> means the bottom, while <i>0</i> means "
           "the top." ) );

	setWhatsThis(subtitles_on_screeshots_check, 
        tr("Include subtitles on screenshots"), 
        tr("If this option is checked, the subtitles will appear in the "
           "screenshots. Note: it may cause some troubles sometimes." ) );

	addSectionTitle(tr("Font"));

	setWhatsThis(ttf_font_edit, tr("TTF font"), 
        tr("Here you can select a ttf font to be used for the subtitles. "
           "Usually you'll find a lot of ttf fonts in %1")
#ifdef Q_OS_WIN
        .arg("<i>C:\\Windows\\Fonts\\</i>")
#else
        .arg("<i>/usr/X11R6/lib/X11/fonts/truetype/</i>")
#endif
        );

	setWhatsThis(fontCombo, tr("System font"), 
        tr("Here you can select a system font to be used for the subtitles "
           "and OSD. <b>Note:</b> requires a MPlayer with fontconfig support.") );

	setWhatsThis(font_autoscale_combo, tr("Autoscale"), 
        tr("Select the subtitle autoscaling method.") );

	if (Global::pref->show_font_scale_options_in_preferences) 
	{
		setWhatsThis(font_text_scale, tr("Default scale for normal subtitles"),
			tr("This option specifies the default font scale for normal (white) "
	           "subtitles which will be used for new videos."));

		setWhatsThis(ass_font_scale, tr("Default scale for SSA/ASS subtitles"),
			tr("This option specifies the default font scale for SSA/ASS "
	           "subtitles which will be used for new videos."));
	}

	addSectionTitle(tr("SSA/ASS library"));

	setWhatsThis(font_ass_check, tr("Use SSA/ASS library for subtitle rendering"), 
        tr("Check this option to activate the SSA/ASS library. It provides "
           "nicer subtitles, so it's a good idea to do it.") );

	setWhatsThis(colorButton, tr("Text color"), 
        tr("Select the color for the text of the subtitles.") );

	setWhatsThis(borderButton, tr("Border color"), 
        tr("Select the color for the border of the subtitles.") );

	setWhatsThis(ass_styles_edit, tr("SSA/ASS styles"), 
		tr("Here you can override styles for SSA/ASS subtitles. "
           "It can be also used for fine-tuning the rendering of SRT and SUB "
           "subtitles by the SSA/ASS library. "
           "Example: <b>Bold=1,Outline=2,Shadow=4</b>"));
}

#include "moc_prefsubtitles.cpp"
