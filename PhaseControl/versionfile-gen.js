/**
* ������������� ������������� ����� ������ CVersion
*
* ������: ATmega32
*
* �����: �������� �. �.
* skype: viacheslavmezentsev
* �����: mailto:unihomelab@ya.ru
*
* ����: �� 02.10.2011 �.
*
**********************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Revision, Build;

// ������ ������ �� ������ ���������� ��������� ������
var objArgs = WScript.Arguments;

// TODO: ������� �������� �� ���������� ����������

// ��������� ������ ���������� �� ���������
var sSourceFile = objArgs(0);
var sVersionFile = objArgs(1);

// ������� ��������� ��������� ������
WScript.StdOut.WriteLine( "Output file: " +  sSourceFile + ".h" );
WScript.StdOut.WriteLine( "Version file: " +  sVersionFile );

fso = WScript.CreateObject( "Scripting.FileSystemObject" );

ForReading = 1; // ��� ������
TristateFalse = 0; // ������ ASCII

// ��������� ����� ������ �� �����
VersionFile = fso.OpenTextFile( sVersionFile, ForReading, false, TristateFalse );

Major = VersionFile.ReadLine();
Minor = VersionFile.ReadLine();
Revision = VersionFile.ReadLine();
Build = VersionFile.ReadLine();

VersionFile.Close();

// ��������� ������������ ���� ������
var SourceFile;

SourceFile = fso.CreateTextFile( sSourceFile + ".h", true );

var d = new Date();

// ������� ����� ������
SourceFile.Write(
"\
///////////////////////////////////////////////////////////\r\n\
//  Version.h\r\n\
//  Implementation of the Class CVersion\r\n\
//  Created on:      " +
d.toLocaleDateString() + " " + d.toLocaleTimeString() +
"\r\n\
//  Original author: �������� �. �.\r\n\
///////////////////////////////////////////////////////////\r\n\
\r\n\
#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)\r\n\
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_\r\n\
\r\n\
#include \"Defines.h\"\r\n\
\r\n\
/**\r\n\
 * ����� ��� ������ � ������� ���������\r\n\
 *\r\n\
 * ��������. ���� ���� ����������� �������������. ����� ��������\r\n\
 * ����� ��������� �������� ����� �����������.\r\n\
 */\r\n\
class CVersion {\r\n\
\r\n\
public:\r\n\
\r\n\
    inline static uint8_t GetMajor() { return " + Major + "; }\r\n\
    inline static uint8_t GetMinor() { return " + Minor + "; }\r\n\
    inline static uint16_t GetRevision() { return " + Revision + "; }\r\n\
    inline static uint16_t GetBuild() { return " + Build + "; }\r\n\
\r\n\
    inline static FCHAR_PTR GetBuildDateString() {\r\n\
        return SPSTR( \"" + d.toLocaleDateString() + "\" );\r\n\
    }\r\n\
\r\n\
};\r\n\
\r\n\
#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)\r\n\
"
);

SourceFile.Close();
