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


#include "prefsubtitles.h"
#include "images.h"
#include "preferences.h"
#include "paths.h"
#include "assstyles.h"
#include "filedialog.h"
#include "languages.h"
#include "playerid.h"

#include <QInputDialog>

PrefSubtitles::PrefSubtitles(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	ass_subs->setEnabled(false);

	connect(ass_custom_check, SIGNAL(toggled(bool)),
            ass_subs, SLOT(setEnabled(bool)));

	/*
	connect(use_ass_check, SIGNAL(toggled(bool)),
            tab2, SLOT(setEnabled(bool)));
	*/

	connect( style_border_style_combo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(checkBorderStyleCombo(int)) );

#ifndef FONTS_HACK
	windowsfontdir_check->hide();
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
	int sub_autoload_item = sub_autoload_combo->currentIndex();

	retranslateUi(this);

	sub_autoload_combo->setCurrentIndex(sub_autoload_item);

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

	l = Languages::enca(); 
	i = l;
	while (i.hasNext()) {
		i.next();
		enca_lang_combo->addItem( i.value() + " (" + i.key() + ")", i.key() );
	}

	font_encoding_combo->model()->sort(0);
	enca_lang_combo->model()->sort(0);
	//font_encoding_combo->setCurrentIndex(font_encoding_item);
	setFontEncoding(current_encoding);
	setEncaLang(current_enca_lang);

	// Ass styles
	int alignment_item = style_alignment_combo->currentIndex();
	style_alignment_combo->clear();
	style_alignment_combo->addItem(tr("Left", "horizontal alignment"), AssStyles::Left);
	style_alignment_combo->addItem(tr("Centered", "horizontal alignment"), AssStyles::HCenter);
	style_alignment_combo->addItem(tr("Right", "horizontal alignment"), AssStyles::Right);
	style_alignment_combo->setCurrentIndex(alignment_item);

	int valignment_item = style_valignment_combo->currentIndex();
	style_valignment_combo->clear();
	style_valignment_combo->addItem(tr("Bottom", "vertical alignment"), AssStyles::Bottom);
	style_valignment_combo->addItem(tr("Middle", "vertical alignment"), AssStyles::VCenter);
	style_valignment_combo->addItem(tr("Top", "vertical alignment"), AssStyles::Top);
	style_valignment_combo->setCurrentIndex(valignment_item);

	int borderstyle_item = style_border_style_combo->currentIndex();
	style_border_style_combo->clear();
	style_border_style_combo->addItem(tr("Outline", "border style"), AssStyles::Outline);
	style_border_style_combo->addItem(tr("Opaque box", "border style"), AssStyles::Opaque);
	style_border_style_combo->setCurrentIndex(borderstyle_item);

	createHelp();
}

void PrefSubtitles::setData(Preferences * pref) {
	setAssFontScale( pref->initial_sub_scale_ass );
	setAutoloadSub( pref->autoload_sub );
	setSubFuzziness( pref->subfuzziness );
	setFontEncoding( pref->subcp );
	setUseEnca( pref->use_enca );
	setEncaLang( pref->enca_lang );
	setAssLineSpacing( pref->ass_line_spacing );
	setSubtitlesOnScreenshots( pref->subtitles_on_screenshots );
	setFreetypeSupport( pref->freetype_support );
	use_ass_check->setChecked( pref->use_ass_subtitles );

	// Load ass styles
	style_font_combo->setCurrentText(pref->ass_styles.fontname);
	style_size_spin->setValue(pref->ass_styles.fontsize);
	style_text_color_button->setColor(pref->ass_styles.primarycolor);
	style_border_color_button->setColor(pref->ass_styles.outlinecolor);
	style_shadow_color_button->setColor(pref->ass_styles.backcolor);
	style_bold_check->setChecked(pref->ass_styles.bold);
	style_italic_check->setChecked(pref->ass_styles.italic);
	style_alignment_combo->setCurrentIndex(style_alignment_combo->findData(pref->ass_styles.halignment));
	style_valignment_combo->setCurrentIndex(pref->ass_styles.valignment);
	style_border_style_combo->setCurrentIndex(style_border_style_combo->findData(pref->ass_styles.borderstyle));
	style_outline_spin->setValue(pref->ass_styles.outline);
	style_shadow_spin->setValue(pref->ass_styles.shadow);
	style_marginl_spin->setValue(pref->ass_styles.marginl);
	style_marginr_spin->setValue(pref->ass_styles.marginr);
	style_marginv_spin->setValue(pref->ass_styles.marginv);

	setForceAssStyles(pref->force_ass_styles);
	setCustomizedAssStyle(pref->user_forced_ass_style);

	ass_custom_check->setChecked(pref->enable_ass_styles);

#ifdef FONTS_HACK
	windowsfontdir_check->setChecked(pref->use_windowsfontdir);
	if (!windowsfontdir_check->isChecked()) on_windowsfontdir_check_toggled(false);
#endif
}

