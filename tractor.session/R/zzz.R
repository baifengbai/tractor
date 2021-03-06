.onLoad <- function (libname, pkgname)
{
    tractorHome <- Sys.getenv("TRACTOR_HOME")
    if (imageFileExists(file.path(tractorHome, "share", "tractor", "mni", "brain")))
        .StandardBrainPath <<- file.path(tractorHome, "share", "tractor", "mni")
    
    if (is.null(getOption("tractorViewer")))
    {
        viewer <- tolower(Sys.getenv("TRACTOR_VIEWER"))
        if (isTRUE(viewer %in% .Viewers))
            options(tractorViewer=as.vector(viewer))
        else
            options(tractorViewer="tractor")
    }
    
    # Assume path separator (.Platform$file.sep) is "/"
    registerPathHandler("^(.+)@(\\w+)(/(\\w+))?", function(path,index=1) {
        # The match has to have been done just before calling this function (although this is not thread-safe)
        groups <- groups(ore.lastmatch())
        nGroups <- sum(!is.na(groups))
        if (nGroups < 2)
            return (NULL)
        else if (nGroups < 3)
            attachMriSession(groups[1])$getImageFileNameByType(groups[2], index=index)
        else
            attachMriSession(groups[1])$getImageFileNameByType(groups[4], groups[2], index=index)
    })
}
