package filemanager;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public class AboutDialog extends JDialog implements ActionListener
{
    public AboutDialog(JFrame owner)
    {
        super(owner, "About File Manager", true);
        JPanel topPanel = new JPanel();
        JTextArea textArea = new JTextArea(8, 25);
        textArea.setEditable(false);
        textArea.append("File Manager Version 0.11\n\n"
                + "Copyright 2015 by Pasha Naichuk\n"
                + "Company: Naichuk Inc.\n\n"
                + "e-mail: pashkevych@gmail.com");
        
        topPanel.add(textArea);
        
        JPanel bottomPanel = new JPanel();
        JButton okButton = new JButton("OK");
        okButton.addActionListener(this);
        bottomPanel.add(okButton);
        
        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(topPanel, BorderLayout.NORTH);
        mainPanel.add(bottomPanel, BorderLayout.SOUTH);
        
        this.getContentPane().add(mainPanel);
        this.pack();
        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    }
    @Override
    public void actionPerformed(ActionEvent event)
    {
        this.setVisible(false);
        this.dispose();
    }
}
