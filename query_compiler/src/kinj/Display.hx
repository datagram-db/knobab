package kinj;
import kinj.Utils.quoted;

@:keep 
enum DisplayTable {
    ACT_TABLE;
    CNT_TABLE;
    ATT_TABLE(attr:String);
}

@:keep 
function displaytable_to_string(d:DisplayTable):String {
    switch (d) {
        case ACT_TABLE: return "ACTTABLE";
        case CNT_TABLE: return "COUNTTABLE";
        case ATT_TABLE(attr): return "ATTRIBUTETABLE "+quoted(attr);
    }
}

@:keep 
enum DisplayList {
    ATT;
    ACTIVITYLABEL;
    LOGS;
}

@:keep 
function displaylist_to_string(d:DisplayList):String {
    switch (d) {
        case ATT: return "ATTRIBUTES";
        case ACTIVITYLABEL: return "ACTIVITYLABEL";
        case LOGS: return "LOGS";
    }
}

@:keep 
enum Displays {
    Display(d:DisplayTable, f:String);
    List(d:DisplayList, f:String);
    DropLog(e:String);
}

