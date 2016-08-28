package filemanager;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.table.AbstractTableModel;

public class DiskPanel extends JPanel
{
    public DiskPanel(String panelName)
    {
        super();
        this.panelName = panelName;
        this.setLayout(new BorderLayout());
        this.hardDrives = File.listRoots();
        
        int selectedDiskIndex = 0;
        File currentFolderFile = this.hardDrives[selectedDiskIndex];
        
        String currentFolderPath = this.loadCurrentFolder();
        if (currentFolderPath != null)
        {
            selectedDiskIndex = this.getDiskIndex(currentFolderPath);
            currentFolderFile = new File(currentFolderPath);
        }
        
        this.disksComboBox = new JComboBox<>();
        for (int index = 0; index < this.hardDrives.length; ++index)
            this.disksComboBox.addItem(this.getDiskName(this.hardDrives[index]));
        this.disksComboBox.setSelectedIndex(selectedDiskIndex);
        this.disksComboBox.addActionListener(new DiskComboBoxListener());
        
        File selectedDisk = this.hardDrives[selectedDiskIndex];
        this.diskInfoLabel = new JLabel("Disk Info");
        this.diskInfoLabel.setText(getDiskInfo(selectedDisk));
        
        JPanel diskInfoGroupPanel = new JPanel();
        diskInfoGroupPanel.add(this.disksComboBox);
        diskInfoGroupPanel.add(this.diskInfoLabel);
        JPanel diskInfoPanel = new JPanel(new BorderLayout());
        diskInfoPanel.add(diskInfoGroupPanel, BorderLayout.WEST);
        this.add(diskInfoPanel, BorderLayout.NORTH);
        
        this.folderPathLabel = new JLabel("");
        JPanel folderPathPanel = new JPanel(new BorderLayout());
        folderPathPanel.add(this.folderPathLabel, BorderLayout.WEST);
        this.add(folderPathPanel, BorderLayout.CENTER);
        this.updateCurrentFolder(currentFolderFile);
        
        this.filesTableModel = new FilesTableModel(currentFolderFile, false);
        this.filesTable = new FilesTable(this.filesTableModel);
        this.filesTable.addMouseListener(new FilesTableMouseListener(this));
        JScrollPane filesScrollPane = new JScrollPane(this.filesTable);
        this.add(filesScrollPane, BorderLayout.SOUTH);
        
        this.panelListener = null;
    }
    public File getCurrentDirectory()
    {
        return this.filesTableModel.getRoot();
    }
    public File[] getSelectedFiles()
    {
        int[] selectedRows = this.filesTable.getSelectedRows();
        File[] selectedFiles = new File[selectedRows.length];
        for (int index = 0; index < selectedRows.length; ++index)
        {
            selectedFiles[index] = this.filesTableModel.getFile(selectedRows[index]);
        }
        return selectedFiles;
    }
    public void reloadFiles()
    {
        this.filesTableModel.setRoot(this.getCurrentDirectory());
    }
    public void setPanelListener(DiskPanelListener panelListener)
    {
        this.panelListener = panelListener;
    }
    public DiskPanelListener getPanelListener()
    {
        return this.panelListener;
    }
    public void setShowSelectedFile(boolean showSelectedFile)
    {
        this.filesTable.setShowSelectedFile(showSelectedFile);
    }
    public boolean showSelectedFile()
    {
        return this.filesTable.showSelectedFile();
    }
    public void setShowExt(boolean showExt)
    {
        this.filesTableModel.setShowExt(showExt);
    }
    public boolean showExt()
    {
        return this.filesTableModel.showExt();
    }
    public void setShowSize(boolean showSize)
    {
        this.filesTableModel.setShowSize(showSize);
    }
    public boolean showSize()
    {
        return this.filesTableModel.showSize();
    }
    public void setShowDate(boolean  showDate)
    {
        this.filesTableModel.setShowDate(showDate);
    }
    public boolean showDate()
    {
        return this.filesTableModel.showDate();
    }
    public void refreshSelectedFile()
    {
        this.filesTable.repaint();
    }
    public void setPanelName(String panelName)
    {
        this.panelName = panelName;
    }
    public String getPanelName()
    {
        return this.panelName;
    }
    public void setShowHiddenFiles(boolean showHiddenFiles)
    {
        this.filesTableModel.setShowHiddenFiles(showHiddenFiles);
        this.reloadFiles();
    }
    public boolean showHiddenFiles()
    {
        return this.filesTableModel.showHiddenFiles();
    }
    private class DiskComboBoxListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            File selectedDisk = hardDrives[disksComboBox.getSelectedIndex()];
            diskInfoLabel.setText(getDiskInfo(selectedDisk));
            filesTableModel.setRoot(selectedDisk);
            updateCurrentFolder(selectedDisk);
        }
    }
    private class FilesTableModel extends AbstractTableModel
    {
        public FilesTableModel(File root, boolean showHiddenFiles)
        {
            this.showHiddenFiles = showHiddenFiles;
            this.showExt = true;
            this.showSize = true;
            this.showDate = true;
            this.revalidate();
            this.setRoot(root);
        }
        @Override
        public Class<?> getColumnClass(int columnIndex)
        {
            return String.class;
        }
        @Override
        public int getColumnCount()
        {
            return this.columnGetters.length;
        }
        @Override
        public String getColumnName(int columnIndex)
        {
            return this.columnGetters[columnIndex].get();
        }
        @Override
        public int getRowCount()
        {
            int rowCount = this.getNumFiles();
            if (this.rootParent != null)
                ++rowCount;
            return rowCount;
        }
        @Override
        public Object getValueAt(int rowIndex, int columnIndex)
        {
            File file = this.getFile(rowIndex);
            return this.properyGetters[columnIndex].get(file, rowIndex);
        }
        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex)
        {
            return false;
        }
        @Override
        public void setValueAt(Object value, int rowIndex, int columnIndex)
        {
            
        }
        public void setRoot(File root)
        {
            this.root = root;
            this.rootParent = root.getParentFile();
            this.files = root.listFiles(new TableFileFilter());
            this.fireTableDataChanged();
        }
        public void revalidate()
        {
            int columnSize = 1;
            if (this.showExt)
                ++columnSize;
            if (this.showSize)
                ++columnSize;
            if (this.showDate)
                ++columnSize;
            
            this.columnGetters = new ColumnNameGetter[columnSize];
            this.properyGetters = new FilePropertyGetter[columnSize];
            
            int index = 0;
            this.columnGetters[index] = new ColumnNameGetter("Name");
            this.properyGetters[index] = new FileNameGetter();
            ++index;
            
            if (this.showExt)
            {
                this.columnGetters[index] = new ColumnNameGetter("Extension");
                this.properyGetters[index] = new FileExtGetter();
                ++index;
            }
            if (this.showSize)
            {
                this.columnGetters[index] = new ColumnNameGetter("Size");
                this.properyGetters[index] = new FileSizeGetter();
                ++index;
            }
            if (this.showDate)
            {
                this.columnGetters[index] = new ColumnNameGetter("Date");
                this.properyGetters[index] = new FileDateGetter();
            }  
        }
        public File getRoot()
        {
            return this.root;
        }
        public boolean showHiddenFiles()
        {
            return this.showHiddenFiles;
        }
        public void setShowHiddenFiles(boolean showHiddenFiles)
        {
            this.showHiddenFiles = showHiddenFiles;
        }
        public int getNumFiles()
        {
            return this.files.length;
        }
        public File getFile(int index)
        {
            if (this.rootParent != null)
            {
                if (index == 0)
                    return this.rootParent;
                else 
                    --index;
            }
            return this.files[index];
        }
        public void setShowExt(boolean showExt)
        {
            this.showExt = showExt;
            this.revalidate();
            this.fireTableStructureChanged();
        }
        public boolean showExt()
        {
            return this.showExt;
        }
        
        public void setShowSize(boolean showSize)
        {
            this.showSize = showSize;
            this.revalidate();
            this.fireTableStructureChanged();
        }
        
        public boolean showSize()
        {
            return this.showSize;
        }
        
        public void setShowDate(boolean showDate)
        {
            this.showDate = showDate;
            this.revalidate();
            this.fireTableStructureChanged();
        }
        public boolean showDate()
        {
            return this.showDate;
        }
        private class TableFileFilter implements FileFilter
        {
            @Override
            public boolean accept(File file)
            {
                if (file.isHidden())
                    return showHiddenFiles;
                return true;
            }
        }
        private class ColumnNameGetter
        {
            public ColumnNameGetter(String name)
            {
                this.name = name;
            }
            public String get()
            {
                return this.name;
            }
            private String name;
        }
        private class FilePropertyGetter
        {
            public Object get(File file, int rowIndex)
            {
                return null;
            }
        }
        private class FileNameGetter extends FilePropertyGetter
        {
            @Override
            public Object get(File file, int rowIndex)
            {
                if ((rowIndex == 0) && (rootParent != null))
                    return "[..]";
                return "[" + FileUtils.getFileName(file.getName()) + "]";
            }
        }
        private class FileExtGetter extends FilePropertyGetter
        {
            @Override
            public Object get(File file, int rowIndex)
            {
                if ((rowIndex == 0) && (rootParent != null))
                    return "";
                return FileUtils.getFileExtension(file.getName());
            }
        }
        private class FileSizeGetter extends FilePropertyGetter
        {
            @Override
            public Object get(File file, int rowIndex)
            {
                if (file.isDirectory())
                    return "<DIR>";
                return String.format("%d", file.length());
            }
        }
        private class FileDateGetter extends FilePropertyGetter
        {
            @Override
            public Object get(File file, int rowIndex)
            {
                Date lastModifiedDate = new Date(file.lastModified());
                SimpleDateFormat dateFormat = new SimpleDateFormat("MM/dd/yyyy hh:mm");
                return dateFormat.format(lastModifiedDate);
            }
        }
        private File root;
        private File rootParent;
        private File[] files;
        private boolean showHiddenFiles;
        private boolean showExt;
        private boolean showSize;
        private boolean showDate;
        private ColumnNameGetter[] columnGetters;
        private FilePropertyGetter[] properyGetters;
    }   
    private String getDiskInfo(File disk)
    {
        long diskFreeSpace = disk.getFreeSpace() / 1024;
        long diskTotalSpace = disk.getTotalSpace() / 1024;
        return String.format("%dk of %dk free", diskFreeSpace, diskTotalSpace);
    }
    private String getDiskName(File disk)
    {
        String filePath = disk.getAbsolutePath();
        return filePath.substring(0, filePath.indexOf(':'));
    }
    private void updateCurrentFolder(File file)
    {
        String filePath = file.getAbsolutePath();
        this.folderPathLabel.setText(filePath);
        this.saveCurrentFolder(filePath);
    }
    private String getSaveFolderPath()
    {
        return String.format("settings%s.txt", this.panelName);
    }
    private void saveCurrentFolder(String filePath)
    {
        try 
        {
            FileWriter fileWriter = new FileWriter(this.getSaveFolderPath());
            
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
            bufferedWriter.write(filePath);
            bufferedWriter.close();
        } 
        catch (IOException ex) 
        {
            System.out.println(ex.getLocalizedMessage());
        }
    }
    private String loadCurrentFolder()
    {
        try 
        {
            FileReader fileReader = new FileReader(this.getSaveFolderPath());
            
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String filePath = bufferedReader.readLine();
            bufferedReader.close();
            
            return filePath;
        } 
        catch (FileNotFoundException ex) 
        {
            System.out.println(ex.getLocalizedMessage());
        } 
        catch (IOException ex) 
        {
            System.out.println(ex.getLocalizedMessage());
        }
        return null;    
    }
    private int getDiskIndex(String filePath)
    {
        for (int index = 0; index < this.hardDrives.length; ++index)
        {
            File hardDrive = this.hardDrives[index];
            String hardDrivePath = hardDrive.getAbsolutePath();
            if (filePath.startsWith(hardDrivePath))
                return index;
        }
        return -1;
    }
    private class FilesTableMouseListener implements MouseListener
    {
        public FilesTableMouseListener(DiskPanel diskPanel)
        {
            this.diskPanel = diskPanel;
        }
        @Override
        public void mouseClicked(MouseEvent event)
        {
            if (event.getClickCount() == 2)
            {
                int selectedRow = filesTable.getSelectedRow();
                File selectedFile = filesTableModel.getFile(selectedRow);
                if (selectedFile.isDirectory())
                {
                    filesTableModel.setRoot(selectedFile);
                    updateCurrentFolder(selectedFile);
                }
                else if (selectedFile.canExecute())
                {
                    try 
                    {
                        Desktop.getDesktop().open(selectedFile);
                    } 
                    catch (IOException ex) 
                    {
                        System.out.println(ex.getMessage());
                    }
                }
            }
        }
        @Override
        public void mousePressed(MouseEvent event)
        {
            if (panelListener != null)
                panelListener.onDiskPanelSelected(this.diskPanel);
        }
        @Override
        public void mouseReleased(MouseEvent event)
        {}
        @Override
        public void mouseEntered(MouseEvent event)
        {}
        @Override
        public void mouseExited(MouseEvent event)
        {}
        private DiskPanel diskPanel;
    }
    private JComboBox<String> disksComboBox;
    private JLabel diskInfoLabel;
    private JLabel folderPathLabel;
    private FilesTable filesTable;
    private FilesTableModel filesTableModel;
    private File[] hardDrives;
    private DiskPanelListener panelListener;
    private String panelName;
}
