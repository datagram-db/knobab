package uk.ac.ncl.knobab.competitors.SQLMiner.dbloader.tmpORM.withReflection.model;

import java.lang.annotation.Retention;

/**
 * Associates to each class a table name within the relational database
 */
@Retention(value=java.lang.annotation.RetentionPolicy.RUNTIME)
public @interface Table {
    String sqlTable();
}
