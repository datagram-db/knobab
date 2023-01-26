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

