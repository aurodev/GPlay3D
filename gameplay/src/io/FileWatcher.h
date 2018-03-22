#pragma once

#include <efsw/efsw.hpp>
#include "eventManager/EventManager.h"
#include "../Singleton.h"
#include "../Base.h"

namespace gameplay {

/**
 * Listens to files and directories and dispatches events to notify the listener of files and directories changes.
 *
 * Note : Don't use this class directly but use the FileWatcher singleton version.
 *
 * Possible events are :
 * 1. Add, Sent when a file is created or renamed.
 * 2. Delete, sent when a file is deleted or renamed.
 * 3. Modified, sent when a file is deleted or renamed.
 * 4. Moved, sent when a file is moved.
 *
 * How to use :
 * - First, use addDirectory to add a directory to watch.
 * - Secondly, add some listeners where to dispatches events.
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
     * When a directory is watched, files operation in this directory (add, delete, modify, move) will generates
     * events emitted to registered listeners (see addListener).
     *
     * @param directory: path to directory
     * @param recursive: recursive watch
     */
    void addDirectory(const char* directory, bool recursive);

    /**
     * @brief Add a listener when file operation occurs in the specified directory.
     * @param directory where file events occurs (directory must be previously added to FileWatcher see addDirectory)
     *
     * @param ptr: listener instance
     * @param func: listener's method to call
     */
    //template <class T>
    //void addListener(const char* directory, T* ptr, void (T::*func)(EventParams& args));

private:

    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename,
                           efsw::Action action, std::string oldFilename = "" );


    efsw::FileWatcher* _fileWatcher;                        // file watcher core implementation
    std::map<std::string, efsw::WatchID> _directories;      // list of all watched directories
    //EventManager<efsw::WatchID>* _fileWatcherEventManager;  // own event manager
};

/// define a singleton version of FileWatcherBase.
typedef Singleton<FileWatcherBase> FileWatcher;


/*template <class T>
void FileWatcherBase::addListener(const char* directory, T* ptr, void (T::*func)(EventParams& args))
{
    if(_directories.count(directory) > 0)
    {
        efsw::WatchID watchID = _directories.at(directory);
        _fileWatcherEventManager->registerEvent(watchID, ptr, func);
    }
    else
    {
        GP_WARN("Directory %s is not currently watched.", directory);
    }
}*/

}
