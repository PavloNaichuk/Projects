    import javax.swing.table.AbstractTableModel;

    public final class SubjectTableModel extends AbstractTableModel
    {
        public SubjectTableModel()
        {
            subjects = new SubjectInfo[MAX_SUBJECT_COUNT];
            clearContent();
        }
        
        public void clearContent()
        {
            hasChanges = false;
            for (int index = 0; index < subjects.length; ++index)
                subjects[index] = new SubjectInfo("", -1, SUBJECT_TYPE_UNKNOWN);
            
            fireTableDataChanged();
        }
        
        public int getSubjectType(int index)
        {
            return subjects[index].getType();
        }
        
        public void setSubjectType(int index, int type)
        {
            subjects[index].setType(type);
        }
        
        public void setContent(String content)
        {
            String[] subjectInfo = content.split(",");
            for (int index = 0; index < subjectInfo.length; ++index)
            {
                String[] line = subjectInfo[index].split(">");
                subjects[index] = new SubjectInfo(line[0], Integer.parseInt(line[1]), Integer.parseInt(line[2]));
            }
            fireTableDataChanged();
        }
        
        public String getContent()
        {
            StringBuilder content = new StringBuilder();
            for (SubjectInfo subject : subjects)
            {
                content.append(subject.getName());
                content.append(">");
                content.append(subject.getHourCount());
                content.append(">");
                content.append(subject.getType());
                content.append(",");
            }
            return content.toString();
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
                case 2:
                {
                    return Integer.class;
                }
            } 
            return Object.class;
        }
        
        @Override
        public int getRowCount()
        {
            return MAX_SUBJECT_COUNT;
        }

        @Override
        public int getColumnCount()
        {
            return 3;
        }

        @Override
        public String getColumnName(int columnIndex) 
        {
            if (columnIndex == 0)
                return "";
            else if (columnIndex == 1)
                return "Предмет";
            else if (columnIndex == 2)    
                return "К-ть годин";
            return null;           
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) 
        {
            return (columnIndex != 0);
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) 
        {   
            switch (columnIndex)
            {
                case 0:
                {
                    return new Integer(rowIndex + 1);
                }
                case 1:
                {
                    String name = subjects[rowIndex].getName(); 
                    return (name == null) ? "" : name;
                }
                case 2:
                {
                    int hourCount = subjects[rowIndex].getHourCount();
                    return (hourCount == -1) ? "" : new Integer(hourCount);                  
                }
                default:
                {
                    return null;
                }
            }
        }
        
        @Override
        public void setValueAt(Object value, int rowIndex, int columnIndex)
        {
            switch (columnIndex)
            {
                case 1:
                {
                    subjects[rowIndex].setName((String)value);
                    break;
                }
                case 2:
                {
                    subjects[rowIndex].setHourCount(((Integer)value).intValue());
                    break;
                }
            }
            
            fireTableCellUpdated(rowIndex, columnIndex);
            hasChanges = true;
        }
        
        public boolean hasChanges()
        {
            return hasChanges;
        }
        
        private class SubjectInfo
        {
            public SubjectInfo(String inputName, int inputHourCount, int inputType)
            {
                name = inputName;
                hourCount = inputHourCount;
                type = inputType;
            }
            
            public String getName()
            {
                return name;
            }
            
            public void setName(String inputName)
            {
                name = inputName;
            }
            
            public int getHourCount()
            {
                return hourCount;
            }
            
            public void setHourCount(int inputHourCount)
            {
                hourCount = inputHourCount;
            }
            
            public int getType()
            {
                return type;
            }
            
            public void setType(int inputType)
            {
                type = inputType;
            }
                        
            private String name;
            private int hourCount;
            private int type;
        }
        
        public static int SUBJECT_TYPE_EXAM = 0;
        public static int SUBJECT_TYPE_TEST = 1;
        public static int SUBJECT_TYPE_UNKNOWN = 2;
                
        private static int MAX_SUBJECT_COUNT = 80;
        private SubjectInfo[] subjects;
        private boolean hasChanges;
        
        private SubjectTableModel subjectTableModel;
        private StateSubjectTableModel stateSubjectTableModel;
    }