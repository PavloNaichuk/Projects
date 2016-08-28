using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Excel;

namespace ExcelHelper
{
    class SubjectInfo
    {
        public SubjectInfo(string inputName, int inputHourCount, int inputType)
        {
            name = inputName;
            hourCount = inputHourCount;
            type = inputType;
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public int HourCount
        {
            get { return hourCount; }
            set { hourCount = value; }
        }

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        private string name;
        private int hourCount;
        private int type;
    }

    class StateSubjectInfo
    {
        public StateSubjectInfo(string inputName)
        {
            name = inputName;
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        private string name;
    }

    class StudentInfo
    {
        public StudentInfo(string inputFullName)
        {
            fullName = inputFullName;
            grades = new int[80];
        }

        public string FullName
        {
            get { return fullName; }
            set { fullName = value; }
        }

        public int[] Grades
        {
            get { return grades; }
        }

        private string fullName;
        private int[] grades;
    }

    class Program
    {
        static void Main(string[] args)
        {
            int studentIndex = Convert.ToInt32(args[0]);
            string excelTemplatePath = args[1];
            string nauProjectPath = args[2];
            string outputPath = args[3];

            System.IO.StreamReader inputFile = new System.IO.StreamReader(nauProjectPath);
            string fileContent = inputFile.ReadToEnd();
            inputFile.Close();

            string[] splittedFileContent = fileContent.Split('=');

            SubjectInfo[] subjects = ParseSubjectInfo(splittedFileContent[0]);
            StateSubjectInfo[] stateSubjects = ParseStateSubjectInfo(splittedFileContent[1]);
            StudentInfo[] students = ParseStudentInfo(splittedFileContent[2]);

            ExportToExcel(studentIndex, excelTemplatePath, outputPath, subjects, stateSubjects, students);
        }

        private static void ReleaseCOMObject(object obj)
        {
            try
            {
                System.Runtime.InteropServices.Marshal.ReleaseComObject(obj);
                obj = null;
            }
            catch (Exception ex)
            {
                obj = null;
            }
            finally
            {
                GC.Collect();
            }
        }

        private static SubjectInfo[] ParseSubjectInfo(string content)
        {
            string[] allSubjectInfo = content.Split(',');

            SubjectInfo[] subjects = new SubjectInfo[allSubjectInfo.Length - 1];
            for (int index = 0; index < subjects.Length; ++index)
            {
                string[] subjectInfo = allSubjectInfo[index].Split('>');
                string subjectName = (subjectInfo[0].Length > 0) ? (Convert.ToString(index + 1) + "." + subjectInfo[0]) : "";
                subjects[index] = new SubjectInfo(subjectName, Convert.ToInt32(subjectInfo[1]), Convert.ToInt32(subjectInfo[2]));
            }

            return subjects;
        }

        private static StateSubjectInfo[] ParseStateSubjectInfo(string content)
        {
            string[] allSubjectInfo = content.Split(',');

            StateSubjectInfo[] subjects = new StateSubjectInfo[allSubjectInfo.Length - 1];
            for (int index = 0; index < subjects.Length; ++index)
            {
                subjects[index] = new StateSubjectInfo(allSubjectInfo[index]);
            }

            return subjects;
        }

        private static StudentInfo[] ParseStudentInfo(string content)
        {
            string[] allStudentInfo = content.Split(',');

            StudentInfo[] students = new StudentInfo[allStudentInfo.Length - 1];
            for (int studentIndex = 0; studentIndex < students.Length; ++studentIndex)
            {
                String[] studentInfo = allStudentInfo[studentIndex].Split('>');

                students[studentIndex] = new StudentInfo(studentInfo[0]);
                for (int gradeIndex = 1; gradeIndex < (studentInfo.Length - 1); ++gradeIndex)
                    students[studentIndex].Grades[gradeIndex - 1] = Convert.ToInt32(studentInfo[gradeIndex]);
            }

            return students;
        }

