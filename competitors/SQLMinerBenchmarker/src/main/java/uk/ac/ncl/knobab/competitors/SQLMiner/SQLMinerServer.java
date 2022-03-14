package uk.ac.ncl.knobab.competitors.SQLMiner;

import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.Database;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.rdbms.DBMSInterface;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.rdbms.DBMSInterfaceFactory;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmptables.SourceTabLoader;

import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.nio.charset.StandardCharsets;
import java.sql.SQLException;
import java.util.*;

public class SQLMinerServer {

    private static Properties properties = new Properties();
    private static String dbname, username, password;
    private static Integer batchSize;
    private static File log_data_folder;
    private static DBMSInterface engine;

    private static boolean loadProperties() {
        if (properties.isEmpty()) try {
            properties.load(new FileInputStream("conf/postgresql.properties"));
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } else
            return true;
    }

    /**
     * Establishing an database connection
     * @return
     */
    public static Optional<Database> databaseConnection() {
        if (!loadProperties()) return Optional.empty();
        engine = DBMSInterfaceFactory.generate(properties.getProperty("engine", "PostgreSQL"));
        dbname = properties.getProperty("dbname", "ldc");
        username = properties.getProperty("username", System.getProperty("user.name"));
        password = properties.getProperty("password", "password");
        batchSize = Integer.valueOf(properties.getProperty("batchSize", "1000"));
        log_data_folder = new File(properties.getProperty("log.data.folder", "log/"));
        return Database.open(engine, dbname, username, password);
    }

    // Loading the database into PostgreSQL
    public static boolean loadDATAForSQLMiner() throws SQLException {
        Optional<Database> x = databaseConnection();
        if (x.isEmpty()) {
            if (!loadProperties()) return false;
            Optional<Database> opt = Database.openOrCreate(engine, dbname, username, password);

            if (opt.isPresent()) {
                Database database = opt.get();
                System.out.println("Generating the tables and loading the data");
                createLDCDataSchema(database, log_data_folder, batchSize);

            } else {
                System.err.println("Error creating the new database (");
            }
            return true;
        } else {
            try {
                var it = x.get().rawSqlQueryOpen(new File("/home/giacomo/IdeaProjects/JavaConcurrentAPI/SQLMinerBenchmarker/data_mining_patterns/response.sql"));
                while (it.next()) {
                    System.out.println(
                            it.getString(1));
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            return false;
        }
    }

    /**
     *
     * @param database      Database connection
     * @param clazz         Stub class that will be used to lad the data from the file and to provide it to the relational database
     * @param batchSize     Size for the batch insertion
     * @param <T>           Type of the stub class
     * @param <Sub>
     */
    private static <T extends SourceTabLoader, Sub extends T> double instantiateTable(Database database, File path, Class<Sub> clazz, int batchSize) {
        System.out.println("Creating table: "+clazz.getName());
        database.createTableFromClass(clazz);
        try {
            // Dummy object allowing to using reflection for loading data as records
            Sub object = clazz.getDeclaredConstructor().newInstance();

            // Creating a data iterator
            Iterator<Sub> iterator = SourceTabLoader
                    // Loading the data from the current file
                    .loadFromFolder(object, path);

            // Performing a bacth insertion each
            return database.batchInsertion(iterator, batchSize);
        } catch (InstantiationException | IllegalAccessException | IOException | NoSuchMethodException | InvocationTargetException e) {
            e.printStackTrace();
            return 0.0;
        }
    }

    private static <T extends SourceTabLoader, Sub extends T> double instantiateTable(Database database, String table_name, File path, File tabSchema, int batchSize) {
        System.out.println("Creating table: "+table_name);
        LinkedHashMap<String, String> map = database.createTableFromTabFile(table_name, tabSchema);
        if (map.size() == 2 && map.containsKey("__time") && map.containsKey("event_id")) return 0;
        try {

            final BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(path), StandardCharsets.UTF_8));

            // Removing the header;
            br.readLine();

            // Reading the first line after the header
            String ln = br.readLine();

            // File iterator
            Iterator<String[]> it = new Iterator<>() {
                final BufferedReader file = br;
                String line = ln;

                @Override
                public boolean hasNext() {
                    return line != null;
                }

                @Override
                public String[] next() {
                    String toReturn = line;
                    try {
                        line = br.readLine();
                    } catch (IOException e) {
                        return null;
                    }
                    return toReturn.split("\t");
                }
            };

            // Performing a bacth insertion each
            return database.batchInsertion(it, batchSize, table_name, map);
        } catch ( IOException e) {
            e.printStackTrace();
            return 0;
        }
    }

    /**
     * Loads each possible class
     * @param database
     * @param ldcData
     */
    private static void createLDCDataSchema(Database database, File ldcData, int batchSize) {
        // Loading the data from all of these
        Properties schema = new Properties();
        double totalNano = instantiateTable(database, new File(ldcData, "log.tab"), LogRecord.class, batchSize) +
        instantiateTable(database, "payload", new File(ldcData, "payload.tab"), new File(ldcData, "payloadschema.tab"), batchSize);
        System.out.println("Total Loading Time: " + (totalNano/ 1000000.0));
    }

}
