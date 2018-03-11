#include <lamure/vt/Observable.h>

namespace vt {

    void Observable::observe(event_type event, Observer *observer) {
        auto eventIter = _events.find(event);

        if (eventIter == _events.end()) {
            eventIter = _events.insert(pair<event_type, set<Observer*>>(event, set<Observer*>())).first;
        }

        auto observerSet = eventIter->second;

        auto observerIter = observerSet.find(observer);

        if(observerIter != observerSet.end()){
            return;
        }

        eventIter->second.insert(observer);
    }

    void Observable::unobserve(event_type event, Observer *observer){
        auto eventIter = _events.find(event);

        if (eventIter == _events.end()) {
            return;
        }

        auto observerSet = eventIter->second;

        auto observerIter = observerSet.find(observer);

        if(observerIter == observerSet.end()){
            return;
        }

        //(*observerIter) = nullptr;
        observerSet.erase(observer);
    }

    void Observable::inform(event_type event) {
        Observer** observers;
        size_t len;

        {
            auto iter = _events.find(event);

            if (iter == _events.end()) {
                return;
            }

            len = iter->second.size();
            observers = new Observer*[len];

            size_t i = 0;

            for(auto ptr : iter->second){
                observers[i++] = ptr;
            }
        }

        for (size_t i = 0; i < len; ++i) {
            observers[i]->inform(event, this);
        }
    }

}