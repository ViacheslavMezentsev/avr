/*
* Проект: ATmega16 + Petit FAT File System Module
* Контроллер: ATmega16 @ 16 МГц
*
* Для отладки на модели необходимо загрузить моделируемую схему
* в ISIS, выбрать там для контроллера загрузочный файл:
* - gccout.elf, если собирали проект в VS2008/VS2010 (avrgcc);
* - iccout_ubrof8.d90, если собирали проект в IAR (iccavr).
*
*  Файлы находятся в корне проекта (после компиляции).
*
* Внимание. Перед компиляцией внимательно проверьте опции в
* makefile:
* - какие модули подключены (CPPSRC =);
* - External Memory Options для gcc (внешняя ОЗУ);
* - уровень оптимизации (0, 1, 2, 3, s).
*
* TODO:
*
*/

#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "MCU.h"
#include "FileManager.h"


// Точка входа в основную программу
int main() {

    // Инициализация микроконтроллера
    CMCU::Initialization();

    CFileManager::Initialization();

    // Запуск основного потока
    return CFileManager::WinMain();

}
