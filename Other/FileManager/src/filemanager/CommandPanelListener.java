package filemanager;

public interface CommandPanelListener 
{
    void onFileCopyPerformed();
    void onFileMovePerformed();
    void onNewFolderPerformed();
    void onFileDeletePerformed();
}
