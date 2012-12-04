#pragma once

// Свойства в С++, Автор: Денис Майдыковский
// http://www.rsdn.ru/article/vcpp/props.xml

// Свойства в C++, Alno’s blog: C++, Java и Rails
// http://blog.alno.name/ru/2008/05/cpp-properties


class CGame {

private:
    uint8_t F_Level;

    uint8_t getLevel() { return F_Level; }
    void setLevel( uint8_t Level ) { F_Level = Level; }
    
public:	    

    // Начало свойств
    properties_start( CGame );
    
        property( CGame, uint8_t, getLevel, setLevel ) Level;
    
    // Конец свойств
    properties_end();

public:
    
    // Конструктор
    CGame();

	void Initialization();    
    void DrawGlass( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
        uint8_t color, uint8_t bgcolor = BLACK );
    void DrawFigure();  
    void Run();

};