        private static void ExportToExcel(int studentIndex, string excelTemplateFilePath, string outputFilePath,
            SubjectInfo[] subjects, StateSubjectInfo[] stateSubjects, StudentInfo[] students)
        {
            Microsoft.Office.Interop.Excel.Application xlApp = new Microsoft.Office.Interop.Excel.Application();
            Microsoft.Office.Interop.Excel.Workbook xlWorkBook = xlApp.Workbooks.Open(excelTemplateFilePath, 0, false);

            Microsoft.Office.Interop.Excel.Worksheet xlWorkSheet1 = (Microsoft.Office.Interop.Excel.Worksheet)xlWorkBook.Worksheets[1];

            Microsoft.Office.Interop.Excel.Font excelTextFont = xlWorkSheet1.Cells[STATE_SUBJECT_ROW, STATE_SUBJECT_COL].Font;
            System.Drawing.Font textFont = new System.Drawing.Font(excelTextFont.Name, 8/*excelTextFont.Size*/);
            double cellWidth = 312;// xlWorkSheet2.Cells[STATE_SUBJECT_ROW, STATE_SUBJECT_COL].EntireColumn.ColumnWidth;

            int stateSubjectRowIndex = STATE_SUBJECT_ROW;
            for (int index = 0; index < stateSubjects.Length; ++index)
            {
                if (stateSubjects[index].Name.Length > 0)
                {
                    ArrayList splittedSubjectName = SplitSubjectName(stateSubjects[index].Name, textFont, cellWidth);
                    foreach (string subjectName in splittedSubjectName)
                    {
                        xlWorkSheet1.Cells[stateSubjectRowIndex, STATE_SUBJECT_COL] = subjectName;
                        ++stateSubjectRowIndex;
                    }
                }
            }

            Microsoft.Office.Interop.Excel.Worksheet xlWorkSheet3 = (Microsoft.Office.Interop.Excel.Worksheet)xlWorkBook.Worksheets[1];

            StudentInfo studentInfo = students[studentIndex];

            string[] parsedStudentFullName = studentInfo.FullName.Split(' ');
            if (parsedStudentFullName.Length > 0)
                xlWorkSheet3.Cells[LAST_NAME_ROW, LAST_NAME_COL] = parsedStudentFullName[0];

            string firstName = "";
            if (parsedStudentFullName.Length > 2)
                firstName = parsedStudentFullName[1] + " " + parsedStudentFullName[2];
            else if (parsedStudentFullName.Length > 1)
                firstName = parsedStudentFullName[1];
            xlWorkSheet3.Cells[FIRST_NAME_ROW, FIRST_NAME_COL] = firstName;
           
            Microsoft.Office.Interop.Excel.Worksheet xlWorkSheet2 = (Microsoft.Office.Interop.Excel.Worksheet)xlWorkBook.Worksheets[2];

            float totalGradeSum = 0.0f;
            int gradeCount = 0;
            int totalHourCount = 0;

            int part = 0;
            int partOffset = 0;

            excelTextFont = xlWorkSheet2.Cells[SUBJECT_ROW_START[part], SUBJECT_COL[part]].Font;
            textFont = new System.Drawing.Font(excelTextFont.Name, 8);
            cellWidth = xlWorkSheet2.Cells[SUBJECT_ROW_START[part], SUBJECT_COL[part]].EntireColumn.ColumnWidth;

            for (int index = 0; index < subjects.Length; ++index)
            {
                if (subjects[index].Name.Length > 0)
                {
                    ArrayList splittedSubjectName = SplitSubjectName(subjects[index].Name, textFont, cellWidth);
                    if ((SUBJECT_ROW_START[part] + partOffset + splittedSubjectName.Count) > SUBJECT_ROW_FINISH[part])
                    {
                        partOffset = 0;
                        ++part;
                    }

                    foreach (string subjectName in splittedSubjectName)
                    {
                        xlWorkSheet2.Cells[SUBJECT_ROW_START[part] + partOffset, SUBJECT_COL[part]] = subjectName;
                        ++partOffset;
                    }

                    string hourCount = "";
                    if (subjects[index].HourCount != -1)
                    {
                        totalHourCount += subjects[index].HourCount;
                        hourCount = Convert.ToString(subjects[index].HourCount);
                    }
                    xlWorkSheet2.Cells[SUBJECT_ROW_START[part] + partOffset - 1, HOUR_COUNT_COL[part]] = hourCount;

                    string grade = "";
                    if (studentInfo.Grades[index] != 0)
                    {
                        totalGradeSum += studentInfo.Grades[index];
                        ++gradeCount;

                        grade = GetGradeName(studentInfo.Grades[index], subjects[index].Type);
                    }
                    xlWorkSheet2.Cells[SUBJECT_ROW_START[part] + partOffset - 1, GRADE_COL[part]] = grade;
                }
            }

            xlWorkSheet2.Cells[AVERAGE_GRADE_ROW, AVERAGE_GRADE_COL] = Convert.ToString(totalGradeSum / (float)gradeCount);
            xlWorkSheet2.Cells[TOTAL_HOUR_COUNT_ROW, TOTAL_HOUR_COUNT_COL] = Convert.ToString(totalHourCount);

            if (System.IO.File.Exists(outputFilePath))
                System.IO.File.Delete(outputFilePath);

            xlWorkBook.SaveAs(outputFilePath);
            xlWorkBook.Close(true);

            xlApp.Quit();

            ReleaseCOMObject(xlApp);
            ReleaseCOMObject(xlWorkBook);
            ReleaseCOMObject(xlWorkSheet1);
        }

