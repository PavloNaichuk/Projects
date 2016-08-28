import java.io.File;
import java.io.*;

public class ExcelExporter 
{
    public static int export(File outputFile, File tempExportFile, int studentIndex) throws IOException, InterruptedException
    {
        Runtime runtime = Runtime.getRuntime();
        
        File reportTemplate = new File("Report Template.xls");
                
        String[] cmd = new String[]
        {
            "ExcelExporter.exe",
            Integer.toString(studentIndex),
            reportTemplate.getAbsolutePath(),
            tempExportFile.getAbsolutePath(),
            outputFile.getAbsolutePath()
        };
                        
        Process exporterProcess = runtime.exec(cmd);            
        
        int result = exporterProcess.waitFor();
        return result;
    }
}
