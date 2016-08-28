package filemanager;

import java.awt.GridLayout;
import java.awt.event.ActionListener;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.attribute.FileTime;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

public class FilePropertiesDialog extends JDialog 
{
    public FilePropertiesDialog(JFrame owner, File[] files, DiskPanel sourceDiskPanel, DiskPanel targetDiskPanel)
    {
        super(owner, buildTitle(files), true);
        if (files.length == 1)
            this.getContentPane().add(new FilePropertiesPanel(this, files[0], sourceDiskPanel, targetDiskPanel));
        this.pack();
        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }
    private static String buildTitle(File[] files)
    {
        if (files.length == 1)
        {
            String fileName = FileUtils.getFileName(files[0].getName());
            return String.format("%s Properties", fileName);
        }
        return "Files Properties";
    }
    private class FilePropertiesPanel extends JPanel
    {
        public FilePropertiesPanel(JDialog parentDialog, File file, DiskPanel sourceDiskPanel, DiskPanel targetDiskPanel)
        {
            super();
            //this.setLayout(new BoxLayout());
            this.file = file;
            this.sourceDiskPanel = sourceDiskPanel;
            this.targetDiskPanel = targetDiskPanel;
            this.parentDialog = parentDialog;
            
            BasicFileAttributes attributes = null;
            try 
            {
                attributes = Files.readAttributes(file.toPath(), BasicFileAttributes.class);
            }
            catch (IOException ex)
            {}
            FileTime creationTime = attributes.creationTime();
            FileTime modifiedTime = attributes.lastModifiedTime();
            FileTime accessedTime = attributes.lastAccessTime();
            
            String parentPath = file.getParent();
            if (parentPath == null)
                parentPath = "";
            
            String fullFileName = file.getName();
            long fileSize = file.length();
            
            SimpleDateFormat dateFormat = new SimpleDateFormat("EEEEEE, MMMMM d, yyyy, hh:mm:ss a"); 
            ButtonListener buttonListener = new ButtonListener();
            
            JPanel panel1 = new JPanel();
            this.fileNameField = new JTextField(FileUtils.getFileName(fullFileName));
            FileNameListener fileNameListener = new FileNameListener();
            this.fileNameField.getDocument().addDocumentListener(fileNameListener);
            panel1.add(this.fileNameField);
            this.add(panel1);
            
            JPanel panel2 = new JPanel();
            JPanel leftPanel = new JPanel(new GridLayout(7, 1));
            leftPanel.add(new JLabel("Type of file:"));
            leftPanel.add(new JLabel("Location:"));
            leftPanel.add(new JLabel("Size:"));
            leftPanel.add(new JLabel("Created:"));
            leftPanel.add(new JLabel("Modified:"));
            leftPanel.add(new JLabel("Accessed:"));
            leftPanel.add(new JLabel("Attributes:"));
            
            JPanel panel3 = new JPanel();
            
            this.readOnlyCheckBox = new JCheckBox("Read-only");
            this.readOnlyCheckBox.setSelected(file.canWrite() == false);
            this.readOnlyCheckBox.setActionCommand("ReadOnly");
            this.readOnlyCheckBox.addActionListener(buttonListener);
            panel3.add(this.readOnlyCheckBox);
            
            this.hiddenCheckBox = new JCheckBox("Hidden");
            this.hiddenCheckBox.setSelected(file.isHidden());
            this.hiddenCheckBox.setEnabled(false);
            panel3.add(this.hiddenCheckBox);
            
            JPanel rightPanel = new JPanel(new GridLayout(6, 1));
            rightPanel.add(new JLabel(FileUtils.getFileExtension(fullFileName)));
            rightPanel.add(new JLabel(parentPath));
            rightPanel.add(new JLabel(String.format("%.3f KB (%d bytes)", (float)fileSize / (float)1024, fileSize)));
            rightPanel.add(new JLabel(dateFormat.format(new Date(creationTime.toMillis()))));
            rightPanel.add(new JLabel(dateFormat.format(new Date(modifiedTime.toMillis()))));
            rightPanel.add(new JLabel(dateFormat.format(new Date(accessedTime.toMillis()))));
            rightPanel.add(panel3);
            
            panel2.add(leftPanel);
            panel2.add(rightPanel);
            this.add(panel2);
            
            JPanel panel4 = new JPanel();
            
            JButton okButton = new JButton("OK");
            okButton.setActionCommand("OK");
            okButton.addActionListener(buttonListener);
            panel4.add(okButton);
            
            JButton cancelButton = new JButton("Cancel");
            cancelButton.setActionCommand("Cancel");
            cancelButton.addActionListener(buttonListener);
            panel4.add(cancelButton);
            
            this.applyButton = new JButton("Apply");
            this.applyButton.setEnabled(false);
            this.applyButton.setActionCommand("Apply");
            this.applyButton.addActionListener(buttonListener);
            panel4.add(this.applyButton);
            
            this.add(panel4);
        } 
        private class ButtonListener implements ActionListener
        {
            @Override
            public void actionPerformed(ActionEvent event)
            {
                String actionCommand = event.getActionCommand();
                if (actionCommand.equals("OK"))
                {
                    this.applyChanges();
                    parentDialog.setVisible(false);
                    parentDialog.dispose();
                }
                else if (actionCommand.equals("Cancel"))
                {
                    parentDialog.setVisible(false);
                    parentDialog.dispose();
                }
                else if (actionCommand.equals("Apply"))
                {
                    this.applyChanges();
                    applyButton.setEnabled(false);    
                }
                else if (actionCommand.equals("ReadOnly"))
                {
                    applyButton.setEnabled(true);
                }
            }
            private void applyChanges()
            {
                file.setWritable(!readOnlyCheckBox.isSelected());
                
                String oldFileFullName = file.getName();
                String newFileName = fileNameField.getText() + "." + FileUtils.getFileExtension(oldFileFullName);
                Path oldFilePath = file.toPath();
                File newFile = new File(file.getParentFile(), newFileName);
                Path newFilePath = newFile.toPath();
                try 
                {
                    Files.move(oldFilePath, newFilePath);
                    sourceDiskPanel.reloadFiles();
                    targetDiskPanel.reloadFiles();
                } 
                catch (IOException ex) 
                {
                    System.out.println(ex.getMessage());
                } 
            }
        }
        private class FileNameListener implements DocumentListener
        {
            @Override
            public void insertUpdate(DocumentEvent event)
            {
                applyButton.setEnabled(true);
            }
            @Override
            public void removeUpdate(DocumentEvent event)
            {
                String fileName = fileNameField.getText();
                applyButton.setEnabled(!fileName.isEmpty());
            }
            @Override
            public void changedUpdate(DocumentEvent event){}
        }
        private JTextField fileNameField;
        private JButton applyButton;
        private JCheckBox readOnlyCheckBox;
        private JCheckBox hiddenCheckBox;
        private File file;
        private DiskPanel targetDiskPanel;
        private DiskPanel sourceDiskPanel;
        private JDialog parentDialog;
    }
    private class FilesPropertiesPanel extends JPanel
    {
        
    }
}
