﻿using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;


namespace objdump.Instructions {

    // BRLO - Branch if Lower (Unsigned).
    // 1111 00kk kkkk k000
    public class brlo: IInstruction {

        public OpInfo info;
        public OpInfo OpInfo { get { return info; } }

        public brlo() { info = new OpInfo( "BRLO", "Branch if Lower (Unsigned)",
            new Regex( @"111100\d{7}000", RegexOptions.Compiled ) ); }

        public string Disassemble( List< Record > list, ref int counter ) {

            string op;

            var item = list[ counter ];

            try {

                // Преобразуем в двоичное представление.
                var code = Convert.ToString( item.OpCode, 2 ).PadLeft( 16, '0' );

                // Узнаём параметры инструкции.
                // 0123 4567 8901 2345
                // 1111 00kk kkkk k000
                // PC <- PC + k + 1
                uint k = Convert.ToByte( code.Substring( 6, 7 ), 2 );

                k = ( uint ) ( ( ( k & ( 1 << 6 ) ) == 0 ) ? ++k << 1 : - ( ~k << 1 & 0x7F ) );

                // Формируем ассемблерный вид команды.

                // Название инструкции.
                op = info.Name.PadRight( Program.ArgumentsPad, ' ' );

                // Параметры.
                op += String.Format( "${0:X4}", item.Address + k ).PadRight( Program.CommentsPad, ' ' );

                // Описание.
                op += info.Description;

            } catch( Exception ex ) {

                // Отладочное сообщение в случае ошибки.
                op = String.Format( "line: {0}, addr: ${1:X4}, opcode: ${2:X4} - {3}",
                    item.LineNumber, item.Address, item.OpCode, ex.Message );

                throw new Exception( op );
            }

            return op;
        }

    }

}