void PrefSubtitles::getData(Preferences * pref) {
	requires_restart = false;

	pref->initial_sub_scale_ass = assFontScale();
	TEST_AND_SET(pref->autoload_sub, autoloadSub());
	TEST_AND_SET(pref->subfuzziness, subFuzziness());
	TEST_AND_SET(pref->subcp, fontEncoding());
	TEST_AND_SET(pref->use_enca, useEnca());
	TEST_AND_SET(pref->enca_lang, encaLang());
	TEST_AND_SET(pref->ass_line_spacing, assLineSpacing());
	TEST_AND_SET(pref->subtitles_on_screenshots, subtitlesOnScreenshots());
	TEST_AND_SET(pref->freetype_support, freetypeSupport());
	TEST_AND_SET(pref->use_ass_subtitles, use_ass_check->isChecked());

	// Save ass styles
	TEST_AND_SET(pref->ass_styles.fontname, style_font_combo->currentText());
	TEST_AND_SET(pref->ass_styles.fontsize, style_size_spin->value());
	TEST_AND_SET(pref->ass_styles.primarycolor, style_text_color_button->color());
	TEST_AND_SET(pref->ass_styles.outlinecolor, style_border_color_button->color());
	TEST_AND_SET(pref->ass_styles.backcolor, style_shadow_color_button->color());
	TEST_AND_SET(pref->ass_styles.bold, style_bold_check->isChecked());
	TEST_AND_SET(pref->ass_styles.italic, style_italic_check->isChecked());
	TEST_AND_SET(pref->ass_styles.halignment, style_alignment_combo->itemData(style_alignment_combo->currentIndex()).toInt());
	TEST_AND_SET(pref->ass_styles.valignment, style_valignment_combo->currentIndex());
	TEST_AND_SET(pref->ass_styles.borderstyle, style_border_style_combo->itemData(style_border_style_combo->currentIndex()).toInt());
	TEST_AND_SET(pref->ass_styles.outline, style_outline_spin->value());
	TEST_AND_SET(pref->ass_styles.shadow, style_shadow_spin->value());
	TEST_AND_SET(pref->ass_styles.marginl, style_marginl_spin->value());
	TEST_AND_SET(pref->ass_styles.marginr, style_marginr_spin->value());
	TEST_AND_SET(pref->ass_styles.marginv, style_marginv_spin->value());

	pref->ass_styles.exportStyles( Paths::subtitleStyleFile() );

	TEST_AND_SET(pref->force_ass_styles, forceAssStyles());
	TEST_AND_SET(pref->user_forced_ass_style, customizedAssStyle());

	TEST_AND_SET(pref->enable_ass_styles, ass_custom_check->isChecked());

#ifdef FONTS_HACK
	pref->use_windowsfontdir = windowsfontdir_check->isChecked();
#endif
}

void PrefSubtitles::checkBorderStyleCombo( int index ) {
	bool b = (index == 0);
	style_outline_spin->setEnabled(b);
	style_shadow_spin->setEnabled(b);
	style_outline_label->setEnabled(b);
	style_shadow_label->setEnabled(b);
}



void PrefSubtitles::setAssFontScale(double n) {
	ass_font_scale_spin->setValue(n);
}

double PrefSubtitles::assFontScale() {
	return ass_font_scale_spin->value();
}

void PrefSubtitles::setAutoloadSub(bool v) {
	sub_autoload_check->setChecked(v);
}

