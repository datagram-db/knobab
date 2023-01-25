package kinj;
import haxe.Json;
import kinj.Declare.Model;
import haxe.macro.Expr.Case;
import kinj.QueryCompiler.querycompiler_to_query;
import haxe.Http;

@:keep 
enum KINJAnswerType {
    MessageResult;
    CSVResult;
    DumpResponse;
    ModelQueryResponse;
    FutureRawString;
    None;
}

@:keep 
class DumpResponse {
    public var to:String;
    public var format:String;
    public var time:Float;
}

@:keep 
class TraceIntersectionResult  {
    public var trace_id:Int;
    public var event_id:Int;
    public var probability:Float;
    public var conditions:List<String>;
}

@:keep 
class ModelQueryResponseT {
    public var model_declare_to_ltlf:Float;
    public var model_ltlf_query_time:Float;
    public var is_multithreaded:Bool;
    public var no_threads:Int;
    public var PerDeclareSupport:List<Float>;
    public var PerDeclareConfidence:List<Float>;
    public var TraceMaximumSatisfiability:List<Float>;
    public var TraceIntersection:List<TraceIntersectionResult>;
    public var log_filename:String;
    public var with_data:Bool;
    public var log_loading_and_parsing_ms:Float;
    public var log_indexing_ms:Float;
    public var n_traces:Int;
    public var n_acts:Int;
    public var log_trace_average_length:Float;
    public var log_trace_variance:Float;
    public var most_frequent_trace_length:Int;
    public var trace_length_frequency:Float;
    public var atomization_conf:String;
    public var model_filename:String;
    public var queries_plan:String;
    public var operators_version:String;
    public var model_parsing_ms:Float;
    public var model_size:Int;
    public var model_data_decomposition_time:Float;
    public var model_atomization_time:Float;
    public var strategyForScheduling:String;
    public function new() {
        
    }
}

@:keep 
var global_CSV = new Csv(",");

@:keep 
class KINJ_ANSWER {
    public var t:KINJAnswerType;
    public var message:String;
    public var to:String;
    public var format:String;
    public var unique_time:Float;
    public var model_query:ModelQueryResponseT;

    public function new(t:KINJAnswerType) {
        this.t = t;
        message = null;
    }

    public static function asMessage(msg:String):KINJ_ANSWER {
        var ans:KINJ_ANSWER = new KINJ_ANSWER(MessageResult);
        ans.message = msg;
        return ans;
    }
    
    public static function asCSV(msg:String):KINJ_ANSWER {
        var ans:KINJ_ANSWER = new KINJ_ANSWER(CSVResult);
        ans.message = msg;
        return ans;
    }

    public static function asDumpResponse(d:DumpResponse) {
        var ans:KINJ_ANSWER = new KINJ_ANSWER(DumpResponse);
        ans.to = d.to;
        ans.format = d.format;
        ans.unique_time = d.time;
        return ans;
    }

    public static function asModelQueryResponse(d:ModelQueryResponseT) {
        var ans:KINJ_ANSWER = new KINJ_ANSWER(ModelQueryResponse);
        ans.model_query = d;
        return ans;
    }

    public function parseCSV():Iterator<Iterator<String>> {
      if (t != CSVResult) return null;
      return global_CSV.parse(message);  
    } 
}

@:keep 
class KINJ {
    var url:String;

    public function new(addr:String, port:Int) {
        url = "http://"+addr+":"+port;
    }

    public function stop() {
        var req = new Http(url+"/stop");
        req.onError = function(error:String) { throw error; };
        req.request(false);
    }

    function query_with_answer(str:String):String {
        var req = new Http(url+"/query");
        var responseBytes = new haxe.io.BytesOutput();
        req.setPostData(str);
        req.onError = function(error:String) { throw error; };
        req.customRequest( true, responseBytes);
        var response = responseBytes.getBytes();
        return response.toString();
    }

    public function PerformQuery(q:QueryCompiler) {
        var actual_query = querycompiler_to_query(q);
        var response = query_with_answer(actual_query);
        switch (q) {
            case SetBenchmarkingFile(filename): return KINJ_ANSWER.asMessage(response);
            case LoadDataQuery(l, file, env, withData, noStats, withMissing): return new KINJ_ANSWER(None);
            case DisplayData(l): {
                switch (l) {
                    case DropLog(x): return new KINJ_ANSWER(None);
                    default:  return KINJ_ANSWER.asCSV(response);
                }
            }
            case DoDumpLog(x): {
                var o:DumpResponse = haxe.Json.parse(response);
                return KINJ_ANSWER.asDumpResponse(o);
            }
            case DoModelQuery(m): {
                var o:ModelQueryResponseT = Json.parse(response);
                return KINJ_ANSWER.asModelQueryResponse(o);
            }
            case DoQueryPlan(q): return new KINJ_ANSWER(None);
            default: {
                var x = new KINJ_ANSWER(FutureRawString);
                x.message = response;
                return x;
            }
        }
    }
}