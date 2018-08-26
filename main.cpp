/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**  Modified by: MDCSAENZ for Assignment 4
****************************************************************************/

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/Log.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

int main( int argc, char** argv )
{
  std::string file = "settings.cfg";

  try
  {
    FIX::SessionSettings settings( file );

    Application application;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::ScreenLogFactory logFactory( settings );
    FIX::SocketInitiator initiator( application, storeFactory, settings, logFactory );

    initiator.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    application.run();
    initiator.stop();

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    return 1;
  }
}
