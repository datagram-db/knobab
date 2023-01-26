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
import uk.knobab.kinj.Utils.quoted;
import uk.knobab.kinj.Commons.Prop;
import uk.knobab.kinj.Commons.prop_to_string;
import uk.knobab.kinj.Commons.Fields;


@:keep 
enum Declare {
    Unary(name:String, field:Fields, int:Int);
    NAry(name:String, fields:List<Fields>, where:Prop);
}

@:keep 
function declare_to_string(d:Declare):String {
    switch (d) {
        case Unary(name, field, int): 
            return quoted(name)+"("+field.toString()+", "+Std.string(int)+")";
        case NAry(name, fields, where): {
            var result = quoted(name)+"(";
            result = result + fields.map(x->x.toString()).join(", ")+")";
            switch (where) {
                case TRUE:
                    return result;
                case DisjunctedNormalForm(disj):
                    return result+" where " + prop_to_string(where);
            }
        }    
    }
}

@:keep 
enum Model {
    ModelFile(f:String);
    DeclareModel(conj:List<Declare>);
    Template(name:String, i:Int);
}

@:keep 
function model_to_string(x:Model):String {
    switch (x) {
        case ModelFile(f): return "file "+Utils.quoted(f);
        case DeclareModel(conj): return "declare "+conj.map(x->declare_to_string(x)).join(" ");
        case Template(name, i): return "tempalte "+Utils.quoted(name)+" logtop "+Std.string(i);
    }
}
