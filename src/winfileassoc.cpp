/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2007 Ricardo Villalba <rvm@escomposlinux.org>

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


	Winfileassoc.cpp

	Handles file associations in Windows XP/2000 and Vista .
	We assume that the code is run without administrator privileges, so the associations are done for current user only.
	System-wide associations require writing to HKEY_CLASSES_ROOT and we don't want to get our hands dirty with that.
	Each user on the computer can configure his own set of file associations for SMPlayer, which is extremely cool.

	Optionally, during uninstall, it would be a good idea to call RestoreFileAssociations for all media types so
	that we can clean up the registry and restore the old associations for current user. 

	Vista:
	The code can only register the app as default program for selected extensions and check if it is the default. 
	It cannot restore 'old' default application, since this doesn't seem to be possible with the current Vista API.

	Author: Florin Braghis (florin@libertv.ro)
*/

#include "winfileassoc.h"
#include <QSettings>
#include <QApplication>


/*
*/

WinFileAssoc::WinFileAssoc( const QString ClassId, const QString AppName )
{
	m_ClassId = ClassId;
	m_AppName = AppName; 
}

int WinFileAssoc::CreateFileAssociations(const QStringList& fileExtensions)
{
	if (QSysInfo::WindowsVersion == QSysInfo::WV_VISTA)
	{
		return VistaSetAppsAsDefault(fileExtensions); 
	}

	//Registry keys modified:
	//HKEY_LOCAL_MACHINE\.extension 
	//Shell 'Open With...' entry:
	//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.avi

	QSettings RegCR ("HKEY_CLASSES_ROOT", QSettings::NativeFormat); //Read only
	QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);

	//Check if our key exists in RegCR
	if (!RegCR.contains(m_ClassId))
	{
		//If doesn't exist (user didn't run the setup program), try to create the ClassId for current user.
		if (!CreateClassId(QApplication::applicationFilePath(), "SMPlayer Media Player"))
			return 0; 
	}

	if (!RegCU.isWritable() || RegCU.status() != QSettings::NoError)
		return 0; 

	
	int count = 0; 
	foreach(const QString& fileExtension, fileExtensions)
	{
		QString ExtKeyName = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/.%1").arg(fileExtension);
		QString ClassesKeyName = m_ClassId;

		QString BackupKeyName = ClassesKeyName + "/" + fileExtension; 
		QString CUKeyName = "Software/Classes/." + fileExtension; 

		//Save current ClassId for current user
		QString KeyVal = RegCU.value(CUKeyName + "/.").toString(); 

		if (KeyVal.length() == 0 || KeyVal == m_ClassId)
		{
			//No registered app for this extension for current user.
			//Check the system-wide (HKEY_CLASSES_ROOT) ClassId for this extension 
			KeyVal = RegCR.value("." + fileExtension + "/.").toString();
		}

		if (KeyVal != m_ClassId)
			RegCU.setValue(CUKeyName + "/MPlayer_Backup", KeyVal); 

		//Save last ProgId and Application values from the Exts key
		KeyVal = RegCU.value(ExtKeyName + "/Progid").toString();
		if (KeyVal != m_ClassId)
			RegCU.setValue(ExtKeyName + "/MPlayer_Backup_ProgId", KeyVal);

		KeyVal = RegCU.value(ExtKeyName + "/Application").toString(); 
		if (KeyVal != m_ClassId) 
			RegCU.setValue(ExtKeyName + "/MPlayer_Backup_Application", KeyVal); 

		//Create the associations
		RegCU.setValue(CUKeyName + "/.", m_ClassId); 		//Extension class
		RegCU.setValue(ExtKeyName + "/Progid", m_ClassId);					//Explorer FileExt association

		if (RegCU.status() == QSettings::NoError)
			count++; 
	}

	return count; 
}

