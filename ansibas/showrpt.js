var fso, ReportFile, ForReading, TristateTrue, Text;

// ������� ������ �� ������ ���������� ��������� ������
var objArgs = WScript.Arguments;

// TODO: ������� �������� �� ���������� ����������

// ��������� ������ ���������� �� ���������
var sReportFile = objArgs(0);

// ������� ��������� ��������� ������
WScript.StdOut.WriteLine( "Report file: " + sReportFile );

fso = WScript.CreateObject( "Scripting.FileSystemObject" );

ForReading = 1; // ��� ������
TristateFalse = 0; // ������ ASCII

// ��������� ����� ������ �� �����.
ReportFile = fso.OpenTextFile( sReportFile, ForReading, false, TristateFalse );

try {

    for ( var i = 0; i < 30; i ++ ) WScript.StdOut.WriteLine( ReportFile.ReadLine() );

} catch(e) {

} finally {

    ReportFile.Close();
}
