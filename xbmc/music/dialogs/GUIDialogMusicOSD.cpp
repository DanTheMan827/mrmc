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

#include "GUIDialogMusicOSD.h"
#include "guilib/GUIWindowManager.h"
#include "input/Key.h"
#include "input/InputManager.h"
#include "GUIUserMessages.h"
#include "settings/Settings.h"
#include "addons/GUIWindowAddonBrowser.h"

#define CONTROL_STOP_BUTTON      603

CGUIDialogMusicOSD::CGUIDialogMusicOSD(void)
    : CGUIDialog(WINDOW_DIALOG_MUSIC_OSD, "MusicOSD.xml")
{
  m_loadType = KEEP_IN_MEMORY;
}

CGUIDialogMusicOSD::~CGUIDialogMusicOSD(void)
{
}

bool CGUIDialogMusicOSD::OnMessage(CGUIMessage &message)
{
  switch (message.GetMessage())
  {
  case GUI_MSG_CLICKED:
    {
      unsigned int iControl = message.GetSenderId();
      if (iControl == CONTROL_STOP_BUTTON)
      {
        // Close the window
        Close();
      }
      return true;
    }
    break;
  }
  return CGUIDialog::OnMessage(message);
}

bool CGUIDialogMusicOSD::OnAction(const CAction &action)
{
  switch (action.GetID())
  {
  case ACTION_SHOW_OSD:
    Close();
    return true;

  default:
    break;
  }

  return CGUIDialog::OnAction(action);
}

void CGUIDialogMusicOSD::FrameMove()
{
  if (m_autoClosing)
  {
    // check for movement of mouse or a submenu open
    if (CInputManager::GetInstance().IsMouseActive())
      // extend show time by original value
      SetAutoClose(m_showDuration);
  }
  CGUIDialog::FrameMove();
}
