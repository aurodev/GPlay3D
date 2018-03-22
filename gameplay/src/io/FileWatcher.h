#pragma once

#include <efsw/efsw.hpp>
#include "../Singleton.h"
#include "../Base.h"
#include "eventManager/EventManager.h"
#include "eventManager/BaseEventData.h"
#include "../Platform.h"

namespace gameplay {

/**
 * FileWatcher event.
 */

using FileWatcherEventRef = std::shared_ptr<class FileWatcherEvent>;

class FileWatcherEvent : public EventData
{
public:

    static EventType E_FILEWATCHER;

    struct WatchData
    {
        unsigned int action;
        std::string directory;
        std::string filename;
        std::string oldFilename;
    };
    WatchData _data;

    static FileWatcherEventRef create(WatchData data)
    {
        return FileWatcherEventRef(new FileWatcherEvent(data));
    }

    EventDataRef copy() {}
    const char* getName() const {}
    EventType getEventType() const { return E_FILEWATCHER; }
    virtual ~FileWatcherEvent() {}

private:

    explicit FileWatcherEvent(WatchData data) :
        EventData(Platform::getAbsoluteTime())
      , _data(data)
    {
    }
};


/**
 * Listens to files and directories and dispatch event to notify listeners that files and directories were changed.
 *
 * Note : Don't use this class directly but use the FileWatcher singleton version.
 *
 * Possible events are :
 * 1. Add, Sent when a file is created or renamed.
 * 2. Delete, sent when a file is deleted or renamed.
 * 3. Modified, sent when a file is deleted or renamed.
 * 4. Moved, sent when a file is moved.
 *
 */
class FileWatcherBase : public efsw::FileWatchListener
{
public:

    /**
     * Constructor.
     */
    FileWatcherBase();


    /**
     * Destructor
     */
    ~FileWatcherBase();

    /**
     * @brief Add a directory to watch.
     * When a directory is watched, files operation in this directory (add, delete, modify, move)
     * will generates a FileWatcherEvent event.
     *
     * @param directory: path to directory
     * @param recursive: recursive watch
     */
    void addDirectory(const char* directory, bool recursive);


private:

    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename,
                           efsw::Action action, std::string oldFilename = "" );


    efsw::FileWatcher* _fileWatcher; // file watcher core implementation
};

/// define a singleton version of FileWatcherBase.
typedef Singleton<FileWatcherBase> FileWatcher;

}
