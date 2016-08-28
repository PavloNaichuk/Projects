package filemanager;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JButton;
import javax.swing.JPanel;

public class CommandPanel extends JPanel
{
    public CommandPanel()
    {
        super();
        ButtonListener buttonListener = new ButtonListener();
        
        JButton copyButton = new JButton("F5 Copy");
        copyButton.setActionCommand("copy");
        copyButton.addActionListener(buttonListener);
        this.add(copyButton);
        
        JButton moveButton = new JButton("F6 Move");
        moveButton.setActionCommand("move");
        moveButton.addActionListener(buttonListener);
        this.add(moveButton);
        
        JButton newFolderButton = new JButton("F7 New Folder");
        newFolderButton.setActionCommand("new folder");
        newFolderButton.addActionListener(buttonListener);
        this.add(newFolderButton);
        
        JButton deleteButton = new JButton("F8 Delete");
        deleteButton.setActionCommand("delete");
        deleteButton.addActionListener(buttonListener);
        this.add(deleteButton);
        
        this.panelListener = null;
    }
    public void setPanelListener(CommandPanelListener panelListener)
    {
        this.panelListener = panelListener;
    }
    public CommandPanelListener getPanelListener()
    {
        return this.panelListener;
    }
    private class ButtonListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            String actionCommand = event.getActionCommand();
            if (actionCommand.equals("copy"))
            {
                if (panelListener != null)
                    panelListener.onFileCopyPerformed();
            }
            else if (actionCommand.equals("move"))
            {
                if (panelListener != null)
                    panelListener.onFileMovePerformed();
            }
            else if (actionCommand.equals("new folder"))
            {
                if (panelListener != null)
                    panelListener.onNewFolderPerformed();
            }
            else if (actionCommand.equals("delete"))
            {
                if (panelListener != null)
                    panelListener.onFileDeletePerformed();
            }
        }
    }
    private CommandPanelListener panelListener;
}
