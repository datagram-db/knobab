package uk.ac.ncl.knobab.competitors.SQLMiner;

import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.Database;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Main {
    public static void benchmarkSQLMinerQueries(String sqls, int iters) {
        try {
            Database db = SQLMinerServer.databaseConnection().get();

            File timeFile = new File(SQLMinerServer.getDBName() + "_timings.csv");
            if (!timeFile.exists()) {
                timeFile.createNewFile();
            }

            StringBuilder sb = new StringBuilder();
            File dir = new File(sqls);

            for(String file : dir.list()){
                try {
                    for (int i = 0; i < iters; ++i) {
                        File test = new File(dir + "/" + file);
                        ResultSet set = db.rawSqlQueryOpen(test);
                        set.last();
                        String part = set.getString(1);
                        Double d =  Double.parseDouble(part.replaceAll("[^0-9.]", ""));
                        sb.append(file + ',' + String.valueOf(d) + '\n');
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }

            try {
                Files.write(Paths.get(timeFile.getName()), sb.toString().getBytes(), StandardOpenOption.WRITE);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public static void main(String args[]) throws SQLException {
        //-queries "response.sql,alternate_response.sql,chain_response.sql,precedence.sql,alternate_precedence.sql,chain_precedence.sql,responded_existence.sql,not_succession.sql"

        int i = 0;
        int iters = 1;
        String sqlQueryDir = "";

        while (i < args.length && args[i].startsWith("-")) {
            String arg = args[i++];

            // use this type of check for arguments that require arguments
            if (arg.equals("-queries")) {
                if (i < args.length) {
                    sqlQueryDir = args[i++];
                    System.out.println("declare files = " + sqlQueryDir.toString());
                } else {
                    System.err.println("-test requires a directory");
                }
            }
            if (arg.equals("-iterations")) {
                if (i < args.length) {
                    iters = Integer.parseInt(args[i++]);
                }
            }
        }

        if(!sqlQueryDir.isEmpty()){
            benchmarkSQLMinerQueries(sqlQueryDir, iters);
        }
        else{
            SQLMinerServer.loadDATAForSQLMiner();
        }
    }
}