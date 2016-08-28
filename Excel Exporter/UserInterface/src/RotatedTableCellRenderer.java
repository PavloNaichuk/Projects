import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

class RotatedTableCellRenderer extends JLabel implements TableCellRenderer   
{  
    public RotatedTableCellRenderer(double inputDegreesRotation)   
    {  
        degreesRotation = inputDegreesRotation;
    }  

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column)  
    {  
        try  
        {  
            this.setText(value.toString());  
        }  
        catch(NullPointerException ne)  
        {  
            this.setText("Nullvalue");  
        }  
        return this;  
    }  

    @Override
    public void paint(Graphics g)  
    {  
        Graphics2D g2 = (Graphics2D)g;
        g2.setClip(0,0,500,500);
        g2.setColor(new Color(0, 0, 0));
        g2.setFont(new Font("Arial", Font.PLAIN, 12));
        AffineTransform at = new AffineTransform();
        at.setToTranslation(getWidth(), getHeight());
        g2.transform(at);
        double radianAngle = (degreesRotation / 180.0) * Math.PI;
        at.setToRotation(radianAngle);
        g2.transform(at);
        g2.drawString(this.getText(), 0.0f, 0.0f);
    }
    private double degreesRotation = -90.0;
}  