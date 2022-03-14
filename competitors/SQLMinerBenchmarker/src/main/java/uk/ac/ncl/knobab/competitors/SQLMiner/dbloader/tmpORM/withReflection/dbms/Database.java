package uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms;


import org.jooq.*;
import org.jooq.impl.DSL;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.dbms.rdbms.DBMSInterface;
import uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.UniqueIndex;

import java.io.*;
import java.io.File;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.sql.*;
import java.sql.Statement;
import java.util.*;

import static java.nio.charset.StandardCharsets.UTF_8;

/**
 * Public interface among different possible databases
 */
public class Database implements AutoCloseable {

    private final String name;
    private final Connection connection;
    private final DBMSInterface dialect;

    /**
     * Creates a database connection
     * @param name          Database name
     * @param connection    Java connection
     * @param dialect       Dialect associated to the RDBMS
     */
    private Database(String name, Connection connection, DBMSInterface dialect) {
        this.name = name;
        this.connection = connection;
        this.dialect = dialect;
    }

    /**
     * Returns the jOOQ query engine in java. It automatically generates the best query generator for the
     * current RDBMS dialect
     *
     * @return
     */
    public DSLContext jooq() {
        return DSL.using(connection, dialect.currentDialect());
    }

    /**
     * Returns all the tables contained within the current database
     * @return
     */
    public List<Table<?>> getTables() {
        return jooq().meta().getTables();
    }

