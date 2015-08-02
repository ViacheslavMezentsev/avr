/*********************************************
* Изменение версии программы
*
* Автор: Мезенцев В. Н.
* Почта: mailto:viacheslavmezentsev@gmail.com
*
**********************************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Build, Revision;

// Создаём ссылку на массив параметров командной строки
var objArgs = WScript.Arguments;

// TODO: Сделать проверку на количество параметров

// Назначаем каждой переменной по параметру
var sVersionFile = objArgs(0);

// Выводим параметры командной строки
WScript.StdOut.WriteLine( "Version file: " + sVersionFile );

fso = WScript.CreateObject( "Scripting.FileSystemObject" );

ForReading = 1; // Для чтения
TristateFalse = 0; // Формат ASCII

// Считываем номер версии из файла.
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

    // Изменяем версию
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

    // Выводим изменённые значения номера версии программы
    VersionFile.WriteLine( Major + "." + Minor + "." + Build + "." + Revision );

    VersionFile.Close();

}
