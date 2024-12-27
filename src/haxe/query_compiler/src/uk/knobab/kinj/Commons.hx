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
import uk.knobab.kinj.Utils.float_to_number;
import uk.knobab.kinj.Utils.quoted;

@:keep 
enum FieldValue {
    NUMBER(n:Float);
    STRING(s:String);
}

@:keep 
function field_to_string(f:FieldValue):String {
    switch (f) {
        case NUMBER(n): return float_to_number(n);
        case STRING(s): return quoted(s);
    }
}

@:keep 
class Field {
    var var_name:String;
    var value:FieldValue;

    public function new(x,y) {
        this.var_name = x;
        this.value = y;
    }

    public static function stringField(s:String, v:String):Field {
        return new Field(s, STRING(v));
    }
    public static function numberField(s:String, v:Float):Field {
        return new Field(s, NUMBER(v));
    }

    public function toString():String {
        return "var " + quoted(this.var_name) +" = "+field_to_string(this.value);
    }
}

@:keep 
enum Rel {
    LT;
    LEQ;
    GT;
    GEQ;
    EQ;
    NEQ;
}



@:keep 
function rel_to_string(r:Rel) {
    switch (r) {
        case LT: return "<";
        case LEQ: return "<=";
        case GT: return ">";
        case GEQ: return ">=";
        case EQ: return "=";
        case NEQ: return "!=";
    }
}

@:keep 
enum AtomRHS {
    RHSValue(f:FieldValue);
    Var(s:String);
}

@:keep 
function atomrhs_to_string(a:AtomRHS):String {
    switch (a) {
        case RHSValue(f): return field_to_string(f);
        case Var(s): return " var "+ quoted(s);
    }
}

@:keep
class Atom {
    var isNegated:Bool;
    var var_name:String;
    var rel:Rel;
    var rhs:AtomRHS;

    public function new(var_name:String, rel:Rel, rhs:AtomRHS, ?isNegated:Bool=false) {
        this.isNegated = isNegated;
        this.var_name = var_name;
        this.rel = rel;
        this.rhs = rhs;
    }

    public function toString():String {
        var result:String = "";
        if (this.isNegated) result = result+"~";
        return result+" var " + quoted(this.var_name) + " " +rel_to_string(rel)+" "+atomrhs_to_string(this.rhs);
    }
}

@:keep
enum Prop {
    TRUE;
    DisjunctedNormalForm(disj:List<List<Atom>>);
}

@:keep
function prop_to_string(p:Prop):String {
    switch (p) {
        case TRUE: return "true";
        case DisjunctedNormalForm(disj): {
            return disj.map(ls -> ls.map(x->x.toString()).join(" && ")).join(" || ");
        }
    }
}

@:keep
class Fields {
    var label:String;
    var prop:Prop;
    public function new(l:String, p:Prop) {
        this.label = l;
        this.prop = p;
    }
    public function toString():String {
        return quoted(label)+" , "+prop_to_string(prop);
    }
}