    /**
     * Given a Class, it returns the table associated using the annotated information
     * @param clazz
     * @return
     */
    public Optional<Table<?>> getTable(Class<?> clazz) {
        return getTable(clazz.getDeclaredAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.Table.class).sqlTable());
    }

    public Optional<Table<?>> getTable(String tableName) {
        for (Table<?> table : jooq().meta().getTables()) {
            if (table.getName().equals(tableName)) {
                return Optional.of(table);
            }
        }
        return Optional.empty();
    }


    /**
     * Checks whether the table annotated within the class exists
     * @param clazz
     * @return
     */
    public boolean tableExists(Class<?> clazz) {
        String tableName = clazz.getDeclaredAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.Table.class).sqlTable();
        return jooq().meta().getTables().stream().anyMatch(x -> x.getName().equals(tableName));
    }

    /**
     * This method inserts one single row
     * @param record
     * @param <T>       Object containing the table's information
     * @return          If the table is not present, returns null, and any other object otherwise.
     */
    public <T> InsertValuesStepN<?> insertInto(T record) {
        Optional<Table<?>> table = getTable(record.getClass());
        if (table.isPresent()) {
            // For each field ff = f[i] within the record
            Field[] f = record.getClass().getDeclaredFields();
            ArrayList<Object> values = new ArrayList<>();
            CreateTableColumnStep step = null;
            for (int i = 0; i<f.length; i++) {
                Field ff = f[i];
                ff.setAccessible(true);
                // if the field is annotated to be serialized into the relational database
                if (ff.isAnnotationPresent(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.SQLType.class)) {
                    try {
                        // add its value to the collection of elements
                        values.add(ff.get(record));
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }
            // INSERT INTO record.class() ( .. fields ...) ( .. values ... )
            return jooq().insertInto(table.get(), table.get().fields()).values(values);
        }
        return null;
    }

    /**
     * This method inserts one single row
     */
    public <T> InsertValuesStepN<?> insertInto(String name, Map<String, String> schema, String[] array) {
        Optional<Table<?>> table = getTable(name);
        org.jooq.Field<?>[] fields = new org.jooq.Field[schema.size()];
        int i = 0;
        for (var x : schema.entrySet())
            fields[i++] = table.get().field(x.getKey());
        if (table.isPresent()) {
            // For each field ff = f[i] within the record
            ArrayList<Object> values = new ArrayList<>();
            CreateTableColumnStep step = null;
            for (i = 0; i<array.length; i++) {
                // if the field is annotated to be serialized into the relational database
                // add its value to the collection of elements
                values.add(array[i].isEmpty() ? null : array[i]);
            }
            // INSERT INTO record.class() ( .. fields ...) ( .. values ... )
            return jooq().insertInto(table.get(), table.get().fields()).values(values);
        }
        return null;
    }

    /**
     * Performs a batch insertion over a multitude of elements
     * @param recordIterator    Iterator over the records
     * @param limit             Batch insertion limit
     * @return                  Number of insertions with success.
     */
    public double batchInsertion(Iterator<String[]> recordIterator, int limit, String name, Map<String, String> schema) {
        if (limit <= 0)
            return 0;
        ArrayList<InsertValuesStepN<?>> batchInsertion = new ArrayList<>(limit);
        ArrayList<int[]> results = new ArrayList<>();
        long totalTimeNanos = 0;

        while (recordIterator.hasNext()) {
            int count = batchInsertion.size();
            if (count >= limit) {
                // Performs the batch insertion to the database

                long startTime = System.nanoTime();
                int[] result = jooq().batch(batchInsertion).execute();
                long endTime = System.nanoTime();

                results.add(result);
                totalTimeNanos += (endTime-startTime);
                batchInsertion.clear();
            } else {
                // adding the number of insertions for the current command
                String[] element = recordIterator.next();
                if (element != null) batchInsertion.add(insertInto(name, schema, element));
            }
        }
        if (!batchInsertion.isEmpty()) {                // Finalize the last insertion
            long startTime = System.nanoTime();
            int[] result = jooq().batch(batchInsertion).execute();
            long endTime = System.nanoTime();
            results.add(result);
            totalTimeNanos += (endTime-startTime);
        }
        return totalTimeNanos;
    }



    public <T> double batchInsertion(Iterator<T> recordIterator, int limit) {
        if (limit <= 0)
            return 0;
        ArrayList<InsertValuesStepN<?>> batchInsertion = new ArrayList<>(limit);
        ArrayList<int[]> results = new ArrayList<>();
        long totalTimeNanos = 0;
        while (recordIterator.hasNext()) {
            int count = batchInsertion.size();
            if (count >= limit) {
                // Performs the batch insertion to the database
                long startTime = System.nanoTime();
                int[] result = jooq().batch(batchInsertion).execute();
                long endTime = System.nanoTime();

                results.add(result);
                totalTimeNanos += (endTime-startTime);
                // Remove all the previous insertions
                batchInsertion.clear();
            } else {
                // adding the number of insertions for the current command
                T element = recordIterator.next();
                if (element != null) batchInsertion.add(insertInto(element));
            }
        }
        if (!batchInsertion.isEmpty()) {                // Finalize the last insertion
            long startTime = System.nanoTime();
            int[] result = jooq().batch(batchInsertion).execute();
            long endTime = System.nanoTime();
            results.add(result);
            totalTimeNanos += (endTime-startTime);
        }
        return totalTimeNanos;
    }


    /**
     * Tries to create a table using the schema information that can be inferred from the class specification
     * @param clazz
     * @return
     */
    public boolean createTableFromClass(Class<?> clazz) {
        // CREATE TABLE clazz (
        CreateTableColumnStep statement = jooq().createTableIfNotExists(clazz.getDeclaredAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.Table.class).sqlTable());
        Field[] f = clazz.getDeclaredFields();
        CreateTableColumnStep step = null;
        for (int i = 0; i<f.length; i++) {
            Field ff = f[i];
            ff.setAccessible(true);
            if (ff.isAnnotationPresent(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.SQLType.class)) {
                // fieldName fieldType
                if (step == null) {
                    step = statement.column(ff.getName(), dialect.getClassAssociatedToSQLType(ff.getAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.SQLType.class).type()));
                } else {
                    step = step.column(ff.getName(), dialect.getClassAssociatedToSQLType(ff.getAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.SQLType.class).type()));
                }
            }
        }
        // )
        try {
            step.execute();
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public boolean createTableFromSpecification(String table_name, HashMap<String, String> map) {
        // CREATE TABLE clazz (
        CreateTableColumnStep statement = jooq().createTableIfNotExists(table_name);
        CreateTableColumnStep step = null;
        for (var x : map.entrySet()) {
            step = Objects.requireNonNullElse(step, statement).column(x.getKey(), dialect.getClassAssociatedToSQLType(x.getValue()));
        }
        // )
        try {
            assert step != null;
            step.execute();
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public LinkedHashMap<String, String> createTableFromTabFile(String table_name, File file) {
        // CREATE TABLE clazz (
        CreateTableColumnStep statement = jooq().createTableIfNotExists(table_name);
        CreateTableColumnStep step = null;
        List<ArrayList<String>> arrayOfArrays = null;
        LinkedHashMap<String, String> map = new LinkedHashMap<>();
        try {
            FileInputStream fis = new FileInputStream(file);
            InputStreamReader isr = new InputStreamReader(fis, "UTF-8");
            BufferedReader br = new BufferedReader(isr);
            String line;
            while ( ( line = br.readLine() ) != null )
            {
                var array = line.split("\t");
                map.put(array[0], array[1]);
                step = Objects.requireNonNullElse(step, statement).column(array[0], dialect.getClassAssociatedToSQLType(array[1]));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        // )
        try {
            assert step != null;
            step.execute();
        } catch (Exception e) {
        }
        return map;
    }

    /**
     * Creates an unique index using the class annotations
     * @param clazz         Class through which obtain the annotations
     * @param indexName     Name of the index
     * @return
     */
    public boolean createUniqueIndex(Class<?> clazz, String indexName) {
        Field[] f = clazz.getDeclaredFields();
        // Collect all the elements that are annotated to belong to the unique index
        ArrayList<String> uniqueAnnotated = new ArrayList<>();
        for (int i = 0; i<f.length; i++) {
            Field ff = f[i];
            ff.setAccessible(true);
            if (ff.isAnnotationPresent(UniqueIndex.class)) {
                uniqueAnnotated.add(ff.getName());
            }
        }
        if (!uniqueAnnotated.isEmpty()) {
            String arrays[] = uniqueAnnotated.toArray(new String[uniqueAnnotated.size()]);
            // CREATE UNIQUE INDEX indexname ON clazz ( ... array ... )
            jooq().createUniqueIndex(indexName).on(clazz.getDeclaredAnnotation(uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model.Table.class).sqlTable(), arrays).execute();
            return true;
        }
        return false;
    }

    /**
     * Opens an existing database
     * @param iface         Database interface
     * @param dbname        Database name
     * @param username      Database username
     * @param password      Database password
     * @return  If the connection was not established, it returns an empty object, and an instance of the Database otherwise
     */
    public static Optional<Database> open(DBMSInterface iface, String dbname, String username, String password) {
        if (dbname == null)
            dbname = iface.defaultDatabaseName();
        Optional<Connection> functConnection = ConnectionBridge.connect(iface, dbname, username, password);
        if (functConnection.isPresent())
            return Optional.of(new Database(dbname, functConnection.get(), iface));
        else
            return Optional.empty();
    }


    /**
     * Opens an existing database. If such database does not exist, it tries to create one.
     * @param iface         Database interface
     * @param dbname        Database name
     * @param username      Database username
     * @param password      Database password
     * @return  If the connection was not established or/and the database cannot be created, it returns an empty object,
     *          and an instance of the Database otherwise
     */
    public static Optional<Database> openOrCreate(DBMSInterface iface, String dbname, String username, String password) {
        Optional<Database> functConnection = open(iface, dbname, username, password);
        if (functConnection.isPresent())
            return functConnection;
        else {
            // Trying to create the new database
            Optional<Database> defaultDb = open(iface, null, username, password);
            if (!defaultDb.isPresent()) {
                return Optional.empty();
            } else {
                try {
                    Statement s = defaultDb.get().connection.createStatement();
                    s.executeUpdate("CREATE DATABASE " + dbname);
                    try {
                        defaultDb.get().close();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    return open(iface, dbname, username, password);
                } catch (SQLException e) {
                    e.printStackTrace();
                    return Optional.empty();
                }
            }
        }
    }

    /**
     * Closes the database connection
     * @throws Exception
     */
    @Override
    public void close() throws Exception {
        connection.close();
    }

    /**
     * Executes a sql query from string directly using the connection (no jooq)
     * @param query
     */
    public ResultSet rawSqlQuery(String query) {
        try (Statement s = connection.createStatement()) {
            return s.executeQuery(query);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public ResultSet rawSqlQueryOpen(File file) throws IOException, SQLException {
        Statement s = connection.createStatement();
        return s.executeQuery(new String(Files.readAllBytes(file.toPath()),UTF_8));
    }

    public ResultSet rawSqlQuery(File file) throws IOException {
        return rawSqlQuery(new String(Files.readAllBytes(file.toPath()),UTF_8));
    }

    public boolean rawSqlCommand(String query) {
        try (Statement s = connection.createStatement()) {
            return s.execute(query);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public int rawSqlUpdate(String query) {
        try (Statement s = connection.createStatement()) {
            return s.executeUpdate(query);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }

    /**
     * Runs a sql statement from string directly using the connection (no jooq)
     * @param query
     */
    public void rawSqlStatement(String query) {
            try (Statement s = connection.createStatement()) {
                s.execute(query);
            } catch (Exception e) {
                e.printStackTrace();
            }
    }

    public PreparedStatement prepareStatement(String query) throws SQLException {
        return connection.prepareStatement(query);
    }

    /**
     * Runs a sql statement from file directly using the connection (no jooq)
     * @param file
     */
    public void rawSqlStatement(File file) {
        try {
            String query = new String(Files.readAllBytes(file.toPath()),UTF_8);
            rawSqlStatement(query);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
