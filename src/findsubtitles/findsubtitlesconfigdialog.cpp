/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "findsubtitlesconfigdialog.h"
#include <QNetworkProxy>

FindSubtitlesConfigDialog::FindSubtitlesConfigDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	proxy_type_combo->addItem( tr("Http"), QNetworkProxy::HttpProxy);
	proxy_type_combo->addItem( tr("Socks5"), QNetworkProxy::Socks5Proxy);

	use_proxy_check->setWhatsThis( tr("Enable/disable the use of the proxy.") );
	proxy_hostname_edit->setWhatsThis( tr("The host name of the proxy.") );
	proxy_port_spin->setWhatsThis( tr("The port of the proxy.") );
	proxy_username_edit->setWhatsThis( tr("If the proxy requires authentication, this sets the username.") );
	proxy_password_edit->setWhatsThis( 
        tr("The password for the proxy. <b>Warning:</b> the password will be saved "
           "as plain text in the configuration file.") );
	proxy_type_combo->setWhatsThis( tr("Select the proxy type to be used.") );

	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

FindSubtitlesConfigDialog::~FindSubtitlesConfigDialog() {
}

void FindSubtitlesConfigDialog::setServer(QString server) {
	server_edit->setText(server);
}

QString FindSubtitlesConfigDialog::server() {
	return server_edit->text();
}

void FindSubtitlesConfigDialog::setUseProxy(bool b) {
	use_proxy_check->setChecked(b);
}

bool FindSubtitlesConfigDialog::useProxy() {
	return 	use_proxy_check->isChecked();
}

void FindSubtitlesConfigDialog::setProxyHostname(QString host) {
	proxy_hostname_edit->setText(host);
}

QString FindSubtitlesConfigDialog::proxyHostname() {
	return proxy_hostname_edit->text();
}

void FindSubtitlesConfigDialog::setProxyPort(int port) {
	proxy_port_spin->setValue(port);
}

int FindSubtitlesConfigDialog::proxyPort() {
	return proxy_port_spin->value();
}

void FindSubtitlesConfigDialog::setProxyUsername(QString username) {
	proxy_username_edit->setText(username);
}

QString FindSubtitlesConfigDialog::proxyUsername() {
	return proxy_username_edit->text();
}

void FindSubtitlesConfigDialog::setProxyPassword(QString password) {
	proxy_password_edit->setText(password);
}

QString FindSubtitlesConfigDialog::proxyPassword() {
	return proxy_password_edit->text();
}

void FindSubtitlesConfigDialog::setProxyType(int type) {
	int index = proxy_type_combo->findData(type);
	if (index == -1) index = 0;
	proxy_type_combo->setCurrentIndex(index);
}

int FindSubtitlesConfigDialog::proxyType() {
	int index = proxy_type_combo->currentIndex();
	return proxy_type_combo->itemData(index).toInt();
}

#include "moc_findsubtitlesconfigdialog.cpp"
