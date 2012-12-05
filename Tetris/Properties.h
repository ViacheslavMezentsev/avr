#pragma once

// �������� � �++, �����: ����� ������������
// http://www.rsdn.ru/article/vcpp/props.xml

// �������� � C++, Alno�s blog: C++, Java � Rails
// http://blog.alno.name/ru/2008/05/cpp-properties

/**
 * �����, ��������������� ����� ������� ��� �������, � ����� ������������ 
 * ��� �������� � ������ ������� �������.
 *
 * PropertyOwner - ����� ���������
 */
template < typename PropertyOwner >
class properties {

    public:
        
        // �������� ��������� �� ��������� �� ��������� �� ��������
        static PropertyOwner * owner( void * property ) { 

            int aai = ( int ) & ( ( ( PropertyOwner * ) 0 )->__properties );

            return ( PropertyOwner * ) ( ( char * ) property - aai );

        }

};


/**
 * ������ ������ ��������.
 *
 * PropertyOwner - ����� ���������
 * PropertyType - ��� ��������
 * getter() - ������
 * setter() - ������
 */
template <
    typename PropertyOwner,
    typename PropertyType,
    PropertyType ( PropertyOwner:: * getter ) (),
    void ( PropertyOwner:: * setter ) ( PropertyType ) >
class property {
public:

    /**
     * ������ �������� - ����� �������
     */
    operator PropertyType() {
        return ( properties < PropertyOwner >::owner( this )->* getter ) ();
    }

    /**
     * ������ � �������� - ����� �������
     */
    void operator = ( const PropertyType & value ) {
        ( properties< PropertyOwner >::owner( this )->* setter ) ( value );
    }

};

// ������� ��� �������� ����������� �������

/**
 * ������ ���������� ������� � ������ cls
 */
#define properties_start(cls) union { properties<cls> __properties;

/**
 * ��������� ���������� ������� � ������ cls
 */
#define properties_end() };

/**
 * �������� �������� � ������ cls ���� type c �������� getter � �������� setter
 */
#define property(cls,type,getter,setter) property<cls,type,&cls::getter,&cls::setter>