#ifndef __SMARTPTR_H__
#define __SMARTPTR_H__

// "AVR, �++ � ����� ���������"
// ������: http://we.easyelectronics.ru/Soft/avr-s-i-umnye-ukazateli.html
// �����: http://we.easyelectronics.ru/profile/neiver/

// ����������:
// ����� ������-�������� ����������� ������������� C++
// Abrahams D., Gurtovoy A. � C++ Template Metaprogramming
// ������������� �. ����������� �������������� �� C++
// ������ �. �������� ���������� ����� �++�

//
// ���� ������ ��������� ��� ���������:
// 1 - ��� ���������� �� ������� ��������� ���������
// 2 - ����� ���������� ������� ������ �� �������� ��������� ������������
template < class T, class Accessor >
class BasePtr {

        // ��������� ��� ������������ ����� ������ ��� ���������
        typedef BasePtr Self;

public:

    BasePtr( T *address = 0 ): _address( address ) {}

    // ����� ���������� ��� �������� �������������� ���� ����������
    // ������������
    inline Self& operator ++() {

            _address++;

            return *this;

    }
        
    // �������������
    inline Self operator ++( int ) {
            
        Self tmp = *this;
        _address++;

        return tmp;

    }
        
    // ������������
    inline Self& operator --() {
            
        _address--;
        
        return *this;

    }
        
    // �������������
    inline Self operator --( int ) {
            
        Self tmp = *this;
        _address--;
        
        return tmp;

    }

    inline Self& operator +=( int value ) {
            
        _address += value;
        
        return *this;

    }

    inline Self& operator -=( int value ) {
            
        _address -= value;
        
        return *this;

    }

    inline Self operator +( int value ) {
            
        return Self( _address + value );

    }

    inline Self operator -( int value ) {
            
        return Self( _address - value );

    }

    inline bool operator !=( const Self &other ) const {
            
        return _address != other._address;

    }

    inline bool operator ==( const Self &other ) const {
            
        return _address == other._address;

    }

    // �������� ���������������� ���������. ����� �������������� ������ �� ���������.
    inline const T operator *() const {
        
        union {
                T value;
                uint8_t bytes[ sizeof(T) ];
        } data;

        for ( unsigned i = 0; i < sizeof(T); ++i ) {
         
            data.bytes[i] = Accessor::Read( ( const uint8_t* const )( _address) + i );

        }

        return data.value;

    }

    inline const T operator []( int value ) {
            
        return * Self( _address + value );

    }

    inline T * operator & () {

        return ( T * ) _address;

    }

private:

    T * _address;

};

template<class T>
class FlashPtr: public BasePtr<T, FlashPtr<T> > {

    public:

        FlashPtr( T *address ): BasePtr< T, FlashPtr<T> >( address ) {}

        static uint8_t Read( const uint8_t *addr ) {

                return pgm_read_byte( addr );

        }

};

template<class T>
class EepromPtr: public BasePtr< T, EepromPtr<T> > {
        
    public:

        EepromPtr( T *address ): BasePtr< T, EepromPtr<T> >( address ) {}

        static uint8_t Read( const uint8_t *addr ) {

                return eeprom_read_byte( addr );

        }

};

#endif
