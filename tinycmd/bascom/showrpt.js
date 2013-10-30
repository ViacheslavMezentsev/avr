var fso, ReportFile, ForReading, TristateTrue, Text;

// Создаём ссылку на массив параметров командной строки
var objArgs = WScript.Arguments;

// TODO: Сделать проверку на количество параметров

// Назначаем каждой переменной по параметру
var sReportFile = objArgs(0);

// ¬ыводим параметры командной строки
WScript.StdOut.WriteLine( "Report file: " + sReportFile );

fso = WScript.CreateObject( "Scripting.FileSystemObject" );

ForReading = 1; // Для чтения
TristateFalse = 0; // Формат ASCII

// Считываем номер версии из файла.
ReportFile = fso.OpenTextFile( sReportFile, ForReading, false, TristateFalse );

try {

    for ( var i = 0; i < 30; i ++ ) WScript.StdOut.WriteLine( ReportFile.ReadLine() );

} catch(e) {

} finally {

    ReportFile.Close();
}