bool WinFileAssoc::GetRegisteredExtensions( const QStringList& extensionsToCheck, QStringList& registeredExtensions )
{

	if (QSysInfo::WindowsVersion == QSysInfo::WV_VISTA)
	{
		return VistaGetDefaultApps(extensionsToCheck, registeredExtensions); 
	}

	QSettings RegCR ("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
	QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);

	if (RegCR.status() != QSettings::NoError)
		return false; 

	if (RegCU.status() != QSettings::NoError)
		return false; 

	foreach(const QString& fileExtension, extensionsToCheck)
	{
		bool bRegistered = false; 
		//Check the explorer extension (Always use this program to open this kind of file...)
		QString CurClassId = RegCU.value(QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/.%1/Progid").arg(fileExtension)).toString(); 

		if (CurClassId.size())	//Registered with Open With... ?
		{
			bRegistered = (CurClassId == m_ClassId);
		}
		else
		{
			//No classId means that no associations exists in Default Programs or Explorer
			
			//Check the default per-user association 
			bRegistered = RegCU.value("Software/Classes/." + fileExtension + "/.").toString() == m_ClassId; 

			//Finally, check the system-wide association
			if (!bRegistered)
				bRegistered = RegCR.value("." + fileExtension + "/.").toString() == m_ClassId;
		}

		if (bRegistered)
			registeredExtensions.append(fileExtension); 
	}
	
	return true; 
}

int WinFileAssoc::RestoreFileAssociations(const QStringList& fileExtensions)
{
	if (QSysInfo::WindowsVersion == QSysInfo::WV_VISTA)
		return 0; //Not supported by the API

	QSettings RegCR ("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
	QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);

	if (!RegCU.isWritable() || RegCU.status() != QSettings::NoError)
		return 0; 

	int count = 0; 
	foreach(const QString& fileExtension, fileExtensions)
	{
		QString ExtKeyName = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/.%1").arg(fileExtension);
		QString OldProgId = RegCU.value(ExtKeyName + "/MPlayer_Backup_ProgId").toString(); 
		QString OldApp  = RegCU.value(ExtKeyName + "/MPlayer_Backup_Application").toString(); 
		QString OldClassId = RegCU.value("Software/Classes/." + fileExtension + "/MPlayer_Backup").toString(); 

		//Restore old explorer ProgId
		if (!OldProgId.isEmpty() && OldProgId != m_ClassId)
			RegCU.setValue(ExtKeyName + "/Progid", OldProgId);
		else
			RegCU.remove(ExtKeyName + "/Progid"); 

		//Restore old explorer Application 
		if (!OldApp.isEmpty() && OldApp != m_ClassId)
			RegCU.setValue(ExtKeyName + "/Application", OldApp); 
		else
			RegCU.remove(ExtKeyName + "/Application");

		//Restore old association for current user
		if (!OldClassId.isEmpty() && OldClassId != m_ClassId)
			RegCU.setValue("Software/Classes/." + fileExtension + "/.", OldClassId); 
		else
			RegCU.remove("Software/Classes/." + fileExtension + "/."); 	//No old association with this extension, it's better to remove it entirely

		//Remove our keys:
		//CurrentUserClasses/.ext/MPlayerBackup
		//Explorer: Backup_Application and Backup_ProgId
		RegCU.remove("Software/Classes/." + fileExtension + "/MPlayer_Backup"); 
		RegCU.remove(ExtKeyName + "/MPlayer_Backup_Application"); 
		RegCU.remove(ExtKeyName + "/MPlayer_Backup_ProgId"); 
	}
	return count; 
}

//Note: It's better to create the classId from the installation program.
bool WinFileAssoc::CreateClassId(const QString& executablePath, const QString& friendlyName)
{
	QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);
	if (!RegCU.isWritable() || RegCU.status() != QSettings::NoError)
		return false; 

	QString appPath = executablePath;
	appPath.replace('/', '\\'); //Explorer gives 'Access Denied' if we write the path with forward slashes to the registry

	QString classId = "Software/Classes/" + m_ClassId; 
	//Add our ProgId to the HKCR classes
	RegCU.setValue(classId + "/shell/open/FriendlyAppName", friendlyName);
	RegCU.setValue(classId + "/shell/open/command/.", QString("\"%1\" \"%2\"").arg(appPath, "%1"));
	RegCU.setValue(classId + "/DefaultIcon/.", QString("\"%1\",1").arg(appPath));
	//Add "Enqueue" command
	RegCU.setValue(classId + "/shell/enqueue/.", QObject::tr("Enqueue in SMPlayer"));
	RegCU.setValue(classId + "/shell/enqueue/command/.", QString("\"%1\" -add-to-playlist \"%2\"").arg(appPath, "%1"));
	return true; 
}

//Called when no associations exist. Note: It's better to do this in the Setup program.
bool WinFileAssoc::RemoveClassId()
{
	QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);

	if (!RegCU.isWritable() || RegCU.status() != QSettings::NoError)
		return false; 

	QString classId = "Software/Classes/" + m_ClassId; 

	RegCU.remove(classId + "/shell/open/FriendlyAppName");
	RegCU.remove(classId + "/shell/open/command/.");
	RegCU.remove(classId + "/shell/enqueue/command/.");
	RegCU.remove(classId + "/shell/enqueue/.");
	RegCU.remove(classId + "/DefaultIcon/.");
	RegCU.remove(classId);
	return true; 
}

