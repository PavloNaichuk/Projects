    import javax.swing.table.AbstractTableModel;

    public final class StateSubjectTableModel extends AbstractTableModel
    {
        public StateSubjectTableModel()
        {
            subjects = new StateSubjectInfo[MAX_STATE_SUBJECT_COUNT];
            clearContent();
        }
        
        public void clearContent()
        {
            hasChanges = false;
            for (int index = 0; index < subjects.length; ++index)
                subjects[index] = new StateSubjectInfo("");
            
            fireTableDataChanged();
        }
        
        public String getContent()
        {
            StringBuilder content = new StringBuilder();
            for (StateSubjectInfo subject : subjects)
            {
                content.append(subject.getName());
                content.append(",");
            }
            return content.toString();
        }
        
        public void setContent(String content)
        {
            String[] subjectInfo = content.split(",");
            for (int index = 0; index < subjectInfo.length; ++index)
            {
                subjects[index] = new StateSubjectInfo(subjectInfo[index]);
            }
            fireTableDataChanged();
        }
        
        @Override
        public int getRowCount() 
        {
            return MAX_STATE_SUBJECT_COUNT;
        }

        @Override
        public int getColumnCount() 
        {
            return 2;
        }

        @Override
        public String getColumnName(int columnIndex) 
        {
            switch (columnIndex)
            {
                case 0: return "";
                case 1: return "Державні іспити";
                default: return null;
            } 
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) 
        {
            return (columnIndex != 0);
        }
        
        @Override
        public Class<?> getColumnClass(int columnIndex)
        {
            switch (columnIndex)
            {
                case 0:
                {
                    return Integer.class;
                }
                case 1:
                {
                    return String.class;
                }
            } 
            return Object.class;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) 
        {
            switch (columnIndex)
            {
                case 0: return new Integer(rowIndex + 1);
                case 1: return subjects[rowIndex].getName();
                default: return null;
            }
        }
        
        @Override
        public void setValueAt(Object value, int rowIndex, int columnIndex)
        {
            if (columnIndex == 1)
            {
                subjects[rowIndex].setName((String)value);
            }
            
            fireTableCellUpdated(rowIndex, columnIndex);
            hasChanges = true;
        }
        
        public boolean hasChanges()
        {
            return hasChanges;
        }
        
        private class StateSubjectInfo
        {            
            public StateSubjectInfo(String inputName)
            {
                name = inputName;
            }
            
            public String getName() 
            {
                return name;
            }
            
            public void setName(String inputName)
            {
                name = inputName;
            }
            
            private String name;
        }
        
        private static int MAX_STATE_SUBJECT_COUNT = 5;
        private StateSubjectInfo[] subjects;
        private boolean hasChanges;
    }