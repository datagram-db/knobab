/*
 * GNU General Public License, Version 3.0
 *
 * Copyright (c) 2023 Giacomo Bergami
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 package uk.knobab.kinj;
 /**
  * provides the double quote to a string
  */
  @:keep 
  function quoted(s:String):String {
    var i = 0;
    var l = s.length;
    var quoteEscaped:String = '\\"';
    var result:String = '"';
    for (i in 0...l) {
        var c = s.charAt(i);
        if (c == '"')
            result = result+quoteEscaped;
        else 
            result = result+c;
    }
    result = result+'"';
    return result;
}

@:keep 
function float_to_number(n:Float):String {
    var after_dot = Std.string(Std.int(n*10.0));
    var l = 10-after_dot.length;
    var begin = Std.string(Std.int(n))+".";
    for (i in 0...l) begin = begin+"0";
    return begin+after_dot;
}
