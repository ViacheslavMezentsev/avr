#pragma once

// �������� � �++, �����: ����� ������������
// http://www.rsdn.ru/article/vcpp/props.xml

// �������� � C++, Alno�s blog: C++, Java � Rails
// http://blog.alno.name/ru/2008/05/cpp-properties


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
    void DrawGlass( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
        uint8_t color, uint8_t bgcolor = BLACK );
    void DrawFigure();  
    void Run();

};
