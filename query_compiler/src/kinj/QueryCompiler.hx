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
 package kinj;
import kinj.Log.DumpLog;
import kinj.XtLTLf.QueryPlan;
import kinj.Log.logformat_to_string;
import kinj.Log.LogFormat;
import kinj.Display.displaytable_to_string;
import kinj.Display.displaylist_to_string;
import kinj.Utils.quoted;
import kinj.Display.Displays;

@:keep 
enum QueryCompiler {
    SetBenchmarkingFile(filename:String);
    LoadDataQuery(l:LogFormat, file:String, env:String, withData:Bool, noStats:Bool, withMissing:Bool);
    DisplayData(l:Displays);
    DoDumpLog(dl:DumpLog);
    DoModelQuery(m:ModelQuery);
    DoQueryPlan(q:QueryPlan);
    DoWithModel(m:WithModel);
}

@:keep 
function querycompiler_to_query(x:QueryCompiler) {
    switch (x) {
        case SetBenchmarkingFile(filename):    return "benchmarking-log " + quoted(filename);
        case LoadDataQuery(l, file, env, withData, noStats, withMissing): {
            var result:String= "load " + logformat_to_string(l) + " " + quoted(file);
            if (withData) result= result+" with data ";
            if (noStats) result=result+" no stats ";
            if (withMissing) result=result+" with missing ";
            return result+" as "+quoted(env);
        }
        case DisplayData(l): {
            switch (l) {
                case Display(d,f): return "display " + displaytable_to_string(d) + " for " + quoted(f);
                case List(d, f): return "list " + displaylist_to_string(d) + " for " + quoted(f);
                case DropLog(e): return "droplog " + quoted(e);
            }
        }
        case DoDumpLog(dl): return dl.toString();
        case DoModelQuery(m): return m.toString();
        case DoQueryPlan(q): return q.toString();
        case DoWithModel(m): return m.toString();
    }
}