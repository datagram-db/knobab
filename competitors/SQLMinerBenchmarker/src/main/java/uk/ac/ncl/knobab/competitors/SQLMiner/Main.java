package uk.ac.ncl.knobab.competitors.SQLMiner;

import org.jooq.Table;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.Database;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.*;

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
                fileStream.println("n_traces,model_filename,mode_size,atomization_conf,execution_time,failure");
                fileStream.flush();
            }
            File dir = new File(sqls);

            List<String> ls =
                    Arrays.asList(Objects.requireNonNull(dir.list()));
            Collections.sort(ls);
            for(String file : ls) {
                try (Database db = SQLMinerServer.databaseConnection().get()) {
                    File model_size = new File("/home/samuelappleby/Documents/CodeBases/knobab/data/testing/sqlQueries/model_size.sql");
                    ResultSet rs = db.rawSqlQueryOpen(model_size);
                    rs.last();

                    boolean withTraceInfo = file.contains("with_trace_info");
                    String ModelFilename = toCamelCase(file.replace(".sql","").replace("_with_trace_info", ""));
                    ModelFilename = ModelFilename.replace("Alternate", "Alt");
                    System.out.println(ModelFilename);
                    String AtomizationConf = withTraceInfo ? "SQLMiner + TraceInfo" : "SQLMiner + Support";
                    Timestamp startTS = null, endTS = null;
                    try {
                        for (int i = 0; i < iters; ++i) {
                            startTS = new Timestamp(System.currentTimeMillis());
                            System.out.println("Iteration: " + (i + 1) + " Start: " + startTS);
                            File test = new File(dir + "/" + file);
                            ResultSet set = db.rawSqlQueryOpen(test);
                            endTS = new Timestamp(System.currentTimeMillis());
                            set.last();
                            String part = set.getString(1);
                            Double d =  Double.parseDouble(part.replaceAll("[^0-9.]", ""));
                            fileStream.println(ntraces + "," + ModelFilename + "," + rs.getRow() + "," + AtomizationConf + "," + String.valueOf(d));
                            System.out.println("Time: " + d + " End: " + endTS);
                            long diff  = endTS.getTime() - startTS.getTime();
                            System.out.println("Diff: " + diff);
                        }
                        fileStream.flush();
                    } catch (Exception e) {
                        endTS = new Timestamp(System.currentTimeMillis());
                        long diff  = endTS.getTime() - startTS.getTime();
                        System.out.println("Failed after: " + diff);
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
        int iters = 1;
        //String sqlQueryDir = "/home/giacomo/projects/knobab/data/testing/sqlQueries/benchmarking3";
        String sqlQueryDir = "/home/samuelappleby/Documents/CodeBases/knobab/data/testing/sqlQueries/benchmarking_trace_info";
        benchmarkSQLMinerQueries(1000, sqlQueryDir, iters);
    }
}