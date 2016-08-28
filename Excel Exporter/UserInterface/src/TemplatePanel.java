import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import javax.swing.BoxLayout;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumnModel;

public class TemplatePanel extends Panel
{
    public TemplatePanel()
    {
        readFile = null;
        
        setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
        
        subjectTableModel = new SubjectTableModel();
        
        subjectTable = new JTable(subjectTableModel);
        TableColumnModel subjectTableColumnModel = subjectTable.getColumnModel();
        subjectTableColumnModel.getColumn(0).setMaxWidth(30);
        subjectTableColumnModel.getColumn(2).setMaxWidth(80);
        
        subjectTableScrollPane = new JScrollPane(subjectTable, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        add(subjectTableScrollPane);
        
        stateSubjectTableModel = new StateSubjectTableModel();
        
        stateSubjectTable = new JTable(stateSubjectTableModel);
        TableColumnModel stateSubjectTableColumModel = stateSubjectTable.getColumnModel();
        stateSubjectTableColumModel.getColumn(0).setMaxWidth(30);
                
        stateSubjectTableScrollPane = new JScrollPane(stateSubjectTable, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        add(stateSubjectTableScrollPane);
    }
    
    @Override
    public void readFromFile(File file) throws IOException
    {
        readFile = file;
        
        Path filePath = Paths.get(file.getPath());
        byte[] fileBytes = Files.readAllBytes(filePath);
        String fileContent = new String(fileBytes);
        String[] splittedFileContent = fileContent.split("=");
        
        subjectTableModel.setContent(splittedFileContent[0]);
        stateSubjectTableModel.setContent(splittedFileContent[1]);
    }
  
    @Override
    public void writeToFile(File file) throws IOException
    {
        String fileContent = subjectTableModel.getContent() + "=" + stateSubjectTableModel.getContent();
        
        Path filePath = Paths.get(file.getPath());
        Files.write(filePath, fileContent.getBytes());
    }
    
    @Override
    public File getReadFile()
    {
        return readFile;
    }
    
    @Override
    public boolean hasChanges()
    {
        return subjectTableModel.hasChanges() || stateSubjectTableModel.hasChanges();
    }
    
    @Override
    public void clearContent()
    {
        subjectTableModel.clearContent();
        stateSubjectTableModel.clearContent();
    }
    
    public SubjectTableModel getSubjectTableModel() 
    {
        return subjectTableModel;
    }
    
    public StateSubjectTableModel getStateSubjectTableModel()
    {
        return stateSubjectTableModel;
    }
    
    private File readFile;
        
    private JTable subjectTable;
    private JScrollPane subjectTableScrollPane;
    private SubjectTableModel subjectTableModel;
    
    private JTable stateSubjectTable;
    private JScrollPane stateSubjectTableScrollPane;
    private StateSubjectTableModel stateSubjectTableModel;
}
