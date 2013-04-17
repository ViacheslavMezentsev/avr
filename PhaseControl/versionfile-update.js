/**
* Изменение версии программы
*
* Автор: Мезенцев В. Н.
* Почта: mailto:unihomelab@ya.ru
*
**********************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Revision, Build;

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

// Считываем номер версии из файла
VersionFile = fso.OpenTextFile( sVersionFile, ForReading, false, TristateFalse );

Major = VersionFile.ReadLine();
Minor = VersionFile.ReadLine();
Revision = VersionFile.ReadLine();
Build = VersionFile.ReadLine();

VersionFile.Close();

// Изменяем версию
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

// Выводим изменённые значения номера версии программы
VersionFile.WriteLine( Major );
VersionFile.WriteLine( Minor );
VersionFile.WriteLine( Revision );
VersionFile.WriteLine( Build );

VersionFile.Close();
