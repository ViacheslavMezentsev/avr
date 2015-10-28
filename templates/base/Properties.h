#pragma once

// Свойства в С++, Автор: Денис Майдыковский
// http://www.rsdn.ru/article/vcpp/props.xml

// Свойства в C++, Alno’s blog: C++, Java и Rails
// http://blog.alno.name/ru/2008/05/cpp-properties

/**
 * Класс, предоставляющий общие сервисы для свойств, а также используемый 
 * для хранения в классе позиции свойств.
 *
 * PropertyOwner - Класс владельца
 */
template < typename PropertyOwner >
class properties {

    public:
        
        // Получить указатель на владельца по указателю на свойство
        static PropertyOwner * owner( void * property ) { 

            int aai = ( int ) & ( ( ( PropertyOwner * ) 0 )->__properties );

            return ( PropertyOwner * ) ( ( char * ) property - aai );

        }

};


/**
 * Шаблон класса свойства.
 *
 * PropertyOwner - Класс владельца
 * PropertyType - Тип свойства
 * getter() - Геттер
 * setter() - Сеттер
 */
template <
    typename PropertyOwner,
    typename PropertyType,
    PropertyType ( PropertyOwner:: * getter ) (),
    void ( PropertyOwner:: * setter ) ( PropertyType ) >
class property {
public:

    /**
     * Чтение свойства - вызов геттера
     */
    operator PropertyType() {
        return ( properties < PropertyOwner >::owner( this )->* getter ) ();
    }

    /**
     * Запись в свойство - вызов сеттера
     */
    void operator = ( const PropertyType & value ) {
        ( properties< PropertyOwner >::owner( this )->* setter ) ( value );
    }

};

// Макросы для удобного определения свойств

/**
 * Начать объявления свойств в классе cls
 */
#define properties_start(cls) union { properties<cls> __properties;

/**
 * Закончить объявление свойств в классе cls
 */
#define properties_end() };

/**
 * Объявить свойство в классе cls типа type c геттером getter и сеттером setter
 */
#define property(cls,type,getter,setter) property<cls,type,&cls::getter,&cls::setter>