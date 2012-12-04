#pragma once


/**
 * Класс, предоставляющий общие сервисы для свойств, а также используемый для хранения в классе позиции свойств.
 */
template <
    typename PropertyOwner // Класс владельца
>
class properties {

    public:
        
        // Получить указатель на владельца по указателю на свойство
        static PropertyOwner * owner( void * property ) { 

            int aai = (int)&(((PropertyOwner*)0)->__properties);

            return (PropertyOwner *)((char*)property - aai);

        }

};


/**
 * Шаблон класса свойства
 */
template <
    typename PropertyOwner, // Класс владельца
    typename PropertyType, // Тип свойства
    PropertyType (PropertyOwner::*getter)(), // Геттер
    void (PropertyOwner::*setter)(PropertyType) > // Сеттер
class property {
public:

    /**
     * Чтение свойства - вызов геттера
     */
    operator PropertyType() {
        return (properties<PropertyOwner>::owner( this )->*getter)();
    }

    /**
     * Запись в свойство - вызов сеттера
     */
    void operator = ( const PropertyType & value ) {
        (properties<PropertyOwner>::owner( this )->*setter)( value );
    }
};

// Макросы для удобного определения свойств /////////

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