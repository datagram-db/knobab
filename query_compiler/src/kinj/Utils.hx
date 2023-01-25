package kinj;
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