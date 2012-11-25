/**
* ��������� ������ ���������
*
* �����: �������� �. �.
* �����: mailto:unihomelab@ya.ru
*
**********************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Revision, Build;

// ������ ������ �� ������ ���������� ��������� ������
var objArgs = WScript.Arguments;

// TODO: ������� �������� �� ���������� ����������

// ��������� ������ ���������� �� ���������
var sVersionFile = objArgs(0);

// ������� ��������� ��������� ������
WScript.StdOut.WriteLine( "Version file: " + sVersionFile );

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

// �������� ������
Build++;

if ( Build > 9999 ) {

    Revision++;
    Build = 0;

    if ( Revision > 9999 ) {

        Minor++;
        Revision = 0;

        if ( Minor > 99 ) {

            Major++;
            Minor = 0;

            if ( Major > 9 ) {

                Major = 0;

            }

        }

    }

}

VersionFile = fso.CreateTextFile( sVersionFile, true );

// ������� ��������� �������� ������ ������ ���������
VersionFile.WriteLine( Major );
VersionFile.WriteLine( Minor );
VersionFile.WriteLine( Revision );
VersionFile.WriteLine( Build );

VersionFile.Close();
