#pragma once
/*
 *      Copyright (C) 2015 MrMC
 *      https://github.com/MrMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MrMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */



#include <string>
#include "FileItem.h"
#include "Application.h"


class CSubtitleSearch
{
public:
  CSubtitleSearch(){}
  virtual ~CSubtitleSearch() {}
  virtual bool SubtitleSearch(const std::string &path,const std::string strLanguages,
                              const std::string preferredLanguage,CFileItemList &subtitlesList) { return false; }
  virtual std::string ModuleName() { return ""; }
  virtual bool Download(const CFileItem *subItem,std::vector<std::string> &items) { return false; }
  virtual void ChangeUserPass() {}
  
};
