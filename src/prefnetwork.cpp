/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "prefnetwork.h"
#include "preferences.h"
#include "images.h"
#include <QNetworkProxy>

#ifdef YOUTUBE_SUPPORT
#include "retrieveyoutubeurl.h"
#endif

PrefNetwork::PrefNetwork(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	proxy_type_combo->addItem( tr("HTTP"), QNetworkProxy::HttpProxy);
	proxy_type_combo->addItem( tr("SOCKS5"), QNetworkProxy::Socks5Proxy);

#ifdef YOUTUBE_SUPPORT
	yt_quality_combo->addItem( "240p (flv)", RetrieveYoutubeUrl::FLV_240p );

	yt_quality_combo->addItem( "360p (flv)", RetrieveYoutubeUrl::FLV_360p );
	yt_quality_combo->addItem( "360p (mp4)", RetrieveYoutubeUrl::MP4_360p );
	yt_quality_combo->addItem( "360p (webm)", RetrieveYoutubeUrl::WEBM_360p );

	yt_quality_combo->addItem( "480p (flv)", RetrieveYoutubeUrl::FLV_480p );
	yt_quality_combo->addItem( "480p (webm)", RetrieveYoutubeUrl::WEBM_480p );

	yt_quality_combo->addItem( "720p (mp4)", RetrieveYoutubeUrl::MP4_720p );
	yt_quality_combo->addItem( "720p (webm)", RetrieveYoutubeUrl::WEBM_720p );

	yt_quality_combo->addItem( "1080p (mp4)", RetrieveYoutubeUrl::MP4_1080p );
	yt_quality_combo->addItem( "1080p (webm)", RetrieveYoutubeUrl::WEBM_1080p );
#else
	int i = tab_widget->indexOf(youtube_page);
	if (i != -1) tab_widget->removeTab(i);
#endif

	createHelp();
}

PrefNetwork::~PrefNetwork()
{
}

QString PrefNetwork::sectionName() {
	return tr("Network");
}

QPixmap PrefNetwork::sectionIcon() {
	return Images::icon("pref_network", 22);
}

void PrefNetwork::retranslateStrings() {
	retranslateUi(this);
	createHelp();
}

void PrefNetwork::setData(Preferences * pref) {
	use_proxy_check->setChecked(pref->use_proxy);
	proxy_hostname_edit->setText(pref->proxy_host);
	proxy_port_spin->setValue(pref->proxy_port);
	proxy_username_edit->setText(pref->proxy_username);
	proxy_password_edit->setText(pref->proxy_password);

	setProxyType(pref->proxy_type);

#ifdef YOUTUBE_SUPPORT
	yt_support_check->setChecked(pref->enable_yt_support);
	setYTQuality( pref->yt_quality );
	yt_user_agent_edit->setText( pref->yt_user_agent );
#endif
	streaming_check->setChecked(pref->enable_streaming_sites);
}

void PrefNetwork::getData(Preferences * pref) {
	requires_restart = false;

	pref->use_proxy = use_proxy_check->isChecked();
	pref->proxy_host = proxy_hostname_edit->text();
	pref->proxy_port = proxy_port_spin->value();
	pref->proxy_username = proxy_username_edit->text();
	pref->proxy_password = proxy_password_edit->text();

	pref->proxy_type = proxyType();

#ifdef YOUTUBE_SUPPORT
	pref->enable_yt_support = yt_support_check->isChecked();
	pref->yt_quality = YTQuality();
	pref->yt_user_agent = yt_user_agent_edit->text();
#endif
	pref->enable_streaming_sites = streaming_check->isChecked();
}

void PrefNetwork::setProxyType(int type) {
	int index = proxy_type_combo->findData(type);
	if (index == -1) index = 0;
	proxy_type_combo->setCurrentIndex(index);
}

int PrefNetwork::proxyType() {
	int index = proxy_type_combo->currentIndex();
	return proxy_type_combo->itemData(index).toInt();
}

#ifdef YOUTUBE_SUPPORT
void PrefNetwork::setYTQuality(int q) {
	yt_quality_combo->setCurrentIndex(yt_quality_combo->findData(q));
}

int PrefNetwork::YTQuality() {
	int index = yt_quality_combo->currentIndex();
    return yt_quality_combo->itemData(index).toInt();
}
#endif

void PrefNetwork::createHelp() {
	clearHelp();

#ifdef YOUTUBE_SUPPORT
	addSectionTitle(tr("Youtube"));

	setWhatsThis(yt_support_check, tr("Enable Youtube internal support"),
		tr("If this option is checked, SMPlayer will try to play videos from Youtube URLs.") );

	setWhatsThis(yt_quality_combo, tr("Youtube quality"),
		tr("Select the preferred quality for youtube videos.") );

	setWhatsThis(yt_user_agent_edit, tr("User agent"),
		tr("Set the user agent that SMPlayer will use when connecting to Youtube.") );
#endif

	setWhatsThis(streaming_check, tr("Play videos from streaming sites"),
		tr("If this option is checked, SMPlayer will try to play videos from "
           "streaming sites like Youtube, Dailymotion, Vimeo, Vevo, etc.") + "<br>"+
		tr("Requires mpv and youtube-dl.") );

	addSectionTitle(tr("Proxy"));

	setWhatsThis(use_proxy_check, tr("Enable proxy"),
		tr("Enable/disable the use of the proxy.") );

	setWhatsThis(proxy_hostname_edit, tr("Host"),
		tr("The host name of the proxy.") );

	setWhatsThis(proxy_port_spin, tr("Port"),
		tr("The port of the proxy.") );

	setWhatsThis(proxy_username_edit, tr("Username"),
		tr("If the proxy requires authentication, this sets the username.") );

	setWhatsThis(proxy_password_edit, tr("Password"),
		tr("The password for the proxy. <b>Warning:</b> the password will be saved "
           "as plain text in the configuration file.") );

	setWhatsThis(proxy_type_combo, tr("Type"),
		tr("Select the proxy type to be used.") );

}

#include "moc_prefnetwork.cpp"
