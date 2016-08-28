package filemanager;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.KeyStroke;

public class MainPanel extends JPanel implements CommandPanelListener, DiskPanelListener
{
    public MainPanel(JFrame mainFrame)
    {
        super();
        this.mainFrame = mainFrame;
        
        this.diskPanel1 = new DiskPanel("Panel1");
        this.diskPanel1.setPanelListener(this);
        
        this.diskPanel2 = new DiskPanel("Panel2");
        this.diskPanel2.setPanelListener(this);
        
        this.commandPanel = new CommandPanel();
        this.commandPanel.setPanelListener(this);
  
        this.add(this.diskPanel1);
        this.add(this.diskPanel2);
        this.add(this.commandPanel);
        
        this.sourceDiskPanel = diskPanel1;
        this.targetDiskPanel = diskPanel2;
        
        InputMap inputMap = this.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_F5, 0), "Copy");
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_F6, 0), "Move");
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_F7, 0), "NewFolder");
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_F8, 0), "Delete");
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0), "About");
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, InputEvent.ALT_MASK), "FileProperties");
        
        ActionMap actionMap = this.getActionMap();
        actionMap.put("Copy", new CopyFileAction());
        actionMap.put("Move", new MoveFileAction());
        actionMap.put("NewFolder", new NewFolderAction());
        actionMap.put("Delete", new DeleteFileAction());
        actionMap.put("About", new ShowAboutDialogAction());
        actionMap.put("FileProperties", new ShowFilePropertiesDialogAction());
    }
    public void setShowHiddenFiles(boolean showHiddenFiles)
    {
        this.sourceDiskPanel.setShowHiddenFiles(showHiddenFiles);
        this.targetDiskPanel.setShowHiddenFiles(showHiddenFiles);
    }
    public boolean showHiddenFiles()
    {
        return this.sourceDiskPanel.showHiddenFiles();
    }
    public void setShowExt(boolean showExt)
    {
        this.sourceDiskPanel.setShowExt(showExt);
        this.targetDiskPanel.setShowExt(showExt);
    }
    public boolean showExt()
    {
        return this.sourceDiskPanel.showExt();
    }
    
    public void setShowSize(boolean showSize)
    {
        this.sourceDiskPanel.setShowSize(showSize);
        this.targetDiskPanel.setShowSize(showSize);
    }
    public boolean showSize()
    {
        return this.sourceDiskPanel.showSize();
    }
    
    public void setShowDate(boolean showDate)
    {
        this.sourceDiskPanel.setShowDate(showDate);
        this.targetDiskPanel.setShowDate(showDate);
    }
    
    public boolean showDate()
    {
        return this. sourceDiskPanel.showDate();
    }
    @Override
    public void onFileCopyPerformed()
    {   
        File targetDirectory = this.targetDiskPanel.getCurrentDirectory();
        
        File[] selectedFiles = this.sourceDiskPanel.getSelectedFiles();
        if (selectedFiles.length > 0)
        {
            if (selectedFiles.length == 1)
            {
                File sourceFile = selectedFiles[0];
                Path sourcePath = sourceFile.toPath();
                String sourceName = sourceFile.getName();
                
                File targetFile = new File(targetDirectory, sourceName);
                String oldTargetPathString = targetFile.getAbsolutePath();
                String newTargetPathString = (String)JOptionPane.showInputDialog(null, String.format("Copy \"%s\" to", sourceName), "File Manager",
                    JOptionPane.QUESTION_MESSAGE, null, null, oldTargetPathString);
                if ((newTargetPathString != null) && (!newTargetPathString.isEmpty()))
                {
                    if (!newTargetPathString.equals(oldTargetPathString))
                        targetFile = new File(newTargetPathString);
                    
                    Path targetPath = targetFile.toPath();
                    try 
                    {
                        Files.copy(sourcePath, targetPath);
                        this.targetDiskPanel.reloadFiles();
                    } 
                    catch (IOException ex) 
                    {
                        System.out.println(ex.getLocalizedMessage());
                    }
                } 
            }
            else 
            {
                String oldTargetPathString = targetDirectory.getAbsolutePath();
                String newTargetPathString = (String)JOptionPane.showInputDialog(null, String.format("Copy %d file(s) to", selectedFiles.length), 
                        "File Manager", JOptionPane.QUESTION_MESSAGE, null, null, oldTargetPathString);
                if ((newTargetPathString != null) && (!newTargetPathString.isEmpty()))
                {
                    if (!newTargetPathString.equals(oldTargetPathString))
                        targetDirectory = new File(newTargetPathString);
                    
                    for (int index = 0; index < selectedFiles.length; ++index)
                    {
                       File sourceFile = selectedFiles[index];
                       Path sourcePath = sourceFile.toPath();
                       String sourceName = sourceFile.getName();
                       
                       File targetFile = new File(targetDirectory, sourceName);
                       Path targetPath = targetFile.toPath();
                       
                       try 
                       {
                           Files.copy(sourcePath, targetPath);
                       }
                       catch (IOException ex)
                       {
                           System.out.println(ex.getLocalizedMessage());
                       }
                    }
                    this.targetDiskPanel.reloadFiles();
                }
                
            }
        }
        else 
        {
            JOptionPane.showMessageDialog(null, "No files selected!", "File Manager", JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    @Override
    public void onFileMovePerformed()
    {
        File targetDirectory = this.targetDiskPanel.getCurrentDirectory();
        
        File[] selectedFiles = this.sourceDiskPanel.getSelectedFiles();
        if (selectedFiles.length > 0)
        {
            if (selectedFiles.length == 1)
            {
                File sourceFile = selectedFiles[0];
                Path sourcePath = sourceFile.toPath();
                String sourceName = sourceFile.getName();
                
                File targetFile = new File(targetDirectory, sourceName);
                String oldTargetPathString = targetFile.getAbsolutePath();
                String newTargetPathString = (String)JOptionPane.showInputDialog(null, String.format("Rename/Move \"%s\" to", sourceName), "File Manager",
                        JOptionPane.QUESTION_MESSAGE, null, null, oldTargetPathString);
                if ((newTargetPathString != null) && (!newTargetPathString.isEmpty()))
                {
                    if (!newTargetPathString.equals(oldTargetPathString))
                        targetFile = new File(newTargetPathString);
                    Path targetPath = targetFile.toPath();
                    try
                    {
                        Files.move(sourcePath, targetPath);
                    }
                    catch (IOException ex)
                    {
                        System.out.println(ex.getLocalizedMessage());
                    }
                    this.targetDiskPanel.reloadFiles();
                    this.sourceDiskPanel.reloadFiles();
                }
            }
            else
            {
                String oldTargetPathString = targetDirectory.getAbsolutePath();
                String newTargetPathString = (String)JOptionPane.showInputDialog(null, String.format("Remane/Move %d file(s) to", selectedFiles.length), 
                        "File Manager", JOptionPane.QUESTION_MESSAGE, null, null, oldTargetPathString);
                if ((newTargetPathString != null) && (!newTargetPathString.isEmpty()))
                {
                    if (!newTargetPathString.equals(oldTargetPathString))
                        targetDirectory = new File(newTargetPathString);
                    for (int index = 0; index < selectedFiles.length; ++index)
                    {
                        File sourceFile = selectedFiles[index];
                        Path sourcePath = sourceFile.toPath();
                        String sourceName = sourceFile.getName();
                        
                        File targetFile = new File(targetDirectory, sourceName);
                        Path targetPath = targetFile.toPath();
                        try
                        {
                            Files.move(sourcePath, targetPath);
                        }
                        catch (IOException ex)
                        {
                            System.out.println(ex.getLocalizedMessage());
                        }
                    }
                    this.targetDiskPanel.reloadFiles();
                    this.sourceDiskPanel.reloadFiles();
                }
            } 
        }
        else 
        {
            JOptionPane.showMessageDialog(null, "No files selected!", "File Manager", JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    @Override
    public void onNewFolderPerformed()
    {
        String newFolderName = (String)JOptionPane.showInputDialog(null, "New folder (directory)", "File Manager",
                JOptionPane.QUESTION_MESSAGE, null, null, "");
        if ((newFolderName != null) && (!newFolderName.isEmpty()))
        {
            File newFolderFile = new File(this.sourceDiskPanel.getCurrentDirectory(), newFolderName);
            Path newFolderPath = newFolderFile.toPath();
            try
            {
                Files.createDirectory(newFolderPath);
            }
            catch (IOException ex)
            {
                System.out.println(ex.getLocalizedMessage());
            }
            this.sourceDiskPanel.reloadFiles();
        } 
    }
    
    @Override
    public void onFileDeletePerformed()
    {
        File[] selectedFiles = this.sourceDiskPanel.getSelectedFiles();
        if (selectedFiles.length > 0)
        {
            DeleteFileVisitor deleteFileVisitor = new DeleteFileVisitor();
            int userOption = 1;
            if (selectedFiles.length == 1)
            {
                if (selectedFiles[0].isDirectory())
                {
                    userOption = JOptionPane.showConfirmDialog(null, "Are you sure that you want to move this folder to the Recycle Bin?", 
                            "Delete Folder", JOptionPane.YES_NO_OPTION);
                }
                else 
                {
                    userOption = JOptionPane.showConfirmDialog(null, "Are you sure that you want to move this file to the Recycle Bin?", 
                            "Delete File", JOptionPane.YES_NO_OPTION);
                }
            }
            else 
            {
                userOption = JOptionPane.showConfirmDialog(null, String.format("Are you sure that you want to move these %d items to the Recycle Bin", selectedFiles.length), 
                        "Delete Multiple Items", JOptionPane.YES_NO_OPTION);
            }
            if (userOption == 0)
            {
                for (int index = 0; index < selectedFiles.length; ++index)
                {
                    File selectedFile = selectedFiles[index];
                    Path selectedPath = selectedFile.toPath();
                    try 
                    {
                        Files.walkFileTree(selectedPath, deleteFileVisitor);
                    } 
                    catch (IOException ex) 
                    {
                        System.out.println(ex.getLocalizedMessage());
                    }
                }
                this.sourceDiskPanel.reloadFiles();    
            }
        }
        else 
        {
            JOptionPane.showMessageDialog(null, "No files selected!", "File Manager", JOptionPane.INFORMATION_MESSAGE);
        }
    }
    
    @Override
    public void onDiskPanelSelected(DiskPanel diskPanel)
    {
        if (this.sourceDiskPanel != diskPanel)
        {
            this.sourceDiskPanel.setShowSelectedFile(false);
            this.sourceDiskPanel.refreshSelectedFile();
            this.targetDiskPanel = this.sourceDiskPanel;
            
            this.sourceDiskPanel = diskPanel;
            this.sourceDiskPanel.setShowSelectedFile(true);
            this.sourceDiskPanel.refreshSelectedFile();
        }
    }
    private class DeleteFileVisitor implements FileVisitor<Path>
    {
        @Override
        public FileVisitResult preVisitDirectory(Path directory, BasicFileAttributes attrs) throws IOException
        {
            return FileVisitResult.CONTINUE;
        }
        @Override
        public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException
        {
            Files.delete(file);
            return FileVisitResult.CONTINUE;
        }
        @Override
        public FileVisitResult visitFileFailed(Path file, IOException ex) throws IOException
        {
            return FileVisitResult.CONTINUE;
        }
        @Override
        public FileVisitResult postVisitDirectory(Path directory, IOException ex) throws IOException
        {
            if (ex == null)
            {
                Files.delete(directory);
                return FileVisitResult.CONTINUE;
            }
            else 
            {
                throw ex;
            }
        }
    }
    private class CopyFileAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event) 
        {
            onFileCopyPerformed();
        }   
    }
    private class MoveFileAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event) 
        {
            onFileMovePerformed();
        }   
    }
    private class NewFolderAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event) 
        {
            onNewFolderPerformed();
        }   
    }
    private class DeleteFileAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event) 
        {
            onFileDeletePerformed();
        }   
    }
    private class ShowAboutDialogAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            AboutDialog aboutDialog = new AboutDialog(mainFrame);
            
            Dimension frameSize = mainFrame.getSize();
            Point frameLocation = mainFrame.getLocation();
            Dimension dialogSize = aboutDialog.getSize();
            Point dialogLocation = new Point(frameLocation.x + (frameSize.width - dialogSize.width) / 2, 
                    frameLocation.y + (frameSize.height - dialogSize.height) / 2); 
            
            aboutDialog.setLocation(dialogLocation);
            aboutDialog.setVisible(true);
        }
    }
    private class ShowFilePropertiesDialogAction extends AbstractAction
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            File[] selectedFiles = sourceDiskPanel.getSelectedFiles();
            FilePropertiesDialog propertiesDialog = new FilePropertiesDialog(mainFrame, selectedFiles, sourceDiskPanel, targetDiskPanel);
            propertiesDialog.setVisible(true);
        } 
    }
            
    private DiskPanel diskPanel1;
    private DiskPanel diskPanel2;
    private CommandPanel commandPanel;
    private DiskPanel sourceDiskPanel;
    private DiskPanel targetDiskPanel;
    private JFrame mainFrame;
}
