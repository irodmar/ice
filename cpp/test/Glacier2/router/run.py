#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2004 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

import os, sys

for toplevel in [".", "..", "../..", "../../..", "../../../.."]:
    toplevel = os.path.normpath(toplevel)
    if os.path.exists(os.path.join(toplevel, "config", "TestUtil.py")):
        break
else:
    raise "can't find toplevel directory!"

sys.path.append(os.path.join(toplevel, "config"))
import TestUtil

router = os.path.join(toplevel, "bin", "glacier2router")

command = router + TestUtil.clientServerOptions + \
          r' --Glacier2.RouterIdentity="abc/def"' + \
          r' --Glacier2.Client.Endpoints="default -p 12347 -t 30000"' + \
          r' --Glacier2.Server.Endpoints="tcp -h 127.0.0.1"' \
          r' --Glacier2.CryptPasswords="' + toplevel + r'/test/Glacier2/router/passwords"'

print "starting router...",
starterPipe = os.popen(command)
TestUtil.getServerPid(starterPipe)
TestUtil.getAdapterReady(starterPipe)
print "ok"

name = os.path.join("Glacier2", "router")

TestUtil.mixedClientServerTest(name)

# We run the test again, to check whether the glacier router can
# handle multiple clients.
TestUtil.mixedClientServerTest(name)

print "shutting down router...",
TestUtil.killServers() # TODO: Graceful shutdown.
print "ok"

starterStatus = starterPipe.close()

if starterStatus:
    TestUtil.killServers()
    #sys.exit(1) # TODO: Uncomment when when we have graceful shutdown.

sys.exit(0)
