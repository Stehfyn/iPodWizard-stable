////////////////////////////////////////////////////////////////////////////////
// This source file is part of the ZipArchive library source distribution and
// is Copyrighted 2000 - 2006 by Tadeusz Dracz (tdracz@artpol-software.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// For the licensing details refer to the License.txt file.
//
// Web Site: http://www.artpol-software.com
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZipFile.h"
#include "ZipPlatform.h"
#include "ZipException.h"

using namespace ZipPlatform;

bool ZipPlatform::DirectoryExists(LPCTSTR lpszDir)
{
	CZipString sz;
	if (!GetCurrentDirectory(sz))
		return false;
	if (!ChangeDirectory(lpszDir))
		return false;
	ChangeDirectory(sz);
	return true;
}

bool ZipPlatform::ForceDirectory(LPCTSTR lpDirectory)
{
	ASSERT(lpDirectory);
	CZipString szDirectory = lpDirectory;
	szDirectory.TrimRight(CZipPathComponent::m_cSeparator);
	CZipPathComponent zpc(szDirectory);
	if ((zpc.GetFilePath().Compare((LPCTSTR)szDirectory)) == 0 ||
		(FileExists(szDirectory) == -1))
		return true;
	if (!ForceDirectory(zpc.GetFilePath()))
		return false;
	if (!CreateDirectory(szDirectory))
		return false;
	return true;
}

bool ZipPlatform::GetFileSize(LPCTSTR lpszFileName, ZIP_U32_U64& dSize)
{
	CZipFile f;
	if (!f.Open(lpszFileName, CZipFile::modeRead | CZipFile::shareDenyWrite, false))
		return false;
	bool ret;
	try
	{
		ZIP_FILE_USIZE size = f.GetLength();
		// the file may be too large if zip64 is not enabled
		ret = size <= ZIP_U32_U64(-1);
		if (ret)
			dSize = (ZIP_U32_U64)size;
	}
#ifdef ZIP_ARCHIVE_MFC
	catch(CZipBaseException* e)
	{
		e->Delete();
		ret = false;
	}
#else
	catch(CZipBaseException e)
	{
		ret = false;
	}
#endif

	try
	{
		f.Close();
	}
#ifdef ZIP_ARCHIVE_MFC
	catch(CZipBaseException* e)
	{
		e->Delete();
	}
#else
	catch(CZipBaseException e)
	{
	}
#endif

	return ret;	
}


