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
import uk.knobab.kinj.Commons.FieldValue;
import uk.knobab.kinj.Commons.Field;

@:keep 
class Datapart {
    var field_list:List<Field>;

    public function new() {
        field_list = new List<Field>();
    }

    public function add(x:Field):Void {
        field_list.add(x);
    }
    public function emplace_back(f:String, fv:FieldValue) {
        field_list.add(new Field(f, fv));
    }
    public function emplace_string(f:String, s:String) {
        field_list.add(Field.stringField(f,s));
    }
    public function emplace_number(f:String, s:Float) {
        field_list.add(Field.numberField(f,s));
    }
    public function size():Int {
        return field_list.length;
    }

    public function toString():String {
        return "{"+field_list.map(x->x.toString()).join(" ")+"}";
    }

    public function isEmpty():Bool {
        return field_list.isEmpty();
    }
}

@:keep 
class Event {
    var activity_label:String;
    var data:Datapart;

    public function new(activity_label:String, data:Datapart) {
        this.activity_label = activity_label;
        this.data = data;
    }
    public static function alloc(lable:String):Event {
        return new Event(lable, new Datapart());
    }

    public function toString():String {
        var result = this.activity_label.charAt(0).toUpperCase()+this.activity_label.substring(1, this.activity_label.length);
        if (data.isEmpty()) return result;
        else return result+ " { " + data.toString() + "}";
    }
}

@:keep 
class Trace {
    var data:Datapart;
    var event_list:List<Event>;

    public function new(d:Datapart, el:List<Event>) {
        this.data = d;
        this.event_list = el;
    }
    public static function alloc():Trace {
        return new Trace(new Datapart(), new List<Event>());
    }
    public static function allocWith(d:Datapart):Trace {
        return new Trace(d, new List<Event>());
    }
    public function add(e:Event):Void {
        event_list.add(e);
    }
    public function emplace_backV(activity_label:String, data:Datapart) {
        event_list.add(new Event(activity_label, data));
    }

    public function emplace_back(activity_label:String) {
        event_list.add(Event.alloc(activity_label));
    }
    public function traceLength():Int {
        return event_list.length;
    }
    public function toString():String {
        return data.toString() + " : " + event_list.map(x->x.toString()).join(" ");
    }
}

@:keep 
class Log {
    var traces:List<Trace>;
    public function new(t:List<Trace>) {
        this.traces = t;
    }
    public static function alloc():Log {
        return new Log(new List<Trace>());
    }
    public function add(t:Trace):Void {
        traces.add(t);
    }
    public function emplace_back(d:Datapart, el:List<Event>) {
        traces.add(new Trace(d, el));
    }
    public function emplace_backV() {
        traces.add(Trace.alloc());
    }
    public function emplace_backD(d:Datapart) {
        traces.add(Trace.allocWith(d));
    }
    public function toString():String {
        return "log {" + traces.map(x->x.toString()).join("; ") +"}"; 
    }
    public function logSize():Int {
        return traces.length;
    }
}

@:keep 
enum LogFormat {
    XES;
    HRF;
    TAB;
    LLOG(l:Log);
}

@:keep 
function logformat_to_string(l:LogFormat):String {
    switch(l) {
        case XES: return "XES";
        case HRF: return "HRF";
        case TAB: return "TAB";
        case LLOG(l): return l.toString();
    }
}

@:keep 
class DumpLog {
    var env:String;
    var format:LogFormat;
    var file:String;

    public function new(env:String, fo:LogFormat, f:String) {
        this.env = env;
        this.format = fo;
        this.file = f;
    }

    public function toString():String {
        var result:String= "dump-env " + quoted(env) + " as ";
        switch (format) {
            case XES: result = result + " XES "; 
            case TAB: result = result + " TAB "; 
            case HRF: throw "ERROR: HRF is not a supported dump log format!";
            case LLOG(l): throw "ERROR: log data is not a valid log format!";
        }
        return result + " in "+ quoted(file);
    }
}
