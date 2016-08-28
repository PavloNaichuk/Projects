package filemanager;

public class FileUtils 
{
    public static String getFileName(String fullFileName)
    {
        int extensionSeparatorIndex = fullFileName.lastIndexOf('.');
        if (extensionSeparatorIndex == -1)
            return fullFileName;
        return fullFileName.substring(0, extensionSeparatorIndex);
    }
    public static String getFileExtension(String fullFileName)
    {
        int extensionSeparatorIndex = fullFileName.lastIndexOf('.');
        if (extensionSeparatorIndex == -1)
            return "";
        return fullFileName.substring(extensionSeparatorIndex + 1, fullFileName.length());
    } 
}
