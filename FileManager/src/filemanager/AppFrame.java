package filemanager;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import javax.swing.AbstractAction;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

public class AppFrame extends JFrame
{
    public AppFrame(int width, int height)
    {
        super();
        
        this.setSize(width, height);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("File Manager");
        this.setResizable(false);
        
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Dimension screenSize = toolkit.getScreenSize();
        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;
        int frameX = (screenWidth - width) / 2;
        int frameY = (screenHeight - height) / 2;
        this.setLocation(frameX, frameY);
        
        this.mainPanel = new MainPanel(this);
        this.add(mainPanel);
        this.panelListener = mainPanel;
        
        JMenuBar menuBar = new JMenuBar();
        this.setJMenuBar(menuBar);
        
        JMenu fileMenu = new JMenu("File");
        menuBar.add(fileMenu);
        JMenuItem exitItem = new JMenuItem(new ExitAction(this)); 
        exitItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F4, InputEvent.ALT_MASK));
        fileMenu.add(exitItem);
        
        JMenu commandsMenu = new JMenu("Commands");
        menuBar.add(commandsMenu);
        
        JMenuItem copyItem = new JMenuItem(new CopyAction());
        copyItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F5, 0));
        commandsMenu.add(copyItem);
        
        JMenuItem moveItem = new JMenuItem(new MoveAction());
        moveItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F6, 0));
        commandsMenu.add(moveItem);
        
        JMenuItem newFolderItem = new JMenuItem(new NewFolderAction());
        newFolderItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F7, 0));
        commandsMenu.add(newFolderItem);
        
        JMenuItem deleteItem = new JMenuItem(new DeleteAction());
        deleteItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F8, 0));
        commandsMenu.add(deleteItem);
        
        JMenu showMenu = new JMenu("Show");
        menuBar.add(showMenu);
        
        this.hiddenItem = new JCheckBoxMenuItem("Hidden", false);
        this.hiddenItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, InputEvent.CTRL_MASK));
        this.hiddenItem.setSelected(this.mainPanel.showHiddenFiles());
        this.hiddenItem.addActionListener(new ShowHiddenFilesListener());
        showMenu.add(hiddenItem);
        
        JMenu filesProperies = new JMenu("Files Properties");
        
        this.extItem = new JCheckBoxMenuItem("Extension", false);
        this.extItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_E, InputEvent.ALT_MASK));
        this.extItem.setSelected(this.mainPanel.showExt());
        this.extItem.addActionListener(new ShowFilesExtListener());
        
        this.sizeItem = new JCheckBoxMenuItem("Size", false);
        this.sizeItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.ALT_MASK));
        this.sizeItem.setSelected(this.mainPanel.showSize());
        this.sizeItem.addActionListener(new ShowFilesSizeListener());
        
        this.dateItem = new JCheckBoxMenuItem("Date", false);
        this.dateItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D, InputEvent.ALT_MASK));
        this.dateItem.setSelected(this.mainPanel.showDate());
        this.dateItem.addActionListener(new ShowFilesDateListener());
        
        filesProperies.add(extItem);
        filesProperies.add(sizeItem);
        filesProperies.add(dateItem);
        
        showMenu.add(filesProperies);
    }
    private class ExitAction extends AbstractAction
    {
        public ExitAction(JFrame frame)
        {
            super("Exit");
            this.frame = frame;
        }
        @Override
        public void actionPerformed(ActionEvent event)
        {
            this.frame.setVisible(false);
            this.frame.dispose();
        }
        private JFrame frame;
    }
    
    private class CopyAction extends AbstractAction
    {
        public CopyAction()
        {
            super("Copy");
        }
        @Override
        public void actionPerformed(ActionEvent event)
        {
            panelListener.onFileCopyPerformed();
        }
    }
    
    private class MoveAction extends AbstractAction
    {
        public MoveAction()
        {
            super("Move");
        }
        @Override
        public void actionPerformed(ActionEvent event)
        {
            panelListener.onFileMovePerformed();
        }
    }
    
    private class NewFolderAction extends AbstractAction
    {
        public NewFolderAction()
        {
            super("New Folder");
        }
        @Override
        public void actionPerformed(ActionEvent event)
        {
            panelListener.onNewFolderPerformed();
        }
    }
    
    private class DeleteAction extends AbstractAction
    {
        public DeleteAction()
        {
            super("Delete");
        }
        @Override
        public void actionPerformed(ActionEvent event)
        {
            panelListener.onFileDeletePerformed();
        }
    }
    
    private class ShowHiddenFilesListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            mainPanel.setShowHiddenFiles(hiddenItem.isSelected());
        } 
    }
    private class ShowFilesExtListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            mainPanel.setShowExt(extItem.isSelected());
        }
    }
    private class ShowFilesSizeListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            mainPanel.setShowSize(sizeItem.isSelected());
        }
    }
    private class ShowFilesDateListener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent event)
        {
            mainPanel.setShowDate(dateItem.isSelected());
        }
    }
    private CommandPanelListener panelListener;
    private JCheckBoxMenuItem hiddenItem;
    private MainPanel mainPanel;
    private JCheckBoxMenuItem extItem;
    private JCheckBoxMenuItem sizeItem;
    private JCheckBoxMenuItem dateItem;  
}
