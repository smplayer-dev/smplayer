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

#include "findsubtitlesconfigdialog.h"
#include <QNetworkProxy>

FindSubtitlesConfigDialog::FindSubtitlesConfigDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	search_method_combo->addItem(tr("Hash"), Hash);
	search_method_combo->addItem(tr("Filename"), Filename);

#ifdef FS_USE_PROXY
	proxy_type_combo->addItem( tr("HTTP"), QNetworkProxy::HttpProxy);
	proxy_type_combo->addItem( tr("SOCKS5"), QNetworkProxy::Socks5Proxy);

	use_proxy_check->setWhatsThis( tr("Enable/disable the use of the proxy.") );
	proxy_hostname_edit->setWhatsThis( tr("The host name of the proxy.") );
	proxy_port_spin->setWhatsThis( tr("The port of the proxy.") );
	proxy_username_edit->setWhatsThis( tr("If the proxy requires authentication, this sets the username.") );
	proxy_password_edit->setWhatsThis( 
        tr("The password for the proxy. <b>Warning:</b> the password will be saved "
           "as plain text in the configuration file.") );
	proxy_type_combo->setWhatsThis( tr("Select the proxy type to be used.") );
#else
	proxy_group->hide();
#endif

#ifndef OS_SEARCH_WORKAROUND
	retries_label->hide();
	retries_spin->hide();
#endif

#ifndef DOWNLOAD_SUBS
	misc_group->hide();
#endif

#ifndef FS_USE_SERVER_CONFIG
	server_label->hide();
	server_edit->hide();
#endif

#if !defined(FS_USE_SERVER_CONFIG) && !defined(OS_SEARCH_WORKAROUND)
	server_group->hide();
#endif

	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

FindSubtitlesConfigDialog::~FindSubtitlesConfigDialog() {
}

#ifdef FS_USE_SERVER_CONFIG
void FindSubtitlesConfigDialog::setServer(QString server) {
	server_edit->setText(server);
}

QString FindSubtitlesConfigDialog::server() {
	return server_edit->text();
}
#endif

void FindSubtitlesConfigDialog::setSearchMethod(SearchMethod m) {
	int pos = search_method_combo->findData(m);
	if (pos == -1) pos = 0;
	search_method_combo->setCurrentIndex(pos);
}

int FindSubtitlesConfigDialog::searchMethod() {
	return search_method_combo->itemData(search_method_combo->currentIndex()).toInt();
}

#ifdef OS_SEARCH_WORKAROUND
void FindSubtitlesConfigDialog::setRetries(int n) {
	retries_spin->setValue(n);
}

int FindSubtitlesConfigDialog::retries() {
	return retries_spin->value();
}
#endif

#ifdef DOWNLOAD_SUBS
void FindSubtitlesConfigDialog::setAppendLang(bool b) {
	addlang_check->setChecked(b);
}

bool FindSubtitlesConfigDialog::appendLang() {
	return addlang_check->isChecked();
}
#endif

#ifdef FS_USE_PROXY
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
#endif

#include "moc_findsubtitlesconfigdialog.cpp"
