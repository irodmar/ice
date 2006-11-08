// **********************************************************************
//
// Copyright (c) 2003-2006 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef TOPIC_I_H
#define TOPIC_I_H

#include <IceUtil/RecMutex.h>
#include <IceStorm/IceStormInternal.h>
#include <IceStorm/PersistentTopicMap.h>
#include <IceStorm/PersistentUpstreamMap.h>
#include <IceStorm/Event.h>
#include <list>

namespace IceStorm
{

class Instance;
typedef IceUtil::Handle<Instance> InstancePtr;

class Subscriber;
typedef IceUtil::Handle<Subscriber> SubscriberPtr;

class TopicI : public TopicInternal
{
public:

    TopicI(const InstancePtr&, const std::string&, const LinkRecordDict&, const std::string&, const std::string&);

    virtual std::string getName(const Ice::Current&) const;
    virtual Ice::ObjectPrx getPublisher(const Ice::Current&) const;
    virtual void subscribe(const QoS&, const Ice::ObjectPrx&, const Ice::Current&);
    virtual Ice::ObjectPrx subscribeAndGetPublisher(const QoS&, const Ice::ObjectPrx&, const Ice::Current&);
    virtual void unsubscribe(const Ice::ObjectPrx&, const Ice::Current&);
    virtual TopicLinkPrx getLinkProxy(const Ice::Current&);
    virtual void link(const TopicPrx&, Ice::Int, const Ice::Current&);
    virtual void unlink(const TopicPrx&, const Ice::Current&);
    virtual void unlinkByName(const std::string&, const Ice::Current&);
    virtual LinkInfoSeq getLinkInfoSeq(const Ice::Current&) const;
    virtual void destroy(const Ice::Current&);

    virtual void linkNotification(const std::string&, const TopicUpstreamLinkPrx&, const Ice::Current&);
    virtual void unlinkNotification(const std::string&, const TopicUpstreamLinkPrx&, const Ice::Current&);

    // Internal methods
    bool destroyed() const;

    void reap();

private:

    void removeSubscriber(const Ice::ObjectPrx&);
    friend class PublisherProxyI;
    friend class TopicLinkI;
    void publish(bool, const EventSeq&);

    //
    // Immutable members.
    //
    const InstancePtr _instance;
    const std::string _name; // The topic name

    /*const*/ Ice::ObjectPrx _publisherPrx;
    /*const*/ TopicLinkPrx _linkPrx;

    // Set of subscribers.
    IceUtil::Mutex _subscribersMutex;
    //
    // We keep a vector of subscribers since the optimized behaviour
    // should be publishing events, not searching through the list of
    // subscribers for a particular subscriber. I tested
    // vector/list/map and although there was little difference vector
    // was the fastest of the three.
    //
    std::vector<SubscriberPtr> _subscribers;

    // Set of subscribers that have encountered an error.
    IceUtil::Mutex _errorMutex;
    std::list<SubscriberPtr> _error;

    const Freeze::ConnectionPtr _connection;

    // The set of downstream topics.
    IceUtil::RecMutex _topicRecordMutex;
    PersistentTopicMap _topics;
    IceStorm::LinkRecordDict _topicRecord;

    // The set of upstream topics.
    IceUtil::RecMutex _upstreamRecordMutex;
    PersistentUpstreamMap _upstream;
    IceStorm::TopicUpstreamLinkPrxSeq _upstreamRecord;

    // Protected by either _upstreamRecordMutex or _topicRecordMutex
    bool _destroyed; // Has this Topic been destroyed?
};
typedef IceUtil::Handle<TopicI> TopicIPtr;

} // End namespace IceStorm

#endif
