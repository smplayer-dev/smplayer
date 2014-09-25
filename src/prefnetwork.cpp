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

PrefNetwork::PrefNetwork(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

	proxy_type_combo->addItem( tr("HTTP"), QNetworkProxy::HttpProxy);
	proxy_type_combo->addItem( tr("SOCKS5"), QNetworkProxy::Socks5Proxy);

	createHelp();
}

PrefNetwork::~PrefNetwork()
{
}

QString PrefNetwork::sectionName() {
	return tr("Network");
}

QPixmap PrefNetwork::sectionIcon() {
	return Images::icon("pref_updates", 22); // FIXME
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
}

void PrefNetwork::getData(Preferences * pref) {
	requires_restart = false;

	pref->use_proxy = use_proxy_check->isChecked();
	pref->proxy_host = proxy_hostname_edit->text();
	pref->proxy_port = proxy_port_spin->value();
	pref->proxy_username = proxy_username_edit->text();
	pref->proxy_password = proxy_password_edit->text();

	pref->proxy_type = proxyType();
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


void PrefNetwork::createHelp() {
	clearHelp();

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
