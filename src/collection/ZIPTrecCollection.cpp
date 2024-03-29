//
// Copyright(C) 2005--2006 Institute of Computing Tech, Chinese Academy of Sciences. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: �����(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2007/1/9
//

#include "collection/ZIPTrecCollection.h"
#include "utility/ZIPFile.h"
#include "utility/dirent.h"
#include "utility/Config.h"

using namespace firtex::utility;

namespace firtex
{
	namespace collection
	{		
		CZIPTrecCollection::CZIPTrecCollection(const tchar* dir)
			:CTrecCollection(dir)
		{			
		}

		
		CZIPTrecCollection::CZIPTrecCollection(const tchar* dir,CIndexWriter* pWriter)
			:CTrecCollection(dir,pWriter)
		{			
		}
		CZIPTrecCollection::~CZIPTrecCollection(void)
		{
		}
		void CZIPTrecCollection::indexDocs(const tchar* directory)
		{
			CZIPFile zipFile;

			DIR* dir = opendir(directory);
			if ( dir != NULL )
			{
				struct dirent* fl;

				struct fileStat buf;
				char path[FX_MAX_PATH];
				_tcscpy(path,directory);
				_tcscat(path,PATH_DELIMITERA);
				tchar* pathP = path + _tcslen(path);
				fl = readdir(dir);
				while ( fl != NULL )
				{
					if ( (_tcscmp(fl->d_name, ".")) && (_tcscmp(fl->d_name, "..")) ) 
					{
						pathP[0]=0;
						strcat(pathP,fl->d_name);
						int32_t ret = fileStat(path,&buf);
						if ( buf.st_mode & S_IFDIR ) 
						{
							indexDocs(path );
						}
						else
						{
							if(stopRequested())
								break;
							FIRTEX_CLOG(level::info) << _T("Processing file: ") << path <<FIRTEX_ENDL;
							if(zipFile.open(path) == false)
							{
								FIRTEX_CLOG(level::warn) << _T("Open file: ") << path << _T(" failed.") << FIRTEX_ENDL;
							}
							else 
							{
								///TODO:
							}
							//parseFile(path);						
						}
					}
					fl = readdir(dir);
				}
				closedir(dir);
			}
			else
			{
				FIRTEX_CLOG(level::warn) << _T("Open directory failed: ") << directory << FIRTEX_ENDL;
			}
		}
	}
}
