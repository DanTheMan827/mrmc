/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "RssManager.h"
#include "addons/AddonInstaller.h"
#include "addons/AddonManager.h"
#include "dialogs/GUIDialogYesNo.h"
#include "filesystem/File.h"
#include "interfaces/Builtins.h"
#include "profiles/ProfilesManager.h"
#include "settings/lib/Setting.h"
#include "settings/Settings.h"
#include "threads/SingleLock.h"
#include "utils/log.h"
#include "utils/RssReader.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"

using namespace std;
using namespace XFILE;

CRssManager::CRssManager()
{
  m_bActive = false;
}

CRssManager::~CRssManager()
{
  Stop();
}

CRssManager& CRssManager::GetInstance()
{
  static CRssManager sRssManager;
  return sRssManager;
}

void CRssManager::OnSettingsLoaded()
{
  Load();
}

void CRssManager::OnSettingsUnloaded()
{
  Clear();
}

bool CRssManager::OnSettingChanging(const CSetting *setting)
{
  if (setting == NULL)
    return false;

  const std::string &settingId = setting->GetId();
  if (settingId == CSettings::SETTING_LOOKANDFEEL_RSSHOST ||
      settingId == CSettings::SETTING_LOOKANDFEEL_RSSINTERVAL ||
      settingId == CSettings::SETTING_LOOKANDFEEL_RSSRTL)
  {
    Reload();
  }
  return true;
}

void CRssManager::Start()
 {
   m_bActive = true;
}

void CRssManager::Stop()
{
  CSingleLock lock(m_critical);
  m_bActive = false;
  for (unsigned int i = 0; i < m_readers.size(); i++)
  {
    if (m_readers[i].reader)
      delete m_readers[i].reader;
  }
  m_readers.clear();
}

bool CRssManager::Load()
{
  RssSet set;
  m_mapRssUrls.clear();
  set.rtl = CSettings::GetInstance().GetBool(CSettings::SETTING_LOOKANDFEEL_RSSRTL);
  set.url.push_back(CSettings::GetInstance().GetString(CSettings::SETTING_LOOKANDFEEL_RSSHOST));
  set.interval.push_back(CSettings::GetInstance().GetInt(CSettings::SETTING_LOOKANDFEEL_RSSINTERVAL));
  m_mapRssUrls.insert(make_pair(1,set));
  return true;
}

bool CRssManager::Reload()
{
  Stop();
  if (!Load())
    return false;
  Start();

  return true;
}

void CRssManager::Clear()
{
  CSingleLock lock(m_critical);
  m_mapRssUrls.clear();
}

// returns true if the reader doesn't need creating, false otherwise
bool CRssManager::GetReader(int controlID, int windowID, IRssObserver* observer, CRssReader *&reader)
{
  CSingleLock lock(m_critical);
  // check to see if we've already created this reader
  for (unsigned int i = 0; i < m_readers.size(); i++)
  {
    if (m_readers[i].controlID == controlID && m_readers[i].windowID == windowID)
    {
      reader = m_readers[i].reader;
      reader->SetObserver(observer);
      reader->UpdateObserver();
      return true;
    }
  }
  // need to create a new one
  READERCONTROL readerControl;
  readerControl.controlID = controlID;
  readerControl.windowID = windowID;
  reader = readerControl.reader = new CRssReader;
  m_readers.push_back(readerControl);
  return false;
}
