#pragma once


enum EnGameState {
    
    gsStopped = 0,
    gsRunning

};


class CKeys {
   
public:	

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( "Помощь" );
            case 2: return SPSTR( "Новая игра" );
            case 9: return SPSTR( "Выход" );
            
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

    // Начало свойств
    properties_start( CGame );
    
        property( CGame, uint8_t, getLevel, setLevel ) Level;
        property( CGame, EnGameState, getState, setState ) State;
    
    // Конец свойств
    properties_end();

public:
    
    // Конструктор
    CGame();

    void Initialization();    
    void DrawTitle();
    void DrawFrame( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
        uint8_t color, uint8_t bgcolor = BLACK );
    void DrawGlass( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
        uint8_t color, uint8_t bgcolor = BLACK );
    void DrawFigure();  
    void DrawFunctionKeys( CKeys & Keys );
    void Run();

};


