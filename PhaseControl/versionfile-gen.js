/****************************************************
* ������������� ������������� ����� ������ CVersion.
*
* �����: �������� �. �.
* skype: viacheslavmezentsev
* �����: mailto:viacheslavmezentsev@gmail.com
*
* ����: �� 06.10.2013 �.
*
*****************************************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Build, Revision;

// ������ ������ �� ������ ���������� ��������� ������.
var objArgs = WScript.Arguments;

// TODO: ������� �������� �� ���������� ����������.

// ��������� ������ ���������� �� ���������.
var sSourceFile = objArgs(0);
var sVersionFile = objArgs(1);

// ������� ��������� ��������� ������.
WScript.StdOut.WriteLine( "Output file: " +  sSourceFile + ".h" );
WScript.StdOut.WriteLine( "Version file: " +  sVersionFile );

fso = WScript.CreateObject( "Scripting.FileSystemObject" );

ForReading = 1; // ��� ������
TristateFalse = 0; // ������ ASCII

// ��������� ����� ������ �� �����.
VersionFile = fso.OpenTextFile( sVersionFile, ForReading, false, TristateFalse );

try {

    var version = VersionFile.ReadLine().split( "." );

    Major = version[0];
    Minor = version[1];
    Build = version[2];
    Revision = version[3];

} catch(e) {

} finally {

    VersionFile.Close();
}

var d = new Date();

if ( Build == "*" ) {

    // Build - ���������� ���� ����� 1 ������ 2000 ����.
    Build = Math.floor( ( d.getTime() - new Date( 2000, 0, 1 ).getTime() ) / ( 24 * 3600 * 1000 ) );

    // Revision - �������� ���������� ������ ����� �������� ������� �����.
    Revision = Math.floor( ( d.getTime() - new Date( d.getFullYear(),
        d.getMonth(), d.getDate() ).getTime() ) / ( 2 * 1000 ) );

}

// ��������� ������������ ���� ������.
var SourceFile;

SourceFile = fso.CreateTextFile( sSourceFile + ".h", true );

// ������� ����� ������.
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
 * ����� ��� ������ � ������� ���������.\r\n\
 *\r\n\
 * ��������. ���� ���� ����������� �������������. ����� ��������\r\n\
 * ����� ��������� �������� ����� �����������.\r\n\
 */\r\n\
class CVersion {\r\n\
\r\n\
public:\r\n\
\r\n\
    inline static uint32_t GetMajor() { return " + Major + "; }\r\n\
    inline static uint32_t GetMinor() { return " + Minor + "; }\r\n\
    inline static uint32_t GetBuild() { return " + Build + "; }\r\n\
    inline static uint32_t GetRevision() { return " + Revision + "; }\r\n\
\r\n\
    inline static FCHAR_PTR GetVersionString() {\r\n\
        return SPSTR( \"" + Major + "." + Minor + "." + Build + "." + Revision + "\" );\r\n\
    }\r\n\
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
