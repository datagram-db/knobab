package kinj;
import kinj.Declare.model_to_string;
import kinj.Declare.Model;
import kinj.Utils.quoted;

@:keep 
class Atomization {
    @:optional public var with_label:String;
    public var max_strlen:Int;
    @:optional public var strategy:String;

    public function new(?with_label:String, ?max_strlen:Int=0, ?strategy:String) {
        this.with_label = with_label;
        this.max_strlen = max_strlen;
        this.strategy = strategy;
    }

    public function toString():String {
        if ((with_label == null) && (max_strlen>0) && (strategy == null))
            return "";
        else {
            var result:String="atomize! ";
            if (with_label != null) result = result+" with label " + Utils.quoted(with_label);
            if (max_strlen>0) result = result+" maximum strlen "+Std.string(max_strlen);
            if (strategy != null) result = result+" with strategy " + quoted(strategy);
            return result;
        }
    }
}

@:keep 
class Grounding {
    @:optional public var strategy:String;
    public var no_preliminary_fill:Bool;
    public var act_for_attributes:Bool;
    public var no_cream_off:Bool;

    public function new(?strategy:String,?no_preliminary_fill:Bool=false, ?act_for_attributes:Bool=false, ?no_cream_off:Bool=false) {
        this.strategy = strategy;
        this.no_preliminary_fill = no_preliminary_fill;
        this.no_cream_off = no_cream_off;
        this.act_for_attributes = act_for_attributes;
    }

    public function toString():String {
        var result:String="";
        if ((strategy == null) && (!no_cream_off) && (!no_preliminary_fill) && (!act_for_attributes))
            return result;
        result = "grounding! ";
        if (strategy != null) result = result + " with strategy " + Utils.quoted(strategy);
        if (no_preliminary_fill) result = result + " no preliminary fill ";
        if (act_for_attributes) result = result + " no act attributes ";
        if (no_cream_off) result = result + " no cream off ";
        return result;
    }
}

@:keep 
class ModelQuery {
    var model:Model;
    var ensemble:String;
    var env:String;
    @:optional var plan:String;
    @:optional var operators:String;
    @:optional var nThreads:Int;
    var display_qp:Bool;
    var a:Atomization;
    var g:Grounding;

    public function new(m:Model, query_type:String, env:String, ?display:Bool=false) {
        this.model = m;
        this.ensemble = query_type;
        this.env = env;
        this.display_qp = display;
        a = null;
        g = null;
        plan = null;
        operators = null;
        nThreads = -1;
    }

    public function setPlan(x:String) {
        this.plan = x;
    }
    public function setOperators(x:String) {
        this.operators = x;
    }
    public function setNThreads(x:Int) {
        if (x>1) {
            this.nThreads = x;
        } else {
            this.nThreads = -1;
        }
    }
    public function setAtomization(a:Atomization) {
        this.a = a;
    }
    public function emplaceAtomization(?with_label:String, ?max_strlen:Int=0, ?strategy:String) {
        this.a = new Atomization(with_label, max_strlen, strategy);
    }
    public function emplaceGrounding(?strategy:String,?no_preliminary_fill:Bool=false, ?act_for_attributes:Bool=false, ?no_cream_off:Bool=false) {
        this.g = new Grounding(strategy, no_preliminary_fill, act_for_attributes, no_cream_off);
    }
    public function setGrounding(g:Grounding) {
        this.g = g;
    }
    public function toString():String {
        var result:String="model-check " + model_to_string(model) + " using "+ Utils.quoted(ensemble) + " over "+ Utils.quoted(env);
        if (plan != null) result = result + " plan " + Utils.quoted(plan);
        if (operators != null) result = result + " with operators " + Utils.quoted(operators);
        if ((nThreads > 1)) result = result + " multithreaded "+Std.string(nThreads);
        if (display_qp)  result = result + " display query-plan ";
        if (a != null) result = result + a.toString() + " ";
        if (g != null) result = result + g.toString();
        return result;
    }
}