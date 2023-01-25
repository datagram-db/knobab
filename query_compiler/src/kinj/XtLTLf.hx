package kinj;
import kinj.Utils.quoted;

@:keep 
enum ATNone {
    Activation;
    Target;
    Neither;
}

@:keep 
function atnone_to_string(x:ATNone):String {
    if (x == null) return "";
    switch(x) {
        case Activation: return "activation";
        case Target: return "target";
        case Neither: return "";
    }
}

@:keep 
enum XtLTLf {
    Init(timed:Bool,nargs:Int,atn:ATNone);
    End(timed:Bool,nargs:Int,atn:ATNone);
    Last(atn:ATNone);
    First(atn:ATNone);
    Exists(negated:Bool,n:Int,timed:Bool,nargs:Int,atn:ATNone);
    Absence(n:Int,timed:Bool,nargs:Int,atn:ATNone);
    Next(x:XtLTLf);
    Or(left:XtLTLf, right:XtLTLf, timed:Bool,theta:Bool,inv:Bool);
    And(left:XtLTLf, right:XtLTLf, timed:Bool,theta:Bool,inv:Bool);
    Impl(left:XtLTLf, right:XtLTLf, timed:Bool,theta:Bool,inv:Bool);
    Ifte(cond:XtLTLf, iftrue:XtLTLf, iffalse:XtLTLf, timed:Bool,theta:Bool,inv:Bool);
    Until(left:XtLTLf, right:XtLTLf, timed:Bool,theta:Bool,inv:Bool);
    Box(x:XtLTLf, timed:Bool);
    Diamond(x:XtLTLf, timed:Bool);
    Negated(x:XtLTLf, timed:Bool, preserve:Bool);
    AndFuture(left:XtLTLf, right:XtLTLf, theta:Bool,inv:Bool);
    AndGlobally(left:XtLTLf, right:XtLTLf, theta:Bool,inv:Bool);
    AndNextGlobally(left:XtLTLf, right:XtLTLf, theta:Bool,inv:Bool);
}

@:keep 
function xtltlf_to_string(x:XtLTLf):String {
    switch (x) {
        case Init(timed, nargs, atn): return "INIT " + (timed ? " t " : "") 
                                                    + (((nargs > 0))? " #"+Std.string(nargs)+" " : " ")
                                                    + atnone_to_string(atn);
        case End(timed, nargs, atn): return "END " + (timed ? " t " : "") 
                                                    + (((nargs > 0))? " #"+Std.string(nargs)+" " : " ")
                                                    + atnone_to_string(atn);
        case Last(atn): return "LAST " + atnone_to_string(atn);
        case First(atn): return "FIRST " + atnone_to_string(atn);
        case Exists(negated, n, timed, nargs, atn): return "EXISTS " + (negated ? " ~ " : " ") 
                                                                     + (((n > 0))? " "+Std.string(n)+" " : " ")
                                                                     + (timed ? " t " : "") 
                                                    + (((nargs > 0))? " #"+Std.string(nargs)+" " : " ")
                                                    + atnone_to_string(atn);
        case Absence(n, timed, nargs, atn): return "ABSENCE " + (((n > 0))? " "+Std.string(n)+" " : " ")
                                                                     + (timed ? " t " : "") 
                                                    + (((nargs > 0))? " #"+Std.string(nargs)+" " : " ")
                                                    + atnone_to_string(atn);
        case Next(x): return "NEXT "+xtltlf_to_string(x);
        case Or(left, right, timed, theta, inv): return xtltlf_to_string(left)+" OR "
                                                        + (timed ? " t " : "") 
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case And(left, right, timed, theta, inv): return xtltlf_to_string(left)+" AND "
                                                        + (timed ? " t " : "") 
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case Impl(left, right, timed, theta, inv): return xtltlf_to_string(left)+" AND "
                                                        + (timed ? " t " : "") 
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case Ifte(cond, iftrue, iffalse, timed, theta, inv): return "IF "+ (timed ? " t " : "") 
                                                                        +xtltlf_to_string(cond)
                                                                        + " THEN "
                                                                        + xtltlf_to_string(iftrue)
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + " ELSE "
                                                        + xtltlf_to_string(iffalse);
        case Until(left, right, timed, theta, inv): return xtltlf_to_string(left)+" U "
                                                        + (timed ? " t " : "") 
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case Box(x, timed): return "G "+ (timed ? " t " : "") + xtltlf_to_string(x);
        case Diamond(x, timed): return "F "+ (timed ? " t " : "") + xtltlf_to_string(x);
        case Negated(x, timed, preserve): return "~ "+ (timed ? " t " : "") + xtltlf_to_string(x) + (preserve ? " PRESERVE ": "");
        case AndFuture(left, right, theta, inv): return xtltlf_to_string(left)+" &Ft "
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case AndGlobally(left, right, theta, inv): return xtltlf_to_string(left)+" &Gt "
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        case AndNextGlobally(left, right, theta, inv): return xtltlf_to_string(left)+" &XGt "
                                                        + (theta ? " THETA " : "")
                                                        + (inv ? " INV " : "")
                                                        + xtltlf_to_string(right);
        
    }
    return "";
}

@:keep 
class DeclareSyntax {
    var template:String;
    var hasArgs:Int;
    var expr:XtLTLf;

    public function new(template:String, expr:XtLTLf, ?hasArgs:Int=-1) {
        this.template = template;
        this.hasArgs = hasArgs;
        this.expr = expr;
    }

    public function toString():String {
        return "template " + quoted(template) + ((hasArgs>0) ? (" args "+Std.string(hasArgs)) : " ") + " := " + xtltlf_to_string(expr);
    }
}

@:keep 
class QueryPlan {
    var autoTimed:Bool;
    var name:String;
    var declare_semantics:List<DeclareSyntax>;

    public function new(name:String, ds:List<DeclareSyntax>, ?autoTime:Bool=true) {
        this.autoTimed = autoTime;
        this.name = name;
        this.declare_semantics = ds;
    }

    public static function alloc(name:String, ?autoTime:Bool=true):QueryPlan {
        return new QueryPlan(name, new List<DeclareSyntax>(),autoTime);
    }
    public function emplace_back(template:String, expr:XtLTLf, ?hasArgs:Int=-1) {
        declare_semantics.add(new DeclareSyntax(template, expr, hasArgs));
    }
    public function add(x : DeclareSyntax):Void {
        declare_semantics.add(x);
    }

    public function toString():String {
        return (autoTimed? "auto-timed ": "") + " queryplan " + Utils.quoted(name) + " { " + declare_semantics.map(x->x.toString()).join(" ") + " } ";
    }
}