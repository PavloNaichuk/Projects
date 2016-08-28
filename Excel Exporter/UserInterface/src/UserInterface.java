import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class UserInterface
{    
    public static void main(String[] args) 
    {
        EventQueue.invokeLater(new Runnable()
        {
             @Override
             public void run()
             {
                 //створюємо фрейм
                 ReportFrame frame = new ReportFrame();
                 frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                 frame.setVisible(true);
             }
        });
    } 
}

class ReportFrame extends JFrame 
{
    public ReportFrame()
    {
        setTitle("Звіт з навчання");
        setSize(1024, 600);
        
        currentPanel = null;
        templatePanel = null;
        projectPanel = null;
        
        JMenuBar menuBar = new JMenuBar();
        setJMenuBar(menuBar);
        
        JMenu fileMenu = new JMenu("Файл");
        menuBar.add(fileMenu);
        
        JMenuItem newTemplateItem = new JMenuItem("Новий шаблон...");
        newTemplateItem.addActionListener(new NewTemplateAction());
        fileMenu.add(newTemplateItem);
        
        JMenuItem openTemplateItem = new JMenuItem("Відкрити шаблон...");
        openTemplateItem.addActionListener(new OpenTemplateAction());
        fileMenu.add(openTemplateItem);
        
        fileMenu.addSeparator();
        
        JMenuItem newProjectItem = new JMenuItem("Новий проект...");
        newProjectItem.addActionListener(new NewProjectAction());
        fileMenu.add(newProjectItem);
        
        JMenuItem openProjectItem = new JMenuItem("Відкрити проект...");
        openProjectItem.addActionListener(new OpenProjectAction());
        fileMenu.add(openProjectItem);
        
        fileMenu.addSeparator();
       
        JMenuItem saveItem = new JMenuItem("Зберегти");
        saveItem.addActionListener(new SaveAction());
        fileMenu.add(saveItem);
        
        JMenuItem saveAsItem = new JMenuItem("Зберегти як...");
        saveAsItem.addActionListener(new SaveAsAction());
        fileMenu.add(saveAsItem);
        
        JMenuItem exportToExcelItem = new JMenuItem("Експортувати в excel...");
        exportToExcelItem.addActionListener(new ExportToExcelAction());
        fileMenu.add(exportToExcelItem);
        
        fileMenu.addSeparator();
         
        JMenuItem closeItem = new JMenuItem ("Закрити");
        closeItem.addActionListener(new CloseAction());
        fileMenu.add(closeItem);
    }
    
    private class NewTemplateAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            if (templatePanel == null)
                templatePanel = new TemplatePanel();
            else
                templatePanel.clearContent();
            
            if (currentPanel != null)
                remove(currentPanel);
            
            currentPanel = templatePanel;
            add(currentPanel);
            revalidate();
            repaint();
        }
    }
    
    private class OpenTemplateAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            JFileChooser fileChooser = new JFileChooser();
            if (fileChooser.showOpenDialog(ReportFrame.this) == JFileChooser.APPROVE_OPTION)
            {
                try 
                {
                    if (templatePanel == null)
                        templatePanel = new TemplatePanel();
                    else
                        templatePanel.clearContent();

                    if (currentPanel != null)
                        remove(currentPanel);

                    currentPanel = templatePanel;
                    add(currentPanel);
                    revalidate();
                    repaint();
                    
                    File file = fileChooser.getSelectedFile();
                    templatePanel.readFromFile(file);
                } 
                catch (IOException ex) 
                {
                    Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
    
    private class NewProjectAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            JFileChooser fileChooser = new JFileChooser();
            if (fileChooser.showOpenDialog(ReportFrame.this) == JFileChooser.APPROVE_OPTION)
            {
                try 
                {
                    if (templatePanel == null)
                        templatePanel = new TemplatePanel();
                    
                    File file = fileChooser.getSelectedFile();
                    templatePanel.readFromFile(file);
                    
                    if (currentPanel != null)
                        remove(currentPanel);
                    
                    SubjectTableModel subjectTableModel = templatePanel.getSubjectTableModel();
                    StateSubjectTableModel stateSubjectTableModel = templatePanel.getStateSubjectTableModel();
                    projectPanel = new ProjectPanel(subjectTableModel, stateSubjectTableModel);

                    currentPanel = projectPanel;
                    add(currentPanel);
                    revalidate();
                    repaint();
                } 
                catch (IOException ex) 
                {
                    Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }       
        }       
    }
    
    private class OpenProjectAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            JFileChooser fileChooser = new JFileChooser();
            if (fileChooser.showOpenDialog(ReportFrame.this) == JFileChooser.APPROVE_OPTION)
            {
                try 
                {
                    if (projectPanel == null)
                        projectPanel = new ProjectPanel();

                    if (currentPanel != null)
                        remove(currentPanel);

                    currentPanel = projectPanel;
                    add(currentPanel);
                    revalidate();
                    repaint();

                    File file = fileChooser.getSelectedFile();
                    currentPanel.readFromFile(file);
                }
                catch (IOException ex) 
                {
                    Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }       
    }
    
    private class SaveAction implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            try 
            {
                currentPanel.writeToFile(currentPanel.getReadFile());
            } 
            catch (IOException ex)
            {
                Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    private class SaveAsAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            JFileChooser fileChooser = new JFileChooser();
            if (fileChooser.showSaveDialog(ReportFrame.this) == JFileChooser.APPROVE_OPTION)
            {
                try 
                {
                    File file = fileChooser.getSelectedFile();
                    currentPanel.writeToFile(file);
                } 
                catch (IOException ex) 
                {
                    Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }
     
    private class ExportToExcelAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            if (currentPanel == projectPanel)
            {
                JFileChooser fileChooser = new JFileChooser();
                if (fileChooser.showSaveDialog(ReportFrame.this) == JFileChooser.APPROVE_OPTION)
                {
                    try 
                    {
                        File tempExportFile = new File("Temp Export Project.nauproj");                                        
                        projectPanel.writeToFile(tempExportFile);

                        File outputFile = fileChooser.getSelectedFile();
                        ExcelExporter.export(outputFile, tempExportFile, projectPanel.getSelectedStudentIndex());

                        tempExportFile.delete();
                    } 
                    catch (IOException | InterruptedException ex)
                    {
                        Logger.getLogger(ReportFrame.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
            }
        }
    }
    
    private class CloseAction implements ActionListener 
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            System.exit(0);
        }       
    }
        
    private TemplatePanel templatePanel;
    private ProjectPanel projectPanel;
    private Panel currentPanel;
}