//Windows Vista specific implementation
//Add libole32.a library if compiling with mingw.
//In smplayer.pro, under win32{ :
//	LIBS += c:/mingw/lib/libole32.a
#ifdef WIN32
#include <windows.h>

#if !defined(IApplicationAssociationRegistration)

typedef enum tagASSOCIATIONLEVEL
{
	AL_MACHINE,
	AL_EFFECTIVE,
	AL_USER
} ASSOCIATIONLEVEL;

typedef enum tagASSOCIATIONTYPE
{
	AT_FILEEXTENSION,
	AT_URLPROTOCOL,
	AT_STARTMENUCLIENT,
	AT_MIMETYPE
} ASSOCIATIONTYPE;

MIDL_INTERFACE("4e530b0a-e611-4c77-a3ac-9031d022281b")
IApplicationAssociationRegistration : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryCurrentDefault(LPCWSTR pszQuery,
		ASSOCIATIONTYPE atQueryType,
		ASSOCIATIONLEVEL alQueryLevel,
		LPWSTR *ppszAssociation) = 0;
	virtual HRESULT STDMETHODCALLTYPE QueryAppIsDefault(LPCWSTR pszQuery,
		ASSOCIATIONTYPE atQueryType,
		ASSOCIATIONLEVEL alQueryLevel,
		LPCWSTR pszAppRegistryName,
		BOOL *pfDefault) = 0;
	virtual HRESULT STDMETHODCALLTYPE QueryAppIsDefaultAll(ASSOCIATIONLEVEL alQueryLevel,
		LPCWSTR pszAppRegistryName,
		BOOL *pfDefault) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetAppAsDefault(LPCWSTR pszAppRegistryName,
		LPCWSTR pszSet,
		ASSOCIATIONTYPE atSetType) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetAppAsDefaultAll(LPCWSTR pszAppRegistryName) = 0;
	virtual HRESULT STDMETHODCALLTYPE ClearUserAssociations( void) = 0;
};
#endif

static const CLSID CLSID_ApplicationAssociationReg = {0x591209C7,0x767B,0x42B2,{0x9F,0xBA,0x44,0xEE,0x46,0x15,0xF2,0xC7}};
static const IID   IID_IApplicationAssociationReg  = {0x4e530b0a,0xe611,0x4c77,{0xa3,0xac,0x90,0x31,0xd0,0x22,0x28,0x1b}};

int WinFileAssoc::VistaSetAppsAsDefault(const QStringList& fileExtensions)
{
	IApplicationAssociationRegistration* pAAR;
	HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationReg,
		NULL, CLSCTX_INPROC, IID_IApplicationAssociationReg,	(void**)&pAAR);

	int count = 0; 
	if (SUCCEEDED(hr) && (pAAR != NULL))
	{
		foreach(const QString& fileExtension, fileExtensions)
		{
			hr = pAAR->SetAppAsDefault((const WCHAR*)m_AppName.utf16(),
				(const WCHAR*)QString("." + fileExtension).utf16(),
				AT_FILEEXTENSION);

			if (SUCCEEDED(hr)) 
				count++; 
		}
		pAAR->Release(); 
	}
	return count; 
}

bool WinFileAssoc::VistaGetDefaultApps(const QStringList &extensions, QStringList& registeredExt)
{
	IApplicationAssociationRegistration* pAAR;

	HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationReg,
		NULL, CLSCTX_INPROC, IID_IApplicationAssociationReg,	(void**)&pAAR);

	if (SUCCEEDED(hr) && (pAAR != NULL))
	{
		foreach(const QString& fileExtension, extensions)
		{
			BOOL bIsDefault = FALSE; 
			hr = pAAR->QueryAppIsDefault((const WCHAR*)QString("." + fileExtension).utf16(),
				AT_FILEEXTENSION,
				AL_EFFECTIVE,
				(const WCHAR*)m_AppName.utf16(),
				&bIsDefault);
			if (SUCCEEDED(hr) && bIsDefault)
			{
				registeredExt.append(fileExtension); 
			}
		}

		pAAR->Release();
		return true;
	}
	return false;
}
#else
bool WinFileAssoc::VistaGetDefaultApps(const QStringList &extensions, QStringList& registeredExt)
{
	return false; 
}

int WinFileAssoc::VistaSetAppsAsDefault(const QStringList& extensions)
{
	return 0; 
}
#endif

