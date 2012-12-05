#pragma once

// Figures: I, J, L, O, S, T, Z
class CFigureI {
   
public:	

    inline FCHAR_PTR operator []( uint8_t n ) {
        
        switch ( n ) {

            case 0: return SPSTR( 
                    "  xx    "
                    "  xx    "
                    "  xx    "
                    "  xx    "
                    );

            case 1: return SPSTR( 
                    "        "
                    "xxxxxxxx"
                    "        "
                    "        "
                    );

            case 2: return SPSTR( 
                    "    xx  "
                    "    xx  "
                    "    xx  "
                    "    xx  "
                    );

            case 3: return SPSTR( 
                    "        "
                    "xxxxxxxx"
                    "        "
                    "        "
                    );

            default: return 0;

        }        

    }

};


class CFigureJ {
   
public:	

    inline FCHAR_PTR operator []( uint8_t n ) {
        
        switch ( n ) {

            case 0: return SPSTR( 
                    "  xx    "
                    "  xxxxxx"
                    "        "
                    "        "
                    );

            case 1: return SPSTR( 
                    "  xxxx  "
                    "  xx    "
                    "  xx    "
                    "        "
                    );

            case 2: return SPSTR( 
                    "  xxxxxx"
                    "      xx"
                    "        "
                    "        "
                    );

            case 3: return SPSTR( 
                    "   xx   "
                    "   xx   "
                    " xxxx   "
                    "        "
                    );

            default: return 0;

        }        

    }

};


class CFigureL {
   
public:	

    inline FCHAR_PTR operator []( uint8_t n ) {
        
        switch ( n ) {

            case 0: return SPSTR( 
                    "      xx"
                    "  xxxxxx"
                    "        "
                    "        "
                    );

            case 1: return SPSTR( 
                    "  xx    "
                    "  xx    "
                    "  xxxx  "
                    "        "
                    );

            case 2: return SPSTR( 
                    "  xxxxxx"
                    "  xx    "
                    "        "
                    "        "
                    );

            case 3: return SPSTR( 
                    "   xxxx "
                    "     xx "
                    "     xx "
                    "        "
                    );

            default: return 0;

        }        

    }

};

