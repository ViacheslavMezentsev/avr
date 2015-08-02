/****************************************************
* Формирователь заголовочного файла модуля CVersion.
*
* Автор: Мезенцев В. Н.
* skype: viacheslavmezentsev
* Почта: mailto:viacheslavmezentsev@gmail.com
*
* Дата: Пт 13.02.2015 г.
*
*****************************************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Build, Revision;

// Создаём ссылку на массив параметров командной строки.
var objArgs = WScript.Arguments;

// TODO: Сделать проверку на количество параметров.

// Назначаем каждой переменной по параметру.
var sSourceFile = objArgs(0);
var sVersionFile = objArgs(1);

// Выводим параметры командной строки.
WScript.StdOut.WriteLine( "Output file: " +  sSourceFile );
WScript.StdOut.WriteLine( "Version file: " +  sVersionFile );

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

var d = new Date();

if ( Build == "*" ) {

    // Build - количество дней после 1 января 2000 года.
    Build = Math.floor( ( d.getTime() - new Date( 2000, 0, 1 ).getTime() ) / ( 24 * 3600 * 1000 ) );

    // Revision - половина количества секунд после полуночи текущих суток.
    Revision = Math.floor( ( d.getTime() - new Date( d.getFullYear(),
        d.getMonth(), d.getDate() ).getTime() ) / ( 2 * 1000 ) );

}

// Формируем заголовочный файл модуля.
var SourceFile;

SourceFile = fso.CreateTextFile( sSourceFile, true );

// Выводим шапку модуля.
SourceFile.Write(
"\
$nocompile\r\n\
\r\n\
' Модуль: Version_Header.bas\r\n\
' Создан: " +
d.toLocaleDateString() + " " + d.toLocaleTimeString() +
"\r\n\
' Автор: Мезенцев В. Н.\r\n\
'\r\n\
' Модуль для автоматизированной работы с версией программы.\r\n\
'\r\n\
' Внимание. Этот файл формируется автоматически. Любые внесённые\r\n\
' здесь изменения пропадут перед компиляцией.\r\n\
\r\n\
'Const VERSION_ENABLED = 0 ' +/- ... байт(а)\r\n\
\r\n\
#if VERSION_ENABLED\r\n\
\r\n\
Declare Function Version_GetMajor() As DWORD\r\n\
Declare Function Version_GetMinor() As DWORD\r\n\
Declare Function Version_GetBuild() As DWORD\r\n\
Declare Function Version_GetRevision() As DWORD\r\n\
Declare Function Version_GetVersionString() As String\r\n\
Declare Function Version_GetBuildDateString() As String\r\n\
\r\n\
Function Version_GetMajor() As DWORD : Version_GetMajor = " + Major + " : End Function\r\n\
Function Version_GetMinor() As DWORD : Version_GetMinor = " + Minor + " : End Function\r\n\
Function Version_GetBuild() As DWORD : Version_GetBuild = " + Build + " : End Function\r\n\
Function Version_GetRevision() As DWORD : Version_GetRevision = " + Revision + " : End Function\r\n\
Function Version_GetVersionString() As String : Version_GetVersionString = \"" + Major + "." + Minor + "." + Build + "." + Revision + "\" : End Function\r\n\
Function Version_GetBuildDateString() As String : Version_GetBuildDateString = \"" + d.toLocaleDateString() + "\" : End Function\r\n\
\r\n\
#endif\r\n\
"
);

SourceFile.Close();
