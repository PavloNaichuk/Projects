package filemanager;

import java.awt.Component;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;

public class FilesTable extends JTable
{
    public FilesTable(TableModel tableModel)
    {
        super(tableModel);
        this.showSelectedFile = true;
        this.tableCellRenderer = new FilesTableCellRenderer();
        this.setColumnSelectionAllowed(false);
    }
    public void setShowSelectedFile(boolean showSelectedFile)
    {
        this.showSelectedFile = showSelectedFile;
    }
    public boolean showSelectedFile()
    {
        return this.showSelectedFile;
    }
    @Override
    public TableCellRenderer getCellRenderer(int row, int column)
    {
        return this.tableCellRenderer;
    }
    @Override
    public boolean isCellSelected(int row, int column)
    {
        return super.isCellSelected(row, column);
    }
    private class FilesTableCellRenderer extends DefaultTableCellRenderer
    {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column)
        {
            if (row == getSelectedRow())
            {
                isSelected = showSelectedFile;
                hasFocus = false;
            }    
                
            return super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
        }   
    }
    private boolean showSelectedFile;
    private TableCellRenderer tableCellRenderer;
}
