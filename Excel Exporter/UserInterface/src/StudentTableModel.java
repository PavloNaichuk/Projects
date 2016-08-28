    import javax.swing.table.AbstractTableModel;

    public final class StudentTableModel extends AbstractTableModel
    {
        public StudentTableModel(SubjectTableModel inputSubjectTableModel, StateSubjectTableModel inputStateSubjectTableModel)
        {
            subjectTableModel = inputSubjectTableModel;
            stateSubjectTableModel = inputStateSubjectTableModel;
            
            students = new StudentInfo[MAX_ROW_COUNT];
            for (int index = 0; index < students.length; ++index)
                students[index] = new StudentInfo(subjectTableModel.getRowCount(), "");
        }
                
        @Override
        public int getRowCount() 
        {
            return MAX_ROW_COUNT;
        }

        @Override
        public int getColumnCount() 
        {
            return (subjectTableModel.getRowCount() + 12);
        }
                
        @Override
        public String getColumnName(int columnIndex)
        {               
            if (columnIndex == getRowNumberColumn())
                return "";
            
            if (columnIndex == getFullNameColumn())
                return "Прізвища студентів";
            
            if ((columnIndex >= getFirstSubjectColumn()) && (columnIndex <= getLastSubjectColumn()))
                return (String)subjectTableModel.getValueAt(columnIndex - getFirstSubjectColumn(), 1);
            
            if (columnIndex == getSeparatorColumn())
                return "";
            
            if ((columnIndex >= getFirstNationalProcentColumn()) && (columnIndex <= getLastNationalProcentColumn()))
            {
                String[] gradeProcentNames = new String[] {"%3", "%4", "%5"};
                return gradeProcentNames[columnIndex - getFirstNationalProcentColumn()];
            }
            
            if ((columnIndex >= getFirstForeignProcentColumn()) && (columnIndex <= getLastForeignProcentColumn()))
            {
                String[] gradeProcentNames = new String[] {"%A", "%B", "%C", "%D", "%E"};
                return gradeProcentNames[columnIndex - getFirstForeignProcentColumn()];
            }
            
            if (columnIndex == getAverageGradeColumn())
                return "Середній бал";
            
            return null;           
        }
        
        @Override
        public Class<?> getColumnClass(int columnIndex)
        {
            if (columnIndex == getRowNumberColumn())
                return Integer.class;
            
            if (columnIndex == getFullNameColumn())
                return String.class;
            
            if ((columnIndex >= getFirstSubjectColumn()) && (columnIndex <= getLastSubjectColumn()))
                return String.class;
                        
            if ((columnIndex >= getFirstNationalProcentColumn()) && (columnIndex <= getLastNationalProcentColumn()))
                return String.class;
            
            if ((columnIndex >= getFirstForeignProcentColumn()) && (columnIndex <= getLastForeignProcentColumn()))
                return String.class;
            
            if (columnIndex == getAverageGradeColumn())
                return String.class;           
            
            return Object.class;
        }

        @Override
        public boolean isCellEditable(int rowIndex, int columnIndex) 
        {
            return ((columnIndex == getFullNameColumn()) || ((columnIndex >= getFirstSubjectColumn()) && (columnIndex <= getLastSubjectColumn())));
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) 
        {
            if (columnIndex == getRowNumberColumn())
                return new Integer(rowIndex + 1);
                        
            if (columnIndex == getFullNameColumn())
                return students[rowIndex].getFirstNameAndLastName();
            
            if ((columnIndex >= getFirstSubjectColumn()) && (columnIndex <= getLastSubjectColumn()))
            {
                int subjectIndex = columnIndex - getFirstSubjectColumn();
                int grade = students[rowIndex].getGrade(subjectIndex);                
                if (grade > 0)
                {
                    int subjectType = subjectTableModel.getSubjectType(subjectIndex);
                    
                    String[] gradeName = new String[] {"", ""};
                    if (IsGradeA(grade))
                    {
                        if (subjectType == SubjectTableModel.SUBJECT_TYPE_EXAM)
                        {
                            gradeName[0] = "Відмінно";
                            gradeName[1] = "A";
                        }
                        else if (subjectType == SubjectTableModel.SUBJECT_TYPE_TEST)
                        {
                            gradeName[0] = "Зараховано";
                            gradeName[1] = "A";
                        }                   
                        return gradeName[0] + " " + Integer.toString(grade) + " " + gradeName[1];
                    }
                    else if (IsGradeB(grade))
                    {
                        if (subjectType == SubjectTableModel.SUBJECT_TYPE_EXAM)
                        {
                            gradeName[0] = "Добре";
                            gradeName[1] = "В";
                        }
                        else if (subjectType == SubjectTableModel.SUBJECT_TYPE_TEST)
                        {
                            gradeName[0] = "Зараховано";
                            gradeName[1] = "В";
                        }                        
                        return gradeName[0] + " " + Integer.toString(grade) + " " + gradeName[1];
                    }
                    else if (IsGradeC(grade))
                    {
                        if (subjectType == SubjectTableModel.SUBJECT_TYPE_EXAM)
                        {
                            gradeName[0] = "Добре";
                            gradeName[1] = "С";
                        }
                        else if (subjectType == SubjectTableModel.SUBJECT_TYPE_TEST)
                        {
                            gradeName[0] = "Зараховано";
                            gradeName[1] = "С";
                        }                        
                        return gradeName[0] + " " + Integer.toString(grade) + " " + gradeName[1];
                    }
                    else if (IsGradeD(grade))
                    {
                        if (subjectType == SubjectTableModel.SUBJECT_TYPE_EXAM)
                        {
                            gradeName[0] = "Задовільно";
                            gradeName[1] = "D";
                        }
                        else if (subjectType == SubjectTableModel.SUBJECT_TYPE_TEST)
                        {
                            gradeName[0] = "Зараховано";
                            gradeName[1] = "D";
                        }                        
                        return gradeName[0] + " " + Integer.toString(grade) + " " + gradeName[1];
                    }
                    else if (IsGradeE(grade))
                    {
                        if (subjectType == SubjectTableModel.SUBJECT_TYPE_EXAM)
                        {
                            gradeName[0] = "Задовільно";
                            gradeName[1] = "Е";
                        }
                        else if (subjectType == SubjectTableModel.SUBJECT_TYPE_TEST)
                        {
                            gradeName[0] = "Зараховано";
                            gradeName[1] = "Е";
                        }                        
                        return gradeName[0] + " " + Integer.toString(grade) + " " + gradeName[1];
                    }
                }
                return "";
            }
            
            if (columnIndex == getSeparatorColumn())
                return "";
            
            if ((columnIndex >= getFirstNationalProcentColumn()) && (columnIndex <= getLastNationalProcentColumn()))
            {
                float gradeProcent = students[rowIndex].getNationalGradeProcent(columnIndex - getFirstNationalProcentColumn());
                return (gradeProcent > 0) ? Float.toString(gradeProcent) : "";
            }
            
            if ((columnIndex >= getFirstForeignProcentColumn()) && (columnIndex <= getLastForeignProcentColumn()))
            {
                float gradeProcent = students[rowIndex].getForeignGradeProcent(columnIndex - getFirstForeignProcentColumn());
                return (gradeProcent > 0) ? Float.toString(gradeProcent) : "";
            }
                
            if (columnIndex == getAverageGradeColumn())
            {
                float averageGrade = students[rowIndex].getAverageGrade();
                return (averageGrade > 0) ? Float.toString(averageGrade) : "";
            }
             
            return "";
        }
        
        @Override
        public void setValueAt(Object value, int rowIndex, int columnIndex)
        {
            if (columnIndex == getFullNameColumn())
            {
                students[rowIndex].setFirstNameAndLastName((String)value);
                fireTableCellUpdated(rowIndex, columnIndex);
            }
            else if ((columnIndex >= getFirstSubjectColumn()) && (columnIndex <= getLastSubjectColumn()))
            {
                try
                {
                    String gradeString = (String)value;
                    
                    int subjectIndex = columnIndex - getFirstSubjectColumn();
                                                            
                    int subjectType = SubjectTableModel.SUBJECT_TYPE_UNKNOWN;
                    if (gradeString.indexOf("іспит") != -1)
                    {
                        gradeString = gradeString.replaceAll("іспит", "").trim();
                        
                        int grade = Integer.parseInt(gradeString);
                        students[rowIndex].setGrade(subjectIndex, grade);
                    
                        subjectType = SubjectTableModel.SUBJECT_TYPE_EXAM;
                    }
                    else if (gradeString.indexOf("залік") != -1)
                    {
                        gradeString = gradeString.replaceAll("залік", "").trim();
                        
                        int grade = Integer.parseInt(gradeString);
                        students[rowIndex].setGrade(subjectIndex, grade);
                        
                        subjectType = SubjectTableModel.SUBJECT_TYPE_TEST;
                    }
                    
                    subjectTableModel.setSubjectType(subjectIndex, subjectType);
                    fireTableCellUpdated(rowIndex, columnIndex);
                    
                    for (int index = getFirstNationalProcentColumn(); index <= getLastNationalProcentColumn(); ++index)
                        fireTableCellUpdated(rowIndex, index);
                    
                    for (int index = getFirstForeignProcentColumn(); index <= getLastForeignProcentColumn(); ++index)
                        fireTableCellUpdated(rowIndex, index);
                }
                catch (NumberFormatException ex)
                {
                }
            }            
        }
        
        private class StudentInfo
        {
            public StudentInfo(int inputSubjectCount, String inputFirstNameAndLastName)
            {
                firstNameAndLastName = inputFirstNameAndLastName;
                grades = new int[inputSubjectCount];
            }

            public String getFirstNameAndLastName()
            {
                return firstNameAndLastName;
            }

            public void setFirstNameAndLastName(String inputFirstNameAndLastName)
            {
                firstNameAndLastName = inputFirstNameAndLastName;
            }

            public int getGradeCount()
            {
                return grades.length;
            }

            public int getGrade(int inputSubjectIndex)
            {
                return grades[inputSubjectIndex];
            }

            public void setGrade(int inputSubjectIndex, int inputGrade)
            {
                grades[inputSubjectIndex] = inputGrade;
            }

            public float getAverageGrade()
            {
                int totalGradeSum = 0;
                int totalGradeCount = 0;
                
                for (int grade : grades)
                {
                    if (grade > 0)
                    {
                        totalGradeSum += grade;
                        ++totalGradeCount;
                    }
                }
                
                return (totalGradeCount > 0) ? ((float)totalGradeSum / (float)totalGradeCount) : 0;
            }
            
            public float getNationalGradeProcent(int gradeIndex)
            {
                int gradeCount = 0;
                int totalGradeCount = 0;
                
                for (int grade : grades)
                {
                    if (grade > 0)
                    {
                        ++totalGradeCount;
                        switch (gradeIndex)
                        {
                            case 0:
                            {
                                if (IsGradeE(grade) || IsGradeD(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 1:
                            {
                                if (IsGradeB(grade) || IsGradeC(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 2:
                            {
                                if (IsGradeA(grade))
                                    ++gradeCount;
                                break;
                            }
                        }
                    }
                }
                return (totalGradeCount > 0) ? ((float)gradeCount * 100.0f / (float)totalGradeCount) : 0;
            }
            
            public float getForeignGradeProcent(int gradeIndex)
            {
                int gradeCount = 0;
                int totalGradeCount = 0;
                
                for (int grade : grades)
                {
                    if (grade > 0)
                    {
                        ++totalGradeCount;
                        switch (gradeIndex)
                        {
                            case 0:
                            {
                                if (IsGradeA(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 1:
                            {
                                if (IsGradeB(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 2:
                            {
                                if (IsGradeC(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 3:
                            {
                                if (IsGradeD(grade))
                                    ++gradeCount;
                                break;
                            }
                            case 4:
                            {
                                if (IsGradeE(grade))
                                    ++gradeCount;
                                break;
                            }
                        }
                    }
                }
                return (totalGradeCount > 0) ? ((float)gradeCount * 100.0f / (float)totalGradeCount) : 0;
            }

            private String firstNameAndLastName;
            private int[] grades;
        }
        
        public void setContent(String content)
        {
            String[] studentInfo = content.split(",");
            for (int index = 0; index < studentInfo.length; ++index)
            {
                String[] line = studentInfo[index].split(">");
                
                students[index].setFirstNameAndLastName(line[0]);
                for (int gradeIndex = 1; gradeIndex < line.length; ++gradeIndex)
                {
                    students[index].setGrade(gradeIndex - 1, Integer.parseInt(line[gradeIndex]));
                }
            }
            fireTableDataChanged();
        }
        
        public String getContent()
        {
            StringBuilder content = new StringBuilder();
            for (StudentInfo studentInfo : students)
            {
                content.append(studentInfo.getFirstNameAndLastName());
                content.append(">");
                
                for (int index = 0; index < studentInfo.getGradeCount(); ++index)
                {
                    content.append(studentInfo.getGrade(index));
                    content.append(">");
                }
     
                content.append(",");
            }
            
            return subjectTableModel.getContent() + "=" 
                    + stateSubjectTableModel.getContent() + "="
                    + content.toString(); 
        }
        
        private int getRowNumberColumn()
        {
            return 0;
        }
        
        private int getFullNameColumn()
        {
            return 1;
        }
        
        private int getFirstSubjectColumn()
        {
            return getFullNameColumn() + 1;
        }
        
        private int getLastSubjectColumn()
        {
            return getFirstSubjectColumn() + subjectTableModel.getRowCount() - 1;
        }
        
        private int getSeparatorColumn()
        {
            return getLastSubjectColumn() + 1;
        }
        
        private int getFirstNationalProcentColumn()
        {
            return getSeparatorColumn() + 1;
        }
        
        private int getLastNationalProcentColumn()
        {
            return getFirstNationalProcentColumn() + 2;
        }
        
        private int getFirstForeignProcentColumn()
        {
            return getLastNationalProcentColumn() + 1;
        }
        
        private int getLastForeignProcentColumn()
        {
            return getFirstForeignProcentColumn() + 4;
        }
        
        private int getAverageGradeColumn()
        {
            return getLastForeignProcentColumn() + 1;            
        }
        
        private static boolean IsGradeA(int grade)
        {
            return (grade >= 90) && (grade <= 100);
        }

        private static boolean IsGradeB(int grade)
        {
            return (grade >= 80) && (grade <= 89);
        }

        private static boolean IsGradeC(int grade)
        {
            return (grade >= 70) && (grade <= 79);
        }

        private static boolean IsGradeD(int grade)
        {
            return (grade >= 60) && (grade <= 69);
        }

        private static boolean IsGradeE(int grade)
        {
            return (grade >= 35) && (grade <= 59);
        }
    
        private static int MAX_ROW_COUNT = 50;
        
        private StudentInfo[] students;
        private SubjectTableModel subjectTableModel;
        private StateSubjectTableModel stateSubjectTableModel;
    }