        static private ArrayList SplitSubjectName(string subjectName, System.Drawing.Font textFont, double cellWidth)
        {
            ArrayList array = new ArrayList();

            string leftString = subjectName.Trim();
            string rightString = "";

            do
            {
                System.Drawing.Size textSize = System.Windows.Forms.TextRenderer.MeasureText(leftString, textFont);
                if (textSize.Width < cellWidth)
                {
                    array.Add(leftString);
                    leftString = rightString;
                    rightString = "";
                }
                else
                {
                    int splitIndex = leftString.LastIndexOf(' ');
                    if (splitIndex == -1)
                    {
                        array.Add(leftString);
                        leftString = rightString;
                        rightString = "";
                    }
                    else
                    {
                        rightString = leftString.Substring(splitIndex + 1).Trim() + " " + rightString;
                        leftString = leftString.Substring(0, splitIndex).Trim();
                    }
                }
            }
            while (leftString.Length > 0);

            return array;
        }

        private static bool IsGradeA(int grade)
        {
            return (grade >= 90) && (grade <= 100);
        }

        private static bool IsGradeB(int grade)
        {
            return (grade >= 80) && (grade <= 89);
        }

        private static bool IsGradeC(int grade)
        {
            return (grade >= 70) && (grade <= 79);
        }

        private static bool IsGradeD(int grade)
        {
            return (grade >= 60) && (grade <= 69);
        }

        private static bool IsGradeE(int grade)
        {
            return (grade >= 35) && (grade <= 59);
        }

        private static string GetGradeName(int grade, int subjectType)
        {
            string[] gradeName = new string[2] { "", "" };
            if (IsGradeA(grade))
            {
                if (subjectType == SUBJECT_TYPE_EXAM)
                {
                    gradeName[0] = "Відмінно";
                    gradeName[1] = "A";
                }
                else if (subjectType == SUBJECT_TYPE_TEST)
                {
                    gradeName[0] = "Зараховано";
                    gradeName[1] = "A";
                }
                return gradeName[0] + " " + Convert.ToString(grade) + " " + gradeName[1];
            }
            else if (IsGradeB(grade))
            {
                if (subjectType == SUBJECT_TYPE_EXAM)
                {
                    gradeName[0] = "Добре";
                    gradeName[1] = "В";
                }
                else if (subjectType == SUBJECT_TYPE_TEST)
                {
                    gradeName[0] = "Зараховано";
                    gradeName[1] = "В";
                }
                return gradeName[0] + " " + Convert.ToString(grade) + " " + gradeName[1];
            }
            else if (IsGradeC(grade))
            {
                if (subjectType == SUBJECT_TYPE_EXAM)
                {
                    gradeName[0] = "Добре";
                    gradeName[1] = "С";
                }
                else if (subjectType == SUBJECT_TYPE_TEST)
                {
                    gradeName[0] = "Зараховано";
                    gradeName[1] = "С";
                }
                return gradeName[0] + " " + Convert.ToString(grade) + " " + gradeName[1];
            }
            else if (IsGradeD(grade))
            {
                if (subjectType == SUBJECT_TYPE_EXAM)
                {
                    gradeName[0] = "Задовільно";
                    gradeName[1] = "D";
                }
                else if (subjectType == SUBJECT_TYPE_TEST)
                {
                    gradeName[0] = "Зараховано";
                    gradeName[1] = "D";
                }
                return gradeName[0] + " " + Convert.ToString(grade) + " " + gradeName[1];
            }
            else if (IsGradeE(grade))
            {
                if (subjectType == SUBJECT_TYPE_EXAM)
                {
                    gradeName[0] = "Задовільно";
                    gradeName[1] = "Е";
                }
                else if (subjectType == SUBJECT_TYPE_TEST)
                {
                    gradeName[0] = "Зараховано";
                    gradeName[1] = "Е";
                }
                return gradeName[0] + " " + Convert.ToString(grade) + " " + gradeName[1];
            }

            return null;
        }

        public static int SUBJECT_TYPE_EXAM = 0;
        public static int SUBJECT_TYPE_TEST = 1;
        public static int SUBJECT_TYPE_UNKNOWN = 2;

        private static int STATE_SUBJECT_COL = 2;
        private static int STATE_SUBJECT_ROW = 11;

        private static int LAST_NAME_COL = 24;
        private static int LAST_NAME_ROW = 2;

        private static int FIRST_NAME_COL = 24;
        private static int FIRST_NAME_ROW = 2;

        private static int[] SUBJECT_ROW_START = new int[3] { 3, 1, 1 };
        private static int[] SUBJECT_ROW_FINISH = new int[3] { 36, 36, 34 };
        private static int[] SUBJECT_COL = new int[3] { 2, 6, 10 };
        private static int[] HOUR_COUNT_COL = new int[3] { 3, 7, 11 };
        private static int[] GRADE_COL = new int[3] { 4, 8, 12 };

        private static int AVERAGE_GRADE_COL = 10;
        private static int AVERAGE_GRADE_ROW = 36;

        private static int TOTAL_HOUR_COUNT_COL = 11;
        private static int TOTAL_HOUR_COUNT_ROW = 36;
    }
}