bool PrefSubtitles::autoloadSub() {
	return sub_autoload_check->isChecked();
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

void PrefSubtitles::setSubFuzziness(int n) {
	sub_autoload_combo->setCurrentIndex(n);
}

int PrefSubtitles::subFuzziness() {
	return sub_autoload_combo->currentIndex();
}

void PrefSubtitles::setSubtitlesOnScreenshots(bool b) {
	subtitles_on_screeshots_check->setChecked(b);
}

bool PrefSubtitles::subtitlesOnScreenshots() {
	return subtitles_on_screeshots_check->isChecked();
}

void PrefSubtitles::setAssLineSpacing(int spacing) {
	ass_line_spacing_spin->setValue(spacing);
}

int PrefSubtitles::assLineSpacing() {
	return ass_line_spacing_spin->value();
}

void PrefSubtitles::setForceAssStyles(bool b) {
	force_ass_styles->setChecked(b);
}

bool PrefSubtitles::forceAssStyles() {
	return force_ass_styles->isChecked();
}

/*
void PrefSubtitles::on_ass_subs_button_toggled(bool b) {
	if (b)
		stackedWidget->setCurrentIndex(1);
	 else 
		stackedWidget->setCurrentIndex(0);
}
*/

void PrefSubtitles::on_ass_customize_button_clicked() {
	bool ok;

	QString edit = forced_ass_style;

	// A copy with the current values in the dialog
	AssStyles ass_styles;
	ass_styles.fontname = style_font_combo->currentText();
	ass_styles.fontsize = style_size_spin->value();
	ass_styles.primarycolor = style_text_color_button->color();
	ass_styles.outlinecolor = style_border_color_button->color();
	ass_styles.backcolor = style_shadow_color_button->color();
	ass_styles.bold = style_bold_check->isChecked();
	ass_styles.italic = style_italic_check->isChecked();
	ass_styles.halignment = style_alignment_combo->itemData(style_alignment_combo->currentIndex()).toInt();
	ass_styles.valignment = style_valignment_combo->currentIndex();
	ass_styles.borderstyle = style_border_style_combo->itemData(style_border_style_combo->currentIndex()).toInt();
	ass_styles.outline = style_outline_spin->value();
	ass_styles.shadow = style_shadow_spin->value();
	ass_styles.marginl = style_marginl_spin->value();
	ass_styles.marginr = style_marginr_spin->value();
	ass_styles.marginv = style_marginv_spin->value();

	if (edit.isEmpty()) {
		edit = ass_styles.toString();
	}

	QString s = QInputDialog::getText(this, tr("Customize SSA/ASS style"),
                                      tr("Here you can enter your customized SSA/ASS style.") +"<br>"+
                                      tr("Clear the edit line to disable the customized style."), 
                                      QLineEdit::Normal, 
                                      edit, &ok );
	if (ok) {
		if (s == ass_styles.toString()) s.clear(); // Clear string if it wasn't changed by the user
		setCustomizedAssStyle(s);
	}
}

void PrefSubtitles::setFreetypeSupport(bool b) {
	freetype_check->setChecked(b);
}

bool PrefSubtitles::freetypeSupport() {
	return freetype_check->isChecked();
}

void PrefSubtitles::on_freetype_check_toggled(bool b) {
	qDebug("PrefSubtitles:on_freetype_check_toggled: %d", b);
}

#ifdef FONTS_HACK
void PrefSubtitles::on_windowsfontdir_check_toggled(bool b) {
	qDebug("PrefSubtitles::on_windowsfontdir_check_toggled: %d", b);

	if (b) {
		style_font_combo->setFontsFromDir(QString::null);
	} else {
		QString fontdir = Paths::fontPath();
		//QString fontdir = "/tmp/fonts/";
		style_font_combo->setFontsFromDir(fontdir);

		// Calling setFontsFromDir resets the fonts in other comboboxes!
		// So the font list is copied from the previous combobox
		/*
		QString current_text = fontCombo->currentText();
		fontCombo->clear();
		for (int n=0; n < style_font_combo->count(); n++) {
			fontCombo->addItem( style_font_combo->itemText(n) );
		}
		fontCombo->setCurrentText(current_text);
		*/
	}
}
#endif

void PrefSubtitles::createHelp() {
	clearHelp();

	addSectionTitle(tr("Subtitles"));

	setWhatsThis(sub_autoload_combo, tr("Autoload"),
        tr("Select the subtitle autoload method.") );

	setWhatsThis(sub_autoload_check, tr("Select first available subtitle"), 
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
           "fails. This option requires a %1 with ENCA "
           "support.").arg(PLAYER_NAME) );

	setWhatsThis(enca_lang_combo, tr("Subtitle language"),
		tr("Select the language for which you want the encoding to be guessed "
           "automatically.") );

	setWhatsThis(subtitles_on_screeshots_check, 
        tr("Include subtitles on screenshots"), 
        tr("If this option is checked, the subtitles will appear in the "
           "screenshots. <b>Note:</b> it may cause some troubles sometimes." ) );

	setWhatsThis(use_ass_check, tr("Use the ASS library"),
		tr("This option enables the ASS library, which allows to display "
           "subtitles with multiple colors, fonts...") );

	setWhatsThis(freetype_check, tr("Freetype support"), 
		tr("You should normally not disable this option. Do it only if your "
           "%1 is compiled without freetype support. "
           "<b>Disabling this option could make subtitles not to work "
           "at all!</b>").arg(PLAYER_NAME));

#ifdef FONTS_HACK
	setWhatsThis(windowsfontdir_check, tr("Enable Windows fonts"),
		tr("If this option is enabled the Windows system fonts will be "
           "available for subtitles. There's an inconvenience: a font cache have "
           "to be created which can take some time.") +"<br>"+
		tr("If this option is not checked then only a few fonts bundled with SMPlayer "
           "can be used, but this is faster.") );
#endif

	addSectionTitle(tr("Font"));

	QString scale_note = tr("This option does NOT change the size of the "
           "subtitles in the current video. To do so, use the options "
           "<i>Size+</i> and <i>Size-</i> in the subtitles menu.");

	setWhatsThis(ass_font_scale_spin, tr("Default scale"),
		tr("This option specifies the default font scale for SSA/ASS "
           "subtitles which will be used for new opened files.") +"<br>"+
		scale_note);

	setWhatsThis(ass_line_spacing_spin, tr("Line spacing"),
		tr("This specifies the spacing that will be used to separate "
           "multiple lines. It can have negative values.") );

	setWhatsThis(styles_container, tr("SSA/ASS style"), 
		tr("The following options allows you to define the style to "
           "be used for non-styled subtitles (srt, sub...).") );
       
	setWhatsThis(style_font_combo, tr("Font"), 
		tr("Select the font for the subtitles.") );

	setWhatsThis(style_size_spin, tr("Size"), 
		tr("The size in pixels.") );

	setWhatsThis(style_bold_check, tr("Bold"), 
		tr("If checked, the text will be displayed in <b>bold</b>.") ); 

	setWhatsThis(style_italic_check, tr("Italic"), 
		tr("If checked, the text will be displayed in <i>italic</i>.") ); 

	setWhatsThis(style_text_color_button, tr("Text color"), 
        tr("Select the color for the text of the subtitles.") );

	setWhatsThis(style_border_color_button, tr("Border color"), 
        tr("Select the color for the border of the subtitles.") );

	setWhatsThis(style_shadow_color_button, tr("Shadow color"), 
        tr("This color will be used for the shadow of the subtitles.") );

	setWhatsThis(style_marginl_spin, tr("Left margin"), 
        tr("Specifies the left margin in pixels.") );

	setWhatsThis(style_marginr_spin, tr("Right margin"), 
        tr("Specifies the right margin in pixels.") );

	setWhatsThis(style_marginv_spin, tr("Vertical margin"), 
        tr("Specifies the vertical margin in pixels.") );

	setWhatsThis(style_alignment_combo, tr("Horizontal alignment"), 
        tr("Specifies the horizontal alignment. Possible values are "
           "left, centered and right.") );

	setWhatsThis(style_valignment_combo, tr("Vertical alignment"), 
        tr("Specifies the vertical alignment. Possible values: "
           "bottom, middle and top.") );

	setWhatsThis(style_border_style_combo, tr("Border style"), 
        tr("Specifies the border style. Possible values: outline "
           "and opaque box.") );

	setWhatsThis(style_outline_spin, tr("Outline"), 
        tr("If border style is set to <i>outline</i>, this option specifies "
           "the width of the outline around the text in pixels.") );

	setWhatsThis(style_shadow_spin, tr("Shadow"), 
        tr("If border style is set to <i>outline</i>, this option specifies "
           "the depth of the drop shadow behind the text in pixels.") );

	setWhatsThis(force_ass_styles, tr("Apply style to ASS files too"), 
        tr("If this option is checked, the style defined above will be "
           "applied to ass subtitles too.") );
}

#include "moc_prefsubtitles.cpp"
