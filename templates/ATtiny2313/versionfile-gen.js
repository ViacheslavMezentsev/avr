/****************************************************
* Формирователь заголовочного файла класса CVersion.
*
* Автор: Мезенцев В. Н.
* skype: viacheslavmezentsev
* Почта: mailto:viacheslavmezentsev@gmail.com
*
* Дата: Вс 06.10.2013 г.
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
WScript.StdOut.WriteLine( "Output file: " +  sSourceFile + ".h" );
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

// Формируем заголовочный файл класса.
var SourceFile;

SourceFile = fso.CreateTextFile( sSourceFile + ".h", true );

// Выводим шапку класса.
SourceFile.Write(
"\
///////////////////////////////////////////////////////////\r\n\
//  Version.h\r\n\
//  Implementation of the Class CVersion\r\n\
//  Created on:      " +
d.toLocaleDateString() + " " + d.toLocaleTimeString() +
"\r\n\
//  Original author: Мезенцев В. Н.\r\n\
///////////////////////////////////////////////////////////\r\n\
\r\n\
#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)\r\n\
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_\r\n\
\r\n\
#include \"Defines.h\"\r\n\
\r\n\
/**\r\n\
 * Образ для работы с версией программы.\r\n\
 *\r\n\
 * Внимание. Этот файл формируется автоматически. Любые внесённые\r\n\
 * здесь изменения пропадут перед компиляцией.\r\n\
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
