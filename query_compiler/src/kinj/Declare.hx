package kinj;
import kinj.Utils.quoted;
import kinj.Commons.Prop;
import kinj.Commons.prop_to_string;
import kinj.Commons.Fields;


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