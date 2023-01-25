package kinj;
import kinj.Declare.model_to_string;
import kinj.Log.DumpLog;
import kinj.ModelQuery.Grounding;
import kinj.ModelQuery.Atomization;
import kinj.Utils.float_to_number;
import kinj.Utils.quoted;
import kinj.Log.logformat_to_string;
import kinj.Declare.Model;
import kinj.Log.LogFormat;

@:keep 
class SampleTo {
    var sampleOutputType:LogFormat;
    var filename:String;
    var minL:Int;
    var maxL:Int;
    var logSize:Int;
    var ratioSample:Float;

    public function new(isXes:Bool, filename:String, minL:Int, maxL:Int, logSize:Int, ?ratioSample:Float=-1) {
        this.sampleOutputType = isXes? LogFormat.XES : LogFormat.TAB;
        this.filename = filename;
        this.minL = minL;
        this.maxL = maxL;
        this.logSize = logSize;
        this.ratioSample = ratioSample;
    }

    public function toString():String {
        return 'sample-to ' + logformat_to_string(sampleOutputType) + quoted(filename)
                            + " min-length" + Std.string(minL) + " max-length " + Std.string(maxL) 
                            + " n-traces " + Std.string(logSize) + (ratioSample<0.0 ? "" : (" sample "+float_to_number(ratioSample)));
    }
}

@:keep 
class WithModel {
    var model:Model;
    var cachePath:String;
    @:optional var doSample:SampleTo;
    @:optional var hasAlignmentStrategy:String;
    @:optional var graphDotFile:String;
    @:optional var a:Atomization;
    @:optional var g:Grounding;
    @:optional var d:DumpLog;

    public function new(model:Model, 
                        cachePath:String, 
                        ?doSample:SampleTo,
                        ?hasAlignment:String,
                        ?graphDotFile:String,
                        ?a:Atomization,
                        ?g:Grounding,
                        ?d:DumpLog) {
        this.model = model;
        this.cachePath = cachePath;
        this.doSample = doSample;
        this.hasAlignmentStrategy = hasAlignment;
        this.graphDotFile = graphDotFile;
        this.a = a;
        this.g = g;
        this.d = d;
    }

    public function toString():String {
        var result:String='model-log '+ model_to_string(model) + ' from-automata-in ' + Utils.quoted(cachePath) + " ";
        if (doSample != null) {
            result += doSample.toString();
        }
        if (hasAlignmentStrategy != null) {
            result += ' with-alignment-strategy ' + Utils.quoted(hasAlignmentStrategy);
        }
        if (graphDotFile != null) {
            result += ' serialize-dot-to ' + Utils.quoted(graphDotFile);
        }
        if (this.a != null) result += a.toString() + " ";
        if (this.g != null) result += g.toString() + " ";
        if (this.d != null) result += d.toString();
        return result;
    }
}