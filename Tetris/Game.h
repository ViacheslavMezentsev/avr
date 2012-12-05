#pragma once


class CKeys {
   
public:	

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( "������" );
            case 2: return SPSTR( "����� ����" );
            case 9: return SPSTR( "�����" );
            
            default: return 0;

        }        

    }

};


class CGame {

private:
    uint8_t F_Level;

    uint8_t getLevel() { return F_Level; }
    void setLevel( uint8_t Level ) { F_Level = Level; }
    
public:	    

    // ������ �������
    properties_start( CGame );
    
        property( CGame, uint8_t, getLevel, setLevel ) Level;
    
    // ����� �������
    properties_end();

public:
    
    // �����������
    CGame();

	void Initialization();    
    void DrawFrame( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
        uint8_t color, uint8_t bgcolor = BLACK );
    void DrawFigure();  
    void DrawFunctionKeys( CKeys & Keys );
    void Run();

};


