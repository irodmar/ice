// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

import Filesystem.*;

public final class FileI extends PersistentFile
{
    public
    FileI()
    {
        _destroyed = false;
    }

    public
    FileI(Ice.Identity id)
    {
        _id = id;
        _destroyed = false;
    }

    public synchronized String
    name(Ice.Current current)
    {
        if(_destroyed)
        {
            throw new Ice.ObjectNotExistException(current.id, current.facet, current.operation);
        }

        return nodeName;
    }

    public void
    destroy(Ice.Current current)
        throws PermissionDenied
    {
        synchronized(this)
	{
	    if(_destroyed)
	    {
		throw new Ice.ObjectNotExistException(current.id, current.facet, current.operation);
	    }
	    _destroyed = true;
	}

        parent.removeNode(nodeName);
        _evictor.remove(_id);
    }

    public synchronized String[]
    read(Ice.Current current)
    {
        if(_destroyed)
        {
            throw new Ice.ObjectNotExistException(current.id, current.facet, current.operation);
        }

        return (String[])text.clone();
    }

    public synchronized void
    write(String[] text, Ice.Current current)
        throws GenericError
    {
        if(_destroyed)
        {
            throw new Ice.ObjectNotExistException(current.id, current.facet, current.operation);
        }

        this.text = text;
    }

    public static Freeze.Evictor _evictor;
    public Ice.Identity _id;
    private boolean _destroyed;
}