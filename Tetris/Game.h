#pragma once


enum EnGameState {
    
    gsStopped = 0,
    gsRunning

};


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
    EnGameState F_State;

    uint8_t getLevel() { return F_Level; }
    void setLevel( uint8_t Level ) { F_Level = Level; }

    EnGameState getState() { return F_State; }
    void setState( EnGameState State ) { F_State = State; }

public:	    

    // ������ �������
    properties_start( CGame );
    
        property( CGame, uint8_t, getLevel, setLevel ) Level;
        property( CGame, EnGameState, getState, setState ) State;
    
    // ����� �������
    properties_end();

public:
    
    // �����������
    CGame();

    void Initialization();    
    void DrawTitle();
    void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        uint8_t Color, uint8_t bgColor = BLACK );
    void DrawGlass( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        uint8_t Color, uint8_t bgColor = BLACK );
    void DrawFigure();  
    void DrawFunctionKeys( CKeys & Keys );
    void Run();

};


