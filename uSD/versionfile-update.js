/*********************************************
* ��������� ������ ���������
*
* �����: �������� �. �.
* �����: mailto:viacheslavmezentsev@gmail.com
*
**********************************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Build, Revision;

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

if ( ( Build != "*" ) && ( Revision != "*" ) ) {

    // �������� ������
    Revision++;

    if ( Revision > 9999 ) {

        Build++;
        Revision = 0;

        if ( Build > 9999 ) {

            Minor++;
            Build = 0;

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
    VersionFile.WriteLine( Major + "." + Minor + "." + Build + "." + Revision );

    VersionFile.Close();

}
