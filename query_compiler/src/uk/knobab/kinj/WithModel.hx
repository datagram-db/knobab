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
import uk.knobab.kinj.Declare.model_to_string;
import uk.knobab.kinj.Log.DumpLog;
import uk.knobab.kinj.ModelQuery.Grounding;
import uk.knobab.kinj.ModelQuery.Atomization;
import uk.knobab.kinj.Utils.float_to_number;
import uk.knobab.kinj.Utils.quoted;
import uk.knobab.kinj.Log.logformat_to_string;
import uk.knobab.kinj.Declare.Model;
import uk.knobab.kinj.Log.LogFormat;

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
