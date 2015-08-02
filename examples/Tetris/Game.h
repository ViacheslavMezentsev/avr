#pragma once


enum EnGameState {
    
    gsStopped = 0,
    gsRunning

};


class CKeys {
   
public:	

    inline uint8_t Count() { return 3; }

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( " 0 |Помощь" );
            case 1: return SPSTR( " 2 |Новая игра" );
            case 2: return SPSTR( " Esc |Выход" );
            
            default: return 0;

        }        

    }

};


class CGame {

private:
    uint8_t level;
    EnGameState state;

    uint8_t getLevel() { return level; }
    void setLevel( uint8_t Level ) { level = Level; }

    EnGameState getState() { return state; }
    void setState( EnGameState State ) { state = State; }

    inline void FormActivate();
    inline void FormKeyDown( uint16_t Key );
    inline void Form10msTimer();
    inline void Form100msTimer();
    inline void Form500msTimer();
    inline void Form1secTimer();
    inline void Form5secTimer();

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
    void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor = clBlack );
    void DrawGlass( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor = clBlack );
    void DrawFigure();  
    void DrawFunctionKeys( CKeys & Keys );
    void Run();

    LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};


