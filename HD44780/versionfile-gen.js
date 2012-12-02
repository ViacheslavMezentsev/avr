/**
* Формирователь заголовочного файлы класса CVersion
*
* Шаблон: ATmega32
*
* Автор: Мезенцев В. Н.
* skype: viacheslavmezentsev
* Почта: mailto:unihomelab@ya.ru
*
* Дата: Вс 02.10.2011 г.
*
**********************************/

var fso, VersionFile, ForReading, TristateTrue, Text;
var Major, Minor, Revision, Build;

// Создаём ссылку на массив параметров командной строки
var objArgs = WScript.Arguments;

// TODO: Сделать проверку на количество параметров

// Назначаем каждой переменной по параметру
var sSourceFile = objArgs(0);
var sVersionFile = objArgs(1);

// Выводим параметры командной строки
WScript.StdOut.WriteLine( "Output file: " +  sSourceFile + ".h" );
WScript.StdOut.WriteLine( "Version file: " +  sVersionFile );

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

// Формируем заголовочный файл класса
var SourceFile;

SourceFile = fso.CreateTextFile( sSourceFile + ".h", true );

var d = new Date();

// Выводим шапку класса
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
 * Образ для работы с версией программы\r\n\
 *\r\n\
 * Внимание. Этот файл формируется автоматически. Любые внесённые\r\n\
 * здесь изменения пропадут перед компиляцией.\r\n\
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
