#include "FileWatcher.h"

namespace gameplay {

FileWatcherBase::FileWatcherBase()
{
    ///_fileWatcherEventManager = new EventManager<efsw::WatchID>();

    // create instance of FileWatcher and start watching.
    _fileWatcher = new efsw::FileWatcher();
    _fileWatcher->watch();
}

FileWatcherBase::~FileWatcherBase()
{
    ///SAFE_DELETE(_fileWatcherEventManager);
}

void FileWatcherBase::addDirectory(const char* directory, bool recursive)
{
    GP_ASSERT(_fileWatcher);

    // add a watch directory task
    efsw::WatchID watchID = _fileWatcher->addWatch(directory, this, recursive);

    // record watched directory
    _directories[directory] = watchID;
}

void FileWatcherBase::handleFileAction( efsw::WatchID watchid,
                                        const std::string& dir,
                                        const std::string& filename,
                                        efsw::Action action, std::string oldFilename)
{
    switch( action )
    {
    case efsw::Actions::Add:
        print("FileWatcher: %s%s has been added.\n", dir.c_str(), filename.c_str());
        break;
    case efsw::Actions::Delete:
        print("FileWatcher: %s%s has been deleted.\n", dir.c_str(), filename.c_str());
        break;
    case efsw::Actions::Modified:
        print("FileWatcher: %s%s has been modified.\n", dir.c_str(), filename.c_str());
        break;
    case efsw::Actions::Moved:
        print("FileWatcher: %s%s has been moved from %s.\n", dir.c_str(), filename.c_str(), oldFilename.c_str());
        break;
    default:
        print("FileWatcher: unknow action.");
    }

    // send event
    /*EventParams args;
    args.set("Action", action);
    args.set("Directory", dir);
    args.set("Filename", filename);
    args.set("OldFilename", oldFilename);
    _fileWatcherEventManager->onEvent(watchid, args);*/
}



}
