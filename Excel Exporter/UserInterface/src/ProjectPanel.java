import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import javax.swing.BoxLayout;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.TableColumnModel;

public class ProjectPanel extends Panel
{
    public ProjectPanel()
    {
        initPanel(new SubjectTableModel(), new StateSubjectTableModel());
    }
    
    public ProjectPanel(SubjectTableModel subjectTableModel, StateSubjectTableModel stateSubjectTableModel)
    {
        initPanel(subjectTableModel, stateSubjectTableModel);
    }
    
    @Override
    public void readFromFile(File file) throws java.io.IOException
    {
        readFile = file;
        
        Path filePath = Paths.get(file.getPath());
        byte[] fileBytes = Files.readAllBytes(filePath);
        String fileContent = new String(fileBytes);
        String[] splittedFileContent = fileContent.split("=");
        
        SubjectTableModel subjectTableModel = new SubjectTableModel();
        subjectTableModel.setContent(splittedFileContent[0]);
        
        StateSubjectTableModel stateSubjectTableModel = new StateSubjectTableModel();
        stateSubjectTableModel.setContent(splittedFileContent[1]);
        
        studentTableModel = new StudentTableModel(subjectTableModel, stateSubjectTableModel);
        studentTable.setModel(studentTableModel);
        
        studentTableModel.setContent(splittedFileContent[2]);
    }
  
    @Override
    public void writeToFile(File file) throws java.io.IOException
    {
        String fileContent = studentTableModel.getContent();
        
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
        return false;
    }
        
    public StudentTableModel getStudentTableModel()
    {
        return studentTableModel;
    }
    
    public int getSelectedStudentIndex()
    {
        return studentTable.getSelectedRow();
    }
 
    private void initPanel(SubjectTableModel subjectTableModel, StateSubjectTableModel stateSubjectTableModel)
    {
        readFile = null;
        
        setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
                
        studentTableModel = new StudentTableModel(subjectTableModel, stateSubjectTableModel);
        studentTable = new JTable(studentTableModel);
        studentTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        
        TableColumnModel studentTableColumnModel = studentTable.getColumnModel();
        studentTableColumnModel.getColumn(0).setMaxWidth(30);
        studentTableColumnModel.getColumn(1).setMinWidth(250);
        
/*        
        JTableHeader tableHeader = gradeTable.getTableHeader();
        
        Dimension headerDimension = tableHeader.getPreferredSize();
        headerDimension.setSize(headerDimension.getWidth(), 80);
        tableHeader.setPreferredSize(headerDimension);
        
        gradeTable.revalidate();
        gradeTable.repaint();
*/        
/*
        TableCellRenderer headerRenderer = new RotatedTableCellRenderer(-90.0);
        
        TableColumnModel tableColumnModel = gradeTable.getColumnModel();
        for (int columnIndex = 2; columnIndex < subjectTableModel.getRowCount() + 2; ++columnIndex)
        {
            TableColumn tableColumn = tableColumnModel.getColumn(columnIndex);
            tableColumn.setHeaderRenderer(headerRenderer);
        }        
*/
        studentTableScrollPane = new JScrollPane(studentTable, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        add(studentTableScrollPane);
    }
    
    private StudentTableModel studentTableModel;
    private JTable studentTable;
    private JScrollPane studentTableScrollPane;
    private File readFile;
}
