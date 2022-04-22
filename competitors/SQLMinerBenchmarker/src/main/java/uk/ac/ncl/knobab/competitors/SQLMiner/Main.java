package uk.ac.ncl.knobab.competitors.SQLMiner;

import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.Database;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Collections;
import java.util.Objects;

public class Main {
    static String toCamelCase(String s){
        String[] parts = s.split("_");
        String camelCaseString = "";
        for (String part : parts){
            camelCaseString = camelCaseString + toProperCase(part);
        }
        return camelCaseString;
    }

    static String toProperCase(String s) {
        return s.substring(0, 1).toUpperCase() +
                s.substring(1).toLowerCase();
    }


    public static void benchmarkSQLMinerQueries(int ntraces, String sqls, int iters) {
        try {
            File timeFile = new File(SQLMinerServer.getDBName() + "_timings.csv");
            boolean firstLine = !timeFile.exists();
            PrintStream fileStream = new PrintStream( new FileOutputStream(timeFile, true));
            if (firstLine) {
                fileStream.println("n_traces,model_filename,atomization_conf,query_time");
                fileStream.flush();
            }
            File dir = new File(sqls);

            var ls =
                    Arrays.asList(Objects.requireNonNull(dir.list()));
            Collections.reverse(ls);
            for(String file : ls) {
                try (Database db = SQLMinerServer.databaseConnection().get()) {
                    boolean withTraceInfo = file.contains("with_trace_info");
                    String ModelFilename = toCamelCase(file.replace(".sql","").replace("_with_trace_info", ""));
                    ModelFilename = ModelFilename.replace("Alternate", "Alt")+"25";
                    System.out.println(ModelFilename);
                    String AtomizationConf = withTraceInfo ? "SQLMiner + TraceInfo" : "SQLMiner + Support";
                    try {
                        for (int i = 0; i < iters; ++i) {
                            System.out.println("Iteration: " + (i + 1));
                            File test = new File(dir + "/" + file);
                            ResultSet set = db.rawSqlQueryOpen(test);
                            set.last();
                            String part = set.getString(1);
                            Double d =  Double.parseDouble(part.replaceAll("[^0-9.]", ""));
                            fileStream.println(ntraces + "," + ModelFilename + "," + AtomizationConf + "," + String.valueOf(d));
                            System.out.println("Time: " + d);
                        }
                        fileStream.flush();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String args[]) throws SQLException {
        int iters = 2;
        String sqlQueryDir = "/home/giacomo/projects/knobab/data/testing/sqlQueries/benchmarking3";
        benchmarkSQLMinerQueries(1000, sqlQueryDir, iters);
    }
